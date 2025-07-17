//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include "vstgui/lib/cview.h"



class View : public VSTGUI::CView {
public:
    View(const VSTGUI::CRect& size) ;

    void draw(VSTGUI::CDrawContext* pContext) override;

};
