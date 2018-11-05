// Don't modify, generated file from C:/aseprite/data/widgets/new_folder_window.xml

#ifndef GENERATED_NEW_FOLDER_WINDOW_H_INCLUDED
#define GENERATED_NEW_FOLDER_WINDOW_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/entry.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class NewFolderWindow : public ui::Window {
  public:
    NewFolderWindow() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("new_folder_window.xml", "new_folder_window", this);
      app::finder(this)
        >> "name" >> m_name
        >> "ok" >> m_ok
        >> "cancel" >> m_cancel
      ;
    }

    ui::Entry* name() const { return m_name; }
    ui::Button* ok() const { return m_ok; }
    ui::Button* cancel() const { return m_cancel; }

  private:
    ui::Entry* m_name;
    ui::Button* m_ok;
    ui::Button* m_cancel;
  };

} // namespace gen
} // namespace app

#endif
