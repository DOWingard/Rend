//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include "vstgui\plugin-bindings\vst3editor.h"




namespace VOID {


class SwellEditorDelegate : public  VSTGUI::VST3EditorDelegate, public VSTGUI::IControlListener
{
public:
	int switchstate = 0; 
    // VSTGUI::CBitmap* backgroundBitmap = nullptr;
	// VSTGUI::ParameterChangeListener* paramListener = nullptr;

	// For example, if your switch parameter is named "SwitchParam"
	Steinberg::Vst::ParamID switchParamID = 13;

	VSTGUI::CView* createCustomView (VSTGUI::UTF8StringPtr name, const VSTGUI::UIAttributes& attributes,
							 const VSTGUI::IUIDescription* description, VSTGUI::VST3Editor* editor) override
	{
		return nullptr;
	}





void didOpen(VSTGUI::VST3Editor* editor) override
{
    this->editor = editor;
    if (editor && editor->getFrame())
    {
        auto controller = editor->getController();
        if (!controller) return;
        
        // Find and register listener for control with tag 13
        VSTGUI::CControl* switchControl = findControlByTag(editor->getFrame(), 13);
        if (switchControl) {
            switchControl->registerControlListener(this);
        }
        
        auto param = controller->getParameterObject(13);
        if (param) {
            float switchstate = param->getNormalized();
           
            // Method 1: Find by iterating through child views (if you know the index)
            VSTGUI::CViewContainer* myContainer = nullptr;
            for (uint32_t i = 0; i < 1; i++) {
                VSTGUI::CView* view = editor->getFrame()->getView(i);
                myContainer = dynamic_cast<VSTGUI::CViewContainer*>(view);
                if (myContainer && myContainer->getWidth() == 765) {
                    if (switchstate <= 0.5) {
                        VSTGUI::CBitmap* bitmap1 = createBitmapFromPath("C:\\Users\\Derek Wingard\\Desktop\\Work\\Plugins\\Swell\\resource\\shell\\shell1.png");
                        myContainer->setBackground(bitmap1);
                        myContainer->invalid();
                    } else if (switchstate > 0.5) {
                        VSTGUI::CBitmap* bitmap2 = createBitmapFromPath("C:\\Users\\Derek Wingard\\Desktop\\Work\\Plugins\\Swell\\resource\\shell\\shell2.png");
                        myContainer->setBackground(bitmap2);
                        myContainer->invalid();
                    }
                }
            }
        }
    }
}
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
				VSTGUI::CBitmap* bitmap1 = createBitmapFromPath("C:\\Users\\Derek Wingard\\Desktop\\Work\\Plugins\\Swell\\resource\\shell\\shell1.png");
				myContainer->setBackground(bitmap1);
				myContainer->invalid();
			} else if (switchstate > 0.5) {
				VSTGUI::CBitmap* bitmap2 = createBitmapFromPath("C:\\Users\\Derek Wingard\\Desktop\\Work\\Plugins\\Swell\\resource\\shell\\shell2.png");
				myContainer->setBackground(bitmap2);
				myContainer->invalid();
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

VSTGUI::VST3Editor* editor = nullptr;
VSTGUI::ParameterChangeListener* paramChangeListener = nullptr;

// void changeBackgroundImage(float switchVal)
// {
// 	if (!editor || !editor->getFrame()) return;
	
// 	// Choose image based on switch state
// 	const char* imageName = (switchVal <= 0.5f) ? "resource/shell/shell1_765x604.png" : "resource/shell/shell2_765x604.png";
	
// 	// Create bitmap from image file
// 	VSTGUI::CBitmap* bitmap = new VSTGUI::CBitmap(imageName);
	
// 	// if (bitmap && bitmap->isLoaded())
// 	// {
// 	// 	// Set the background on the main frame
// 	// 	editor->getFrame()->setBackground(bitmap);
		
// 	// 	// Force redraw
// 	// 	editor->getFrame()->invalid();
// 	// }
	
// 	// Release bitmap reference
// 	if (bitmap) bitmap->forget();
// }	
};
}