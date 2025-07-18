//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include "vstgui/lib/cviewcontainer.h"
#include "vstgui/lib/cbitmap.h" 
#include "vstgui/lib/controls/ccontrol.h"
#include "vstgui\lib\controls\icontrollistener.h"


class View : public VSTGUI::CViewContainer, public VSTGUI::IControlListener {
public:
    View(const VSTGUI::CRect& size);

    void draw(VSTGUI::CDrawContext* pContext) override;

    void valueChanged(VSTGUI::CControl* pControl) override;// <- listens to parameter changes
    void setSwitchState(float state);

private:

    float switchState = 1.0 ;
    VSTGUI::SharedPointer<VSTGUI::CBitmap> background;  // background
    VSTGUI::SharedPointer<VSTGUI::CBitmap> backgroundAlt;
    VSTGUI::CKnob* myKnob          = nullptr;                    // Pointer to knob control
    VSTGUI::COnOffButton* mySwitch = nullptr;   
};