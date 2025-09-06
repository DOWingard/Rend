#include <QApplication>
#include <QSharedMemory>
#include <QDebug>
#include <QObject>
#include <QThread>
#include <QPointer>
#include <QMessageBox>
#include <QLocalSocket>
#include <QLocalServer>
#include <QWebEngineUrlScheme>
#include <LicenseSpring/LicenseHandler.h>
#include <csignal>
#include "MainWindow.h"
#include "../AppConfig.h"

const quint16 LocalConnectionTimeout = 3000;

#ifndef Q_OS_WIN
void exitOnUnixSignals(std::initializer_list<int> quitSignals)
{
    auto handler = [](int sig) -> void {
        qDebug() << "Caught signal " << sig << ", exiting" << Qt::endl;
        QCoreApplication::quit();
    };

    sigset_t blocking_mask;
    sigemptyset(&blocking_mask);
    for (auto sig : quitSignals)
        sigaddset(&blocking_mask, sig);

    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_mask = blocking_mask;
    sa.sa_flags = 0;

    for (auto sig : quitSignals)
        sigaction(sig, &sa, nullptr);
}
#endif

int main(int argc, char *argv[])
{
    const QString appVersion = QString::number(VERSION_MAJOR) + "." +
                               QString::number(VERSION_MINOR) + "." +
                               QString::number(VERSION_BUILD);
    const QString appLocalServerName = "sdksso_local_serv";
    AppConfig appConfig("SSO C++ SDK Sample", appVersion.toStdString().c_str());

    QWebEngineUrlScheme scheme("licensespring");
    scheme.setSyntax(QWebEngineUrlScheme::Syntax::HostAndPort);
    scheme.setDefaultPort(8080);
    scheme.setFlags(QWebEngineUrlScheme::CorsEnabled | QWebEngineUrlScheme::LocalAccessAllowed |
                    QWebEngineUrlScheme::ServiceWorkersAllowed |
                    QWebEngineUrlScheme::ContentSecurityPolicyIgnored);
    QWebEngineUrlScheme::registerScheme(scheme);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

#ifndef Q_OS_WIN
    exitOnUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});
#endif

    QSharedMemory sharedMemory(QString("sdksso_memory_lock"));
    if (!sharedMemory.create(8, QSharedMemory::ReadOnly))
    {
        // app already running, so activate window and try to open SSO activation url, if supplied
        // via argv[]
        qDebug() << "Already running, sending command to existing instance" << Qt::endl;
        QLocalSocket socket;
        auto logSocketError = [&socket]() {
            qDebug() << "Local socket error" << socket.error() << socket.errorString();
        };

        socket.connectToServer(appLocalServerName);
        if (socket.error() == QLocalSocket::ServerNotFoundError)
        {
            logSocketError();
            qDebug() << "Waiting for server " << LocalConnectionTimeout / 2 << "milliseconds";
            // wait a while for server to start
            QThread::msleep(LocalConnectionTimeout / 2);
            socket.connectToServer(appLocalServerName);
        }

        if (socket.error() != QLocalSocket::UnknownSocketError)
        {
            logSocketError();
            QMessageBox::warning(nullptr, "Error", socket.errorString());
            return 1;
        }

        if (socket.waitForConnected(LocalConnectionTimeout))
        {
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);

            // search SSO auth URL in arguments and send to running instance if found
            for (int i = 1; i < argc; i++)
            {
                if (QString(argv[i]).startsWith("licensespring://"))
                {
                    out << QString(argv[i]);
                    break;
                }
            }

            if (block.length())
                socket.write(block);
            socket.waitForBytesWritten(-1);
            socket.waitForDisconnected(-1);
            return 0;
        }

        QMessageBox::warning(
            nullptr, "Error", "Cannot connect to local server. " + socket.errorString());
        return 2;
    }

    auto lsConfig = appConfig.createLicenseSpringConfig();
    LicenseSpring::LicenseHandler::instance().reconfigure(lsConfig);

    MainWindow w;
    w.setStartState();
    w.show();

    QLocalServer server;
    server.setSocketOptions(QLocalServer::WorldAccessOption);
    server.listen(appLocalServerName);
    auto localSocketConnection = [&server, &w]() {
        QPointer<QLocalSocket> pSocket = server.nextPendingConnection();
        qDebug() << "Got incoming connection";
        if (!pSocket->waitForReadyRead(LocalConnectionTimeout))
        {
            qDebug() << QString("Socket error: ").append(pSocket->errorString());
            pSocket->disconnectFromServer();
            return;
        }

        if (pSocket->bytesAvailable() == 0)
        {
            qDebug() << "Illegal (without data) connection occurred!";
            pSocket->disconnectFromServer();
            return;
        }

        QString req;
        QDataStream in(pSocket);
        in >> req;
        pSocket->disconnectFromServer();
        qDebug() << "Received data: " << req << Qt::endl;

        if (req.startsWith("licensespring://"))
        {
            auto &lh = LicenseSpring::LicenseHandler::instance();
            if (lh.isLicenseActive())
                QMessageBox::warning(
                    nullptr, "Error", "Existing active license was found, auth URL ignored");
            else
            {
                w.show();
                w.activateWindow();
                w.setSSOArgUrl(req);
            }
        }
    };

    QObject::connect(&server, &QLocalServer::newConnection, localSocketConnection);

    return app.exec();
}
