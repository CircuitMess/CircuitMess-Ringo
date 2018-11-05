// Don't modify, generated file from C:/aseprite/data/widgets/sprite_properties.xml

#ifndef GENERATED_SPRITE_PROPERTIES_H_INCLUDED
#define GENERATED_SPRITE_PROPERTIES_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/combobox.h"
#include "ui/entry.h"
#include "ui/label.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class SpriteProperties : public ui::Window {
  public:
    SpriteProperties() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("sprite_properties.xml", "sprite_properties", this);
      app::finder(this)
        >> "name" >> m_name
        >> "type" >> m_type
        >> "size" >> m_size
        >> "frames" >> m_frames
        >> "transparent_color_placeholder" >> m_transparentColorPlaceholder
        >> "pixel_ratio" >> m_pixelRatio
        >> "ok" >> m_ok
        >> "cancel" >> m_cancel
      ;
    }

    ui::Entry* name() const { return m_name; }
    ui::Label* type() const { return m_type; }
    ui::Label* size() const { return m_size; }
    ui::Label* frames() const { return m_frames; }
    ui::HBox* transparentColorPlaceholder() const { return m_transparentColorPlaceholder; }
    ui::ComboBox* pixelRatio() const { return m_pixelRatio; }
    ui::Button* ok() const { return m_ok; }
    ui::Button* cancel() const { return m_cancel; }

  private:
    ui::Entry* m_name;
    ui::Label* m_type;
    ui::Label* m_size;
    ui::Label* m_frames;
    ui::HBox* m_transparentColorPlaceholder;
    ui::ComboBox* m_pixelRatio;
    ui::Button* m_ok;
    ui::Button* m_cancel;
  };

} // namespace gen
} // namespace app

#endif
