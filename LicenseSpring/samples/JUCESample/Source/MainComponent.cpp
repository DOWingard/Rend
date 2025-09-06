#include "MainComponent.h"
#include <LicenseSpring/Exceptions.h>
using namespace juce;

//==============================================================================
MainComponent::MainComponent(LicenseSpring::LicenseManager::ptr_t manager) : licenseManager(manager)
{

    addChildComponent(labelKeyBased);
    addChildComponent(labelUserBased);
    addChildComponent(labelUser);
    addChildComponent(labelKey);
    addChildComponent(labelPassword);
    addChildComponent(labelGetTrial);
    addChildComponent(labelUser2);
    addChildComponent(getTrialButton);
    addChildComponent(activateKeyButton);
    addChildComponent(activateUserButton);
    addChildComponent(keyEditor);
    addChildComponent(userEditor);
    addChildComponent(passwordEditor);
    addChildComponent(userEditor2);
    String path;
#ifdef JUCE_WINDOWS
    path = File::getCurrentWorkingDirectory()
               .getParentDirectory()
               .getParentDirectory()
               .getFullPathName() +
           "\\Resources\\logo.png";
#endif

#ifdef JUCE_MAC
    path = File::getCurrentWorkingDirectory()
               .getParentDirectory()
               .getParentDirectory()
               .getParentDirectory()
               .getParentDirectory()
               .getFullPathName() +
           "/Resources/logo.png";
#endif
    image.setImage(ImageCache::getFromFile(File(path)));
    addChildComponent(image);

    addChildComponent(labelInfo);
    addChildComponent(deactivateButton);
    addChildComponent(checkButton);

    userEditor2.addListener(this);
    userEditor.addListener(this);
    activateKeyButton.addListener(this);
    activateUserButton.addListener(this);
    getTrialButton.addListener(this);
    deactivateButton.addListener(this);
    checkButton.addListener(this);

    activateKeyButton.setComponentID("Activate key-based");
    activateUserButton.setComponentID("Activate user-based");
    getTrialButton.setComponentID("Get trial");
    deactivateButton.setComponentID("Deactivate");
    checkButton.setComponentID("Check");

    auto license = licenseManager->getCurrentLicense();

    if (license != nullptr)
    {
        updateLicenseFields();
        makeInfoVisible();
    }
    else
        makeActivationVisible();

    setSize(800, 600);
}

MainComponent::~MainComponent() {}

void MainComponent::makeInfoVisible()
{
    labelKeyBased.setVisible(false);
    labelUserBased.setVisible(false);
    labelUser.setVisible(false);
    labelKey.setVisible(false);
    labelPassword.setVisible(false);
    labelGetTrial.setVisible(false);
    labelUser2.setVisible(false);
    getTrialButton.setVisible(false);
    activateKeyButton.setVisible(false);
    activateUserButton.setVisible(false);
    keyEditor.setVisible(false);
    userEditor.setVisible(false);
    passwordEditor.setVisible(false);
    userEditor2.setVisible(false);
    image.setVisible(false);

    labelInfo.setVisible(true);
    deactivateButton.setVisible(true);
    checkButton.setVisible(true);
}

void MainComponent::makeActivationVisible()
{
    labelInfo.setVisible(false);
    deactivateButton.setVisible(false);
    checkButton.setVisible(false);

    labelKeyBased.setVisible(true);
    labelUserBased.setVisible(true);
    labelUser.setVisible(true);
    labelKey.setVisible(true);
    labelPassword.setVisible(true);
    labelGetTrial.setVisible(true);
    labelUser2.setVisible(true);
    getTrialButton.setVisible(true);
    activateKeyButton.setVisible(true);
    activateUserButton.setVisible(true);
    keyEditor.setVisible(true);
    userEditor.setVisible(true);
    passwordEditor.setVisible(true);
    userEditor2.setVisible(true);
    image.setVisible(true);
}

void MainComponent::buttonClicked(juce::Button *button)
{
    auto buttonId = button->getComponentID();
    if (buttonId == activateKeyButton.getComponentID())
        activateKeyBased();
    if (buttonId == activateUserButton.getComponentID())
        activateUserBased();
    if (buttonId == getTrialButton.getComponentID())
        getTrial();
    if (buttonId == deactivateButton.getComponentID())
        deactivate();
    if (buttonId == checkButton.getComponentID())
        checkLicense();
}

