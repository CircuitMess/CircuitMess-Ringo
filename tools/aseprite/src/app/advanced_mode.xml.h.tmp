// Don't modify, generated file from C:/aseprite/data/widgets/advanced_mode.xml

#ifndef GENERATED_ADVANCED_MODE_H_INCLUDED
#define GENERATED_ADVANCED_MODE_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/label.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class AdvancedMode : public ui::Window {
  public:
    AdvancedMode() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("advanced_mode.xml", "advanced_mode", this);
      app::finder(this)
        >> "warning_label" >> m_warningLabel
        >> "donot_show" >> m_donotShow
      ;
    }

    ui::Label* warningLabel() const { return m_warningLabel; }
    ui::CheckBox* donotShow() const { return m_donotShow; }

  private:
    ui::Label* m_warningLabel;
    ui::CheckBox* m_donotShow;
  };

} // namespace gen
} // namespace app

#endif
