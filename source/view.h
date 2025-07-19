//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include "vstgui/lib/cviewcontainer.h"
#include "vstgui/lib/cbitmap.h" 
#include "vstgui/lib/controls/ccontrol.h"



class View : public VSTGUI::CViewContainer {
public:
    View(const VSTGUI::CRect& size);

    void draw(VSTGUI::CDrawContext* pContext) override;
    void updateBackground(float buttonPressed); 
    float switchState = 1.0 ;

private:


    VSTGUI::SharedPointer<VSTGUI::CBitmap> background;  // background
    VSTGUI::SharedPointer<VSTGUI::CBitmap> backgroundAlt;
    VSTGUI::CKnob* myKnob          = nullptr;                    // Pointer to knob control
    VSTGUI::COnOffButton* mySwitch = nullptr; 
 
};