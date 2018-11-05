// Don't modify, generated file from C:/aseprite/data/widgets/color_mode.xml

#ifndef GENERATED_COLOR_MODE_H_INCLUDED
#define GENERATED_COLOR_MODE_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/listbox.h"
#include "ui/slider.h"
#include "ui/view.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class ColorMode : public ui::Window {
  public:
    ColorMode() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("color_mode.xml", "color_mode", this);
      app::finder(this)
        >> "color_mode_view" >> m_colorModeView
        >> "color_mode" >> m_colorMode
        >> "progress" >> m_progress
        >> "dithering_placeholder" >> m_ditheringPlaceholder
        >> "flatten" >> m_flatten
        >> "ok" >> m_ok
      ;
    }

    ui::View* colorModeView() const { return m_colorModeView; }
    ui::ListBox* colorMode() const { return m_colorMode; }
    ui::Slider* progress() const { return m_progress; }
    ui::HBox* ditheringPlaceholder() const { return m_ditheringPlaceholder; }
    ui::CheckBox* flatten() const { return m_flatten; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::View* m_colorModeView;
    ui::ListBox* m_colorMode;
    ui::Slider* m_progress;
    ui::HBox* m_ditheringPlaceholder;
    ui::CheckBox* m_flatten;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
