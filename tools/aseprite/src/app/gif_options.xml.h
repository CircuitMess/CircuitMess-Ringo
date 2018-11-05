// Don't modify, generated file from C:/aseprite/data/widgets/gif_options.xml

#ifndef GENERATED_GIF_OPTIONS_H_INCLUDED
#define GENERATED_GIF_OPTIONS_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class GifOptions : public ui::Window {
  public:
    GifOptions() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("gif_options.xml", "gif_options", this);
      app::finder(this)
        >> "interlaced" >> m_interlaced
        >> "loop" >> m_loop
        >> "dont_show" >> m_dontShow
        >> "ok" >> m_ok
      ;
    }

    ui::CheckBox* interlaced() const { return m_interlaced; }
    ui::CheckBox* loop() const { return m_loop; }
    ui::CheckBox* dontShow() const { return m_dontShow; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::CheckBox* m_interlaced;
    ui::CheckBox* m_loop;
    ui::CheckBox* m_dontShow;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
