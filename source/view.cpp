//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#include "view.h"

#include "vstgui/vstgui.h"
#include "vstgui\\lib\\controls\\cmoviebitmap.h"


//#include <atomic>




using namespace VSTGUI;

View::View(const CRect& size)
: CViewContainer(size)
{


}

void View::draw(CDrawContext* pContext)
{
    background->draw(pContext, getViewSize());

    CViewContainer::draw(pContext);
    setDirty(false);
}



