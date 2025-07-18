//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#include "view.h"

#include "vstgui/vstgui.h"

//#include <atomic>




using namespace VSTGUI;

View::View(const CRect& size)
: CViewContainer(size)

{

    background = VSTGUI::SharedPointer<VSTGUI::CBitmap>(new VSTGUI::CBitmap("VOID_REND_BASE_c1.png"));
    backgroundAlt = VSTGUI::SharedPointer<VSTGUI::CBitmap>(new VSTGUI::CBitmap("VOID_REND_ALT_c1.png"));

    auto knobBitmap = new CBitmap("Vintage_amp_knob_2.png"); // This is your knob strip (background)
    auto switchBitmap = new CBitmap("EyeButton.png");

    CRect knobRect(250, 250, 350, 350);
    CRect switchRect(200, 200, 300, 300);

    // Since CKnob requires both background and handle bitmaps, 
    // and you only have one bitmap, pass nullptr for handle.
    myKnob = new CKnob(knobRect, nullptr, 10, knobBitmap, nullptr);
    mySwitch = new COnOffButton(switchRect, this, 13, switchBitmap);
    CViewContainer::addView(mySwitch);
    CViewContainer::addView(myKnob);
}

void View::draw(CDrawContext* pContext)
{
    if (switchState == 1.0f && background)
        background->draw(pContext, getViewSize());
    else if (switchState == 0.0f && backgroundAlt)
        backgroundAlt->draw(pContext, getViewSize());
    else if (background) // fallback
        background->draw(pContext, getViewSize());

    CViewContainer::draw(pContext);
    setDirty(false);
}

void View::valueChanged(CControl* pControl)
{
    if (!pControl) return;

    // Check the parameter tag
    switch (pControl->getTag())
    {
        case 13: // <-- check for switch 
        {
            float newState = pControl->getValue();
            View::setSwitchState(newState);
            break;
        }
        default:
            break;
    }
}

void View::setSwitchState(float state)
{
    if (switchState != state)
    {
        switchState = state;
        invalid(); 
    }
}