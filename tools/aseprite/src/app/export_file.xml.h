// Don't modify, generated file from C:/aseprite/data/widgets/export_file.xml

#ifndef GENERATED_EXPORT_FILE_H_INCLUDED
#define GENERATED_EXPORT_FILE_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/combobox.h"
#include "ui/entry.h"
#include "ui/label.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class ExportFile : public ui::Window {
  public:
    ExportFile() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("export_file.xml", "export_file", this);
      app::finder(this)
        >> "output_filename" >> m_outputFilename
        >> "output_filename_browse" >> m_outputFilenameBrowse
        >> "resize_label" >> m_resizeLabel
        >> "resize" >> m_resize
        >> "layers_label" >> m_layersLabel
        >> "layers" >> m_layers
        >> "frames_label" >> m_framesLabel
        >> "frames" >> m_frames
        >> "anidir_label" >> m_anidirLabel
        >> "anidir" >> m_anidir
        >> "pixel_ratio" >> m_pixelRatio
        >> "for_twitter" >> m_forTwitter
        >> "adjust_resize" >> m_adjustResize
        >> "ok" >> m_ok
      ;
    }

    ui::Entry* outputFilename() const { return m_outputFilename; }
    ui::Button* outputFilenameBrowse() const { return m_outputFilenameBrowse; }
    ui::Label* resizeLabel() const { return m_resizeLabel; }
    ui::ComboBox* resize() const { return m_resize; }
    ui::Label* layersLabel() const { return m_layersLabel; }
    ui::ComboBox* layers() const { return m_layers; }
    ui::Label* framesLabel() const { return m_framesLabel; }
    ui::ComboBox* frames() const { return m_frames; }
    ui::Label* anidirLabel() const { return m_anidirLabel; }
    ui::ComboBox* anidir() const { return m_anidir; }
    ui::CheckBox* pixelRatio() const { return m_pixelRatio; }
    ui::CheckBox* forTwitter() const { return m_forTwitter; }
    ui::Button* adjustResize() const { return m_adjustResize; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::Entry* m_outputFilename;
    ui::Button* m_outputFilenameBrowse;
    ui::Label* m_resizeLabel;
    ui::ComboBox* m_resize;
    ui::Label* m_layersLabel;
    ui::ComboBox* m_layers;
    ui::Label* m_framesLabel;
    ui::ComboBox* m_frames;
    ui::Label* m_anidirLabel;
    ui::ComboBox* m_anidir;
    ui::CheckBox* m_pixelRatio;
    ui::CheckBox* m_forTwitter;
    ui::Button* m_adjustResize;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
