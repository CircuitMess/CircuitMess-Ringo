// Don't modify, generated file from C:/aseprite/data/widgets/sprite_size.xml

#ifndef GENERATED_SPRITE_SIZE_H_INCLUDED
#define GENERATED_SPRITE_SIZE_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/combobox.h"
#include "ui/entry.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class SpriteSize : public ui::Window {
  public:
    SpriteSize() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("sprite_size.xml", "sprite_size", this);
      app::finder(this)
        >> "width_px" >> m_widthPx
        >> "height_px" >> m_heightPx
        >> "lock_ratio" >> m_lockRatio
        >> "width_perc" >> m_widthPerc
        >> "height_perc" >> m_heightPerc
        >> "method" >> m_method
        >> "ok" >> m_ok
      ;
    }

    ui::Entry* widthPx() const { return m_widthPx; }
    ui::Entry* heightPx() const { return m_heightPx; }
    ui::CheckBox* lockRatio() const { return m_lockRatio; }
    ui::Entry* widthPerc() const { return m_widthPerc; }
    ui::Entry* heightPerc() const { return m_heightPerc; }
    ui::ComboBox* method() const { return m_method; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::Entry* m_widthPx;
    ui::Entry* m_heightPx;
    ui::CheckBox* m_lockRatio;
    ui::Entry* m_widthPerc;
    ui::Entry* m_heightPerc;
    ui::ComboBox* m_method;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
