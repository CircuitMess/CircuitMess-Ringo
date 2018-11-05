// Don't modify, generated file from C:/aseprite/data/widgets/frame_tag_properties.xml

#ifndef GENERATED_FRAME_TAG_PROPERTIES_H_INCLUDED
#define GENERATED_FRAME_TAG_PROPERTIES_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "app/ui/color_button.h"
#include "ui/button.h"
#include "ui/combobox.h"
#include "ui/entry.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class FrameTagProperties : public ui::Window {
  public:
    FrameTagProperties() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("frame_tag_properties.xml", "frame_tag_properties", this);
      app::finder(this)
        >> "name" >> m_name
        >> "from" >> m_from
        >> "to" >> m_to
        >> "color" >> m_color
        >> "anidir" >> m_anidir
        >> "ok" >> m_ok
      ;
    }

    ui::Entry* name() const { return m_name; }
    ui::Entry* from() const { return m_from; }
    ui::Entry* to() const { return m_to; }
    app::ColorButton* color() const { return m_color; }
    ui::ComboBox* anidir() const { return m_anidir; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::Entry* m_name;
    ui::Entry* m_from;
    ui::Entry* m_to;
    app::ColorButton* m_color;
    ui::ComboBox* m_anidir;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
