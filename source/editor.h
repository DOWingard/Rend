//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include "vstgui\plugin-bindings\vst3editor.h"
#include "editordelegate.h"

#include <memory>



namespace VOID {

class SwellEditor : public VSTGUI::VST3Editor
{
public:
    SwellEditor(Steinberg::Vst::EditController* controller)
    : VST3Editor(controller, "view", "editor.uidesc"),
      delegate_(std::make_unique<SwellEditorDelegate>())
    {
        auto* delegate = new SwellEditorDelegate();
        setDelegate(delegate); 
    }

    VSTGUI::IVST3EditorDelegate* SwellEditor::getDelegate() const
{
    return delegate_.get();
}

protected:
    VSTGUI::VST3Editor* editor = nullptr;


private:
    std::unique_ptr<SwellEditorDelegate> delegate_;
    VSTGUI::CBitmap* background1 = nullptr;
    VSTGUI::CBitmap* background2 = nullptr;
};



}