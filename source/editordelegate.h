//------------------------------------------------------------------------
// Copyright(c) 2025 Void Audio.
//------------------------------------------------------------------------
#pragma once

#include "vstgui\plugin-bindings\vst3editor.h"
#include "vstgui\lib\cbitmap.h"


#include "licenser.h"


namespace VOID {


class SwellEditorDelegate : public  VSTGUI::VST3EditorDelegate, public VSTGUI::IControlListener
{

protected:
    VSTGUI::CBitmap* bitmap1 = createBitmapFromPath("C:\\Users\\Derek Wingard\\Desktop\\Work\\Plugins\\Swell\\resource\\shell\\shell1.png");
    VSTGUI::CBitmap* bitmap2 = createBitmapFromPath("C:\\Users\\Derek Wingard\\Desktop\\Work\\Plugins\\Swell\\resource\\shell\\shell2.png");
    VSTGUI::CBitmap* bitmap3 = createBitmapFromPath("C:/Users/Derek Wingard/Downloads/image1.png");
   
    
    VSTGUI::VST3Editor* editor = nullptr;
    VSTGUI::ParameterChangeListener* paramChangeListener = nullptr;
    LicenseOverlayView* licenseOverlay = nullptr;
    //LicenseSpring::LicenseManager::ptr_t licenseManager;
    
public:
    
	int switchstate = 0; 
    // VSTGUI::CBitmap* backgroundBitmap = nullptr;
	// VSTGUI::ParameterChangeListener* paramListener = nullptr;

	// For example, if your switch parameter is named "SwitchParam"
	Steinberg::Vst::ParamID switchParamID = 13;

	VSTGUI::CView* createCustomView (VSTGUI::UTF8StringPtr name, const VSTGUI::UIAttributes& attributes,
							 const VSTGUI::IUIDescription* description, VSTGUI::VST3Editor* editor) override
	{

        // If online, return nullptr to allow default behavior
        return nullptr;

	}





void didOpen(VSTGUI::VST3Editor* editor) override
{
    //using namespace LicenseSpring;

    this->editor = editor;
    if (!editor || !editor->getFrame())
        return;

    auto* frame = editor->getFrame();
    auto* controller = editor->getController();
    if (!controller)
        return;

    // --- Register listener for switch control ---
    if (VSTGUI::CControl* switchControl = findControlByTag(frame, 13)) {
        switchControl->registerControlListener(this);
    }

    // --- Get switch param state ---
    float switchState = 0.f;
    if (auto* param = controller->getParameterObject(13)) {
        switchState = param->getNormalized();
    }

    // --- Get main view container ---
    VSTGUI::CView* rootView = frame->getView(0);
    auto* mainContainer = dynamic_cast<VSTGUI::CViewContainer*>(rootView);
    if (!mainContainer)
        return;

    // --- Inject license overlay ---
    // LicenseOverlayView will check license state internally in its constructor

    CRect overlaySize = mainContainer->getViewSize();
    licenseOverlay = new LicenseOverlayView(overlaySize);
    //licenseOverlay->setOnActivatedCallback([=] { onLicenseActivated(); });
    //mainContainer->addView(licenseOverlay);


    // --- Set background based on switch state ---
    if (switchState <= 0.5f) {
        mainContainer->setBackground(bitmap1);
    } else {
        mainContainer->setBackground(bitmap2);
    }
    mainContainer->invalid();
    frame->setZoom(0.5);


}


// void onLicenseActivated()
// {
//     if (editor && editor->getFrame() && licenseOverlay) {
//         editor->getFrame()->removeView(licenseOverlay.get());
//         licenseOverlay.reset();
//     }
// }

void willClose(VSTGUI::VST3Editor* editor) override
{
	// Undo the shit

}
void valueChanged(VSTGUI::CControl* pControl) override {
        if (pControl->getTag() == 13)
        {	
			VSTGUI::CViewContainer* myContainer = nullptr;
			VSTGUI::CView* view = editor->getFrame()->getView(0);
            myContainer = dynamic_cast<VSTGUI::CViewContainer*>(view);
            // Get switch value (0.0 = off, 1.0 = on)
            float switchstate = pControl->getValue();
			if (switchstate <= 0.5) {
				myContainer->setBackground(bitmap1);
				myContainer->invalid();
                editor->getFrame()->setZoom(0.5);
			} else if (switchstate > 0.5) {
				myContainer->setBackground(bitmap2);
				myContainer->invalid();
                editor->getFrame()->setZoom(0.5);
                    }
            //changeBackgroundImage(switchstate);
	
		}
}
VSTGUI::CBitmap* createBitmapFromPath(const std::string& filePath)
{
    VSTGUI::CResourceDescription desc(filePath.c_str());
    return new VSTGUI::CBitmap(desc);
}



private:

// Helper function to find control by tag using documented VSTGUI methods
VSTGUI::CControl* findControlByTag(VSTGUI::CViewContainer* container, int32_t tag)
{
    if (!container) return nullptr;
    
    // Use documented CViewContainer methods: getNbViews() and getView()
    uint32_t numViews = container->getNbViews();
    for (uint32_t i = 0; i < numViews; i++) {
        VSTGUI::CView* view = container->getView(i);
        
        // Try to cast to CControl and check tag
        if (auto control = dynamic_cast<VSTGUI::CControl*>(view)) {
            if (control->getTag() == tag) {
                return control;
            }
        }
        
        // Recursively search child containers
        if (auto childContainer = dynamic_cast<VSTGUI::CViewContainer*>(view)) {
            VSTGUI::CControl* result = findControlByTag(childContainer, tag);
            if (result) return result;
        }
    }
    return nullptr;


}
};
}