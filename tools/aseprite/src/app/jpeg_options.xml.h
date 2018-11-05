// Don't modify, generated file from C:/aseprite/data/widgets/jpeg_options.xml

#ifndef GENERATED_JPEG_OPTIONS_H_INCLUDED
#define GENERATED_JPEG_OPTIONS_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/slider.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class JpegOptions : public ui::Window {
  public:
    JpegOptions() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("jpeg_options.xml", "jpeg_options", this);
      app::finder(this)
        >> "quality" >> m_quality
        >> "dont_show" >> m_dontShow
        >> "ok" >> m_ok
      ;
    }

    ui::Slider* quality() const { return m_quality; }
    ui::CheckBox* dontShow() const { return m_dontShow; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::Slider* m_quality;
    ui::CheckBox* m_dontShow;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
