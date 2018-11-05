// Don't modify, generated file from C:/aseprite/data/widgets/layer_properties.xml

#ifndef GENERATED_LAYER_PROPERTIES_H_INCLUDED
#define GENERATED_LAYER_PROPERTIES_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/combobox.h"
#include "ui/entry.h"
#include "ui/slider.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class LayerProperties : public ui::Window {
  public:
    LayerProperties() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("layer_properties.xml", "layer_properties", this);
      app::finder(this)
        >> "name" >> m_name
        >> "user_data" >> m_userData
        >> "mode" >> m_mode
        >> "opacity" >> m_opacity
      ;
    }

    ui::Entry* name() const { return m_name; }
    ui::Button* userData() const { return m_userData; }
    ui::ComboBox* mode() const { return m_mode; }
    ui::Slider* opacity() const { return m_opacity; }

  private:
    ui::Entry* m_name;
    ui::Button* m_userData;
    ui::ComboBox* m_mode;
    ui::Slider* m_opacity;
  };

} // namespace gen
} // namespace app

#endif
