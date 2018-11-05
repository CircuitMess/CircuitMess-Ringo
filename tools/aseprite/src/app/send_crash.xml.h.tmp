// Don't modify, generated file from C:/aseprite/data/widgets/send_crash.xml

#ifndef GENERATED_SEND_CRASH_H_INCLUDED
#define GENERATED_SEND_CRASH_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/link_label.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class SendCrash : public ui::Window {
  public:
    SendCrash() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("send_crash.xml", "send_crash", this);
      app::finder(this)
        >> "official" >> m_official
        >> "filename" >> m_filename
        >> "dev" >> m_dev
        >> "dev_filename" >> m_devFilename
        >> "delete_file" >> m_deleteFile
      ;
    }

    ui::VBox* official() const { return m_official; }
    ui::LinkLabel* filename() const { return m_filename; }
    ui::VBox* dev() const { return m_dev; }
    ui::LinkLabel* devFilename() const { return m_devFilename; }
    ui::Button* deleteFile() const { return m_deleteFile; }

  private:
    ui::VBox* m_official;
    ui::LinkLabel* m_filename;
    ui::VBox* m_dev;
    ui::LinkLabel* m_devFilename;
    ui::Button* m_deleteFile;
  };

} // namespace gen
} // namespace app

#endif
