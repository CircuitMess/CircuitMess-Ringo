// Don't modify, generated file from C:/aseprite/data/widgets/cel_properties.xml

#ifndef GENERATED_CEL_PROPERTIES_H_INCLUDED
#define GENERATED_CEL_PROPERTIES_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/slider.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class CelProperties : public ui::Window {
  public:
    CelProperties() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("cel_properties.xml", "cel_properties", this);
      app::finder(this)
        >> "opacity" >> m_opacity
        >> "user_data" >> m_userData
      ;
    }

    ui::Slider* opacity() const { return m_opacity; }
    ui::Button* userData() const { return m_userData; }

  private:
    ui::Slider* m_opacity;
    ui::Button* m_userData;
  };

} // namespace gen
} // namespace app

#endif
