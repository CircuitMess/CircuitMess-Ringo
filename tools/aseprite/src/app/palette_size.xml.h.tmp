// Don't modify, generated file from C:/aseprite/data/widgets/palette_size.xml

#ifndef GENERATED_PALETTE_SIZE_H_INCLUDED
#define GENERATED_PALETTE_SIZE_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/entry.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class PaletteSize : public ui::Window {
  public:
    PaletteSize() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("palette_size.xml", "palette_size", this);
      app::finder(this)
        >> "colors" >> m_colors
        >> "ok" >> m_ok
      ;
    }

    ui::Entry* colors() const { return m_colors; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::Entry* m_colors;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
