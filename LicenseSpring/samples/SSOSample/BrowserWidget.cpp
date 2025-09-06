#include "BrowserWidget.h"
#include "ui_BrowserWidget.h"

#include <QDir>
#include <QDebug>
#include <QUrlQuery>
#include <QWebEngineProfile>
#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlRequestInterceptor>

#include <LicenseSpring/LicenseHandler.h>

const QString LS_URL_Scheme = QStringLiteral("licensespring");

using namespace LicenseSpring;

/*class LSRequestInterceptor : public QWebEngineUrlRequestInterceptor
{
public:
   LSRequestInterceptor( QObject* parent = nullptr )
      : QWebEngineUrlRequestInterceptor( parent ) {}
   void interceptRequest( QWebEngineUrlRequestInfo& info ) override
   {
      auto url = info.requestUrl();
      if( url.scheme() == LS_URL_Scheme )
      {
         auto* browser = static_cast<BrowserWidget*>( parent() );
         browser->onUrlChanged( url );
      }
   }
};*/

// TODO: Investigate why Qt WebEngine hangs when redirecting to custom URL such as
// licensespring://...
class LSSchemeHandler : public QWebEngineUrlSchemeHandler
{
public:
    LSSchemeHandler(QObject *parent) : QWebEngineUrlSchemeHandler(parent) {}
    void requestStarted(QWebEngineUrlRequestJob *request) override
    {
        auto *browser = static_cast<BrowserWidget *>(parent());
        browser->onUrlChanged(request->requestUrl());
    }
};

BrowserWidget::BrowserWidget(QWidget *parent) : QWidget(parent), ui(new Ui::BrowserWidget)
{
    ui->setupUi(this);

    connect(ui->webView, &QWebEngineView::urlChanged, this, &BrowserWidget::onUrlChanged);

    auto *profile = QWebEngineProfile::defaultProfile();
    if (profile)
    {
        QString dataPath =
            QString::fromStdWString(LicenseHandler::instance().dataLocation()) + "/BrowserData";
        profile->setCachePath(QDir::toNativeSeparators(dataPath + "/Cache"));
        profile->setDownloadPath(QDir::toNativeSeparators(dataPath + "/Downloads"));
        profile->setPersistentStoragePath(QDir::toNativeSeparators(dataPath + "/Persistent"));

        auto *handler = new LSSchemeHandler(this);
        profile->installUrlSchemeHandler(LS_URL_Scheme.toLatin1(), handler);

        // alternative approach to handle custom URL scheme
        /*auto* interceptor = new LSRequestInterceptor( this );
        profile->setUrlRequestInterceptor( interceptor );*/
    }
}

BrowserWidget::~BrowserWidget() { delete ui; }

void BrowserWidget::setUrl(QString url) { ui->webView->setUrl(url); }

void BrowserWidget::onUrlChanged(const QUrl &url)
{
    qDebug() << "URL changed:" << url;

    if (url.hasFragment())
    {
        auto query = QUrlQuery(url.fragment());
        const QString tokenParamName("id_token");
        if (query.hasQueryItem(tokenParamName))
        {
            auto token = query.queryItemValue(tokenParamName);
            qDebug() << "Auth token:" << token;
            auto &lh = LicenseHandler::instance();
            lh.activateLicense(token.toStdString(), "", false);
            if (lh.wasError())
                Q_EMIT errorOccurred();
            else
                Q_EMIT licenseActivated();
        }
        return;
    }

    if (url.scheme() == LS_URL_Scheme && url.hasQuery())
    {
        auto query = QUrlQuery(url.query());
        const QString codeParamName("code");
        if (query.hasQueryItem(codeParamName))
        {
            auto code = query.queryItemValue(codeParamName);
            qDebug() << "Auth code:" << code;
            auto &lh = LicenseHandler::instance();
            lh.activateLicense(code.toStdString());
            if (lh.wasError())
                Q_EMIT errorOccurred();
            else
                Q_EMIT licenseActivated();
        }
    }
}
