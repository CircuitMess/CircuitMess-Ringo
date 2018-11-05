// Don't modify, generated file from C:/aseprite/data/widgets/webp_options.xml

#ifndef GENERATED_WEBP_OPTIONS_H_INCLUDED
#define GENERATED_WEBP_OPTIONS_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/combobox.h"
#include "ui/slider.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class WebpOptions : public ui::Window {
  public:
    WebpOptions() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("webp_options.xml", "webp_options", this);
      app::finder(this)
        >> "loop" >> m_loop
        >> "type" >> m_type
        >> "lossless_options" >> m_losslessOptions
        >> "compression" >> m_compression
        >> "image_hint" >> m_imageHint
        >> "lossy_options" >> m_lossyOptions
        >> "quality" >> m_quality
        >> "image_preset" >> m_imagePreset
        >> "ok" >> m_ok
      ;
    }

    ui::CheckBox* loop() const { return m_loop; }
    ui::ComboBox* type() const { return m_type; }
    ui::VBox* losslessOptions() const { return m_losslessOptions; }
    ui::Slider* compression() const { return m_compression; }
    ui::ComboBox* imageHint() const { return m_imageHint; }
    ui::VBox* lossyOptions() const { return m_lossyOptions; }
    ui::Slider* quality() const { return m_quality; }
    ui::ComboBox* imagePreset() const { return m_imagePreset; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::CheckBox* m_loop;
    ui::ComboBox* m_type;
    ui::VBox* m_losslessOptions;
    ui::Slider* m_compression;
    ui::ComboBox* m_imageHint;
    ui::VBox* m_lossyOptions;
    ui::Slider* m_quality;
    ui::ComboBox* m_imagePreset;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
