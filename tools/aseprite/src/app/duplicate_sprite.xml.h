// Don't modify, generated file from C:/aseprite/data/widgets/duplicate_sprite.xml

#ifndef GENERATED_DUPLICATE_SPRITE_H_INCLUDED
#define GENERATED_DUPLICATE_SPRITE_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/entry.h"
#include "ui/label.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class DuplicateSprite : public ui::Window {
  public:
    DuplicateSprite() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("duplicate_sprite.xml", "duplicate_sprite", this);
      app::finder(this)
        >> "src_name" >> m_srcName
        >> "dst_name" >> m_dstName
        >> "flatten" >> m_flatten
        >> "ok" >> m_ok
      ;
    }

    ui::Label* srcName() const { return m_srcName; }
    ui::Entry* dstName() const { return m_dstName; }
    ui::CheckBox* flatten() const { return m_flatten; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::Label* m_srcName;
    ui::Entry* m_dstName;
    ui::CheckBox* m_flatten;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
