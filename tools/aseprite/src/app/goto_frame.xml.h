// Don't modify, generated file from C:/aseprite/data/widgets/goto_frame.xml

#ifndef GENERATED_GOTO_FRAME_H_INCLUDED
#define GENERATED_GOTO_FRAME_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class GotoFrame : public ui::Window {
  public:
    GotoFrame() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("goto_frame.xml", "goto_frame", this);
      app::finder(this)
        >> "frame_placeholder" >> m_framePlaceholder
        >> "ok" >> m_ok
      ;
    }

    ui::VBox* framePlaceholder() const { return m_framePlaceholder; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::VBox* m_framePlaceholder;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