void MainComponent::activateKeyBased()
{
    try
    {
        auto key = keyEditor.getText();
        if (!key.isEmpty())
        {
            LicenseSpring::LicenseID id = LicenseSpring::LicenseID::fromKey(key.toStdString());
            auto license = licenseManager->activateLicense(id);
            if (license->isValid())
            {
                updateLicenseFields();
                makeInfoVisible();
            }
        }
        else
            AlertWindow::showMessageBoxAsync(
                AlertWindow::WarningIcon, "Warning", "Type key for activation", "Ok");
    }
    catch (LicenseSpring::LicenseSpringException ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("LicenseSpring exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (std::exception ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("Standard exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (...)
    {
        AlertWindow::showMessageBoxAsync(
            AlertWindow::WarningIcon, "Error", String("Unknown exception encountered!"), "Ok");
    }
}

void MainComponent::activateUserBased()
{
    try
    {
        auto user = userEditor.getText();
        auto password = passwordEditor.getText();
        if (!user.isEmpty() && !password.isEmpty())
        {
            auto license = licenseManager->activateLicense(
                LicenseSpring::LicenseID::fromUser(user.toStdString(), password.toStdString()));
            if (license->isValid())
            {
                updateLicenseFields();
                makeInfoVisible();
            }
        }
        else
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Warning",
                "Type user emal and password for activation", "Ok");
    }
    catch (LicenseSpring::LicenseSpringException ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("LicenseSpring exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (std::exception ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("Standard exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (...)
    {
        AlertWindow::showMessageBoxAsync(
            AlertWindow::WarningIcon, "Error", String("Unknown exception encountered!"), "Ok");
    }
}

void MainComponent::getTrial()
{
    try
    {
        auto user = userEditor2.getText();
        auto id = licenseManager->getTrialLicense(user.toStdString());
        userEditor.setText(id.user().c_str());
        passwordEditor.setText(id.password().c_str());
        keyEditor.setText(id.key().c_str());
    }
    catch (LicenseSpring::LicenseSpringException ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("LicenseSpring exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (std::exception ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("Standard exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (...)
    {
        AlertWindow::showMessageBoxAsync(
            AlertWindow::WarningIcon, "Error", String("Unknown exception encountered!"), "Ok");
    }
}

void MainComponent::updateLicenseFields()
{
    try
    {
        auto license = licenseManager->getCurrentLicense();
        if (license == nullptr)
            return;

        String text = "";

        text += "\n------------- License info -------------\n";

        const auto &licenseOwner = license->owner();
        {
            std::string ownerInfo;
            auto formatInfo = [&ownerInfo](const std::string &value) {
                if (value.empty())
                    return;
                if (!ownerInfo.empty())
                    ownerInfo.append(" ");
                ownerInfo.append(value);
            };
            formatInfo(licenseOwner.firstName());
            formatInfo(licenseOwner.lastName());
            formatInfo(licenseOwner.email());
            formatInfo(licenseOwner.company());
            if (!ownerInfo.empty())
            {
                text += "Licensed to: ";
                text += ownerInfo.c_str();
                text += "\n";
            }
        }

        if (!license->key().empty())
        {
            text += "Key = ";
            text += license->key().c_str();
            text += "\n";
        }

        if (!license->user().empty())
        {
            text += "User = ";
            text += license->user().c_str();
            text += "\n";
        }

        text += "Type = ";
        text += license->type().toFormattedString().c_str();
        text += "\n";
        text += "Status = ";
        text += license->status().c_str();
        text += "\n";
        text += "IsActive = ";
        text += license->isActive() ? "True" : "False";
        text += "\n";
        text += "IsEnabled = ";
        text += license->isEnabled() ? "True" : "False";
        text += "\n";
        text += "IsTrial = ";
        text += license->isTrial() ? "True" : "False";
        text += "\n";
        text += "IsFloating = ";
        text += license->isFloating() ? "True" : "False";
        text += "\n";
        text += "IsOfflineActivated = ";
        text += license->isOfflineActivated() ? "True" : "False";
        text += "\n";
        text += "Times activated = ";
        text += static_cast<int>(license->timesActivated());
        text += "\n";
        text += "Max activations = ";
        text += static_cast<int>(license->maxActivations());
        text += "\n";

        auto TmToStr = [](const tm &dateTime) {
            return LicenseSpring::TmToString(dateTime, "%d-%m-%Y %H:%M:%S");
        };

        text += "Validity Period = ";
        text += TmToStr(license->validityPeriod()).c_str();
        text += "\n";
        text += "Validity Period UTC = ";
        text += TmToStr(license->validityPeriodUtc()).c_str();
        text += "\n";
        text += "Days remaining till license expires = ";
        text += license->daysRemaining();
        text += "\n";
        text += "Maintenance period = ";
        text += TmToStr(license->maintenancePeriod()).c_str();
        text += "\n";
        text += "Maintenance period UTC = ";
        text += TmToStr(license->maintenancePeriodUtc()).c_str();
        text += "\n";
        text += "Maintenance days remaining = ";
        text += license->maintenanceDaysRemaining();
        text += "\n";
        text += "Last online check date = ";
        text += TmToStr(license->lastCheckDate()).c_str();
        text += "\n";
        text += "Days passed since last online check = ";
        text += license->daysPassedSinceLastCheck();
        text += "\n";

        auto productFeatures = license->features();
        if (!productFeatures.empty())
        {
            text += "Product features available for this license:\n";
            for (auto feature : productFeatures)
            {
                text += feature.toString().c_str();
                text += "\n";
            }
        }

        auto dataFields = license->customFields();
        if (!dataFields.empty())
        {
            text += "Custom data fields available for this license:\n";
            for (const auto &field : dataFields)
            {
                text += "Data field - Name: ";
                text += field.fieldName().c_str();
                text += ", Value: ";
                text += field.fieldValue().c_str();
                text += "\n";
            }
        }

        const auto &userData = license->userData();
        if (!userData.empty())
        {
            text += "User data for this license: \n";
            for (const auto &field : userData)
            {
                text += "Data field - Name: ";
                text += field.fieldName().c_str();
                text += ", Value: ";
                text += field.fieldValue().c_str();
                text += "\n";
            }
        }

        if (license->type() == LicenseTypeConsumption)
        {
            text += "Total consumptions = ";
            text += license->totalConsumption();
            text += "\n";
            text += "Max consumptions = ";
            text += license->maxConsumption();
            text += "\n";
            text += "Is overages allowed = ";
            text += license->isOveragesAllowed();
            text += "\n";
            if (license->isOveragesAllowed())
            {
                text += "Max overages = ";
                text += license->maxOverages();
                text += "\n";
            }
        }

        labelInfo.setText(text, NotificationType::dontSendNotification);
    }
    catch (LicenseSpring::LicenseSpringException ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("LicenseSpring exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (std::exception ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("Standard exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (...)
    {
        AlertWindow::showMessageBoxAsync(
            AlertWindow::WarningIcon, "Error", String("Unknown exception encountered!"), "Ok");
    }
}

void MainComponent::deactivate()
{
    try
    {
        licenseManager->getCurrentLicense()->deactivate(true);
        makeActivationVisible();
    }
    catch (LicenseSpring::LicenseSpringException ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("LicenseSpring exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (std::exception ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("Standard exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (...)
    {
        AlertWindow::showMessageBoxAsync(
            AlertWindow::WarningIcon, "Error", String("Unknown exception encountered!"), "Ok");
    }
}

void MainComponent::checkLicense()
{
    try
    {
        licenseManager->getCurrentLicense()->check();
        updateLicenseFields();
        AlertWindow::showMessageBoxAsync(
            AlertWindow::InfoIcon, "Info", "License check sucessful!", "Ok");
    }
    catch (LicenseSpring::LicenseSpringException ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("LicenseSpring exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (std::exception ex)
    {
        AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Error",
            String("Standard exception encountered: ") + String(ex.what()), "Ok");
    }
    catch (...)
    {
        AlertWindow::showMessageBoxAsync(
            AlertWindow::WarningIcon, "Error", String("Unknown exception encountered!"), "Ok");
    }
}

void MainComponent::textEditorTextChanged(TextEditor &editor)
{
    if (editor.getText() == userEditor2.getText())
        userEditor.setText(userEditor2.getText().toLowerCase());
    else
        userEditor2.setText(userEditor.getText().toLowerCase());
}
//==============================================================================
void MainComponent::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
}

void MainComponent::resized()
{
    int y = 40;
    image.setBounds((getWidth() - 200) / 2, 40, 200, 110);
    y += 110 + 40;
    int width = (getWidth() - 80) / 3;
    int column1X = 20;
    int column2X = column1X * 2 + width;
    int column3X = column1X + column2X + width;

    labelKeyBased.setBounds(column1X, y, width, 20);
    labelKey.setBounds(column1X, y + 30, width, 20);
    keyEditor.setBounds(column1X, y + 60, width, 20);
    activateKeyButton.setBounds(column1X, y + 90, width, 20);

    labelUserBased.setBounds(column2X, y, width, 20);
    labelUser.setBounds(column2X, y + 30, width, 20);
    userEditor.setBounds(column2X, y + 60, width, 20);
    labelPassword.setBounds(column2X, y + 90, width, 20);
    passwordEditor.setBounds(column2X, y + 120, width, 20);
    activateUserButton.setBounds(column2X, y + 150, width, 20);

    labelGetTrial.setBounds(column3X, y, width, 20);
    labelUser2.setBounds(column3X, y + 30, width, 20);
    userEditor2.setBounds(column3X, y + 60, width, 20);
    getTrialButton.setBounds(column3X, y + 90, width, 20);

    labelInfo.setBounds(20, 20, getWidth() - 40, getHeight() - 50);
    deactivateButton.setBounds(20, getHeight() - 40, getWidth() / 2 - 30, 20);
    checkButton.setBounds(getWidth() / 2, getHeight() - 40, getWidth() / 2 - 30, 20);

    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
