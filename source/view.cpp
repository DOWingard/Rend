//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#include "view.h"

#include "vstgui/vstgui.h"

#include <atomic>




View::View (const VSTGUI::CRect& size) : VSTGUI::CView (size) {}

void View::draw (VSTGUI::CDrawContext* pContext) {
    // rectangular background
    pContext -> setFillColor(VSTGUI::CColor(100,100,255,255)); // blue?
    // draw the rectangle
    pContext -> drawRect (getViewSize(), VSTGUI::kDrawFilled);

    setDirty(false);
}
