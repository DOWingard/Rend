#pragma once

#include <JuceHeader.h>
#include <LicenseSpring/LicenseManager.h>
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component,
                      public juce::TextEditor::Listener,
                      public juce::Button::Listener
{
public:
    //==============================================================================
    MainComponent(LicenseSpring::LicenseManager::ptr_t manager);
    ~MainComponent() override;
    void textEditorTextChanged(juce::TextEditor &editor) override;
    void buttonClicked(juce::Button *) override;
    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    LicenseSpring::LicenseManager::ptr_t licenseManager;
    juce::Label labelKeyBased{{}, "For key-based product enter"};
    juce::Label labelUserBased{{}, "For user-based product enter"};
    juce::Label labelGetTrial{{}, "Get trial"};
    juce::Label labelUser{{}, "User"};
    juce::Label labelUser2{{}, "User"};
    juce::Label labelKey{{}, "Key"};
    juce::Label labelPassword{{}, "Password"};
    juce::TextButton activateKeyButton{"Activate"};
    juce::TextButton activateUserButton{"Activate"};
    juce::TextButton getTrialButton{"Get trial"};
    juce::TextEditor keyEditor;
    juce::TextEditor userEditor;
    juce::TextEditor userEditor2;
    juce::TextEditor passwordEditor;
    juce::ImageComponent image;

    juce::Label labelInfo{{}, ""};
    juce::TextButton deactivateButton{"Dectivate"};
    juce::TextButton checkButton{"Check license"};

    void activateKeyBased();
    void activateUserBased();
    void getTrial();
    void deactivate();
    void checkLicense();
    void updateLicenseFields();
    void makeInfoVisible();
    void makeActivationVisible();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
