// Don't modify, generated file from C:/aseprite/data/widgets/canvas_size.xml

#ifndef GENERATED_CANVAS_SIZE_H_INCLUDED
#define GENERATED_CANVAS_SIZE_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "app/ui/button_set.h"
#include "ui/button.h"
#include "ui/entry.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class CanvasSize : public ui::Window {
  public:
    CanvasSize() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("canvas_size.xml", "canvas_size", this);
      app::finder(this)
        >> "width" >> m_width
        >> "height" >> m_height
        >> "dir" >> m_dir
        >> "left" >> m_left
        >> "top" >> m_top
        >> "right" >> m_right
        >> "bottom" >> m_bottom
        >> "ok" >> m_ok
      ;
    }

    ui::Entry* width() const { return m_width; }
    ui::Entry* height() const { return m_height; }
    app::ButtonSet* dir() const { return m_dir; }
    ui::Entry* left() const { return m_left; }
    ui::Entry* top() const { return m_top; }
    ui::Entry* right() const { return m_right; }
    ui::Entry* bottom() const { return m_bottom; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::Entry* m_width;
    ui::Entry* m_height;
    app::ButtonSet* m_dir;
    ui::Entry* m_left;
    ui::Entry* m_top;
    ui::Entry* m_right;
    ui::Entry* m_bottom;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
