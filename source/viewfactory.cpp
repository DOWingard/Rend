#pragma once

#include "vstgui/vstgui.h"
#include "vstgui/vstgui_uidescription.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"

#include "view.h"

using namespace VSTGUI;

class viewFactory : public ViewCreatorAdapter {
    public:
        // controller class
        viewFactory () { UIViewFactory::registerViewCreator (*this); }
        
        // return a unique name
        IdStringPtr getViewName () const override {return "REND UI";}

        // return the name here from where your custom view interits
        // your view automatically supports the attributes from it
        IdStringPtr getBaseViewName () const override {return UIViewCreator::kCView; }

        // create view here
        CView* create (const UIAttributes& attributes, const IUIDescription* description) const override {
            // create your custom view
            //CRect size (CPoint (45,45), CPoint (400,150));
            //return new View(size);
            return new View(CRect(0,0,640,413));
        }
};
static viewFactory __gFactory;

