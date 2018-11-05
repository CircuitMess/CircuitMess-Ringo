// Don't modify, generated file from C:/aseprite/data/widgets/file_selector.xml

#ifndef GENERATED_FILE_SELECTOR_H_INCLUDED
#define GENERATED_FILE_SELECTOR_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/combobox.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class FileSelector : public ui::Window {
  public:
    FileSelector() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("file_selector.xml", "file_selector", this);
      app::finder(this)
        >> "main" >> m_main
        >> "go_back_button" >> m_goBackButton
        >> "go_forward_button" >> m_goForwardButton
        >> "go_up_button" >> m_goUpButton
        >> "new_folder_button" >> m_newFolderButton
        >> "location" >> m_location
        >> "file_view_placeholder" >> m_fileViewPlaceholder
        >> "file_name_placeholder" >> m_fileNamePlaceholder
        >> "file_type" >> m_fileType
        >> "ok" >> m_ok
        >> "cancel" >> m_cancel
      ;
    }

    ui::VBox* main() const { return m_main; }
    ui::Button* goBackButton() const { return m_goBackButton; }
    ui::Button* goForwardButton() const { return m_goForwardButton; }
    ui::Button* goUpButton() const { return m_goUpButton; }
    ui::Button* newFolderButton() const { return m_newFolderButton; }
    ui::ComboBox* location() const { return m_location; }
    ui::VBox* fileViewPlaceholder() const { return m_fileViewPlaceholder; }
    ui::Box* fileNamePlaceholder() const { return m_fileNamePlaceholder; }
    ui::ComboBox* fileType() const { return m_fileType; }
    ui::Button* ok() const { return m_ok; }
    ui::Button* cancel() const { return m_cancel; }

  private:
    ui::VBox* m_main;
    ui::Button* m_goBackButton;
    ui::Button* m_goForwardButton;
    ui::Button* m_goUpButton;
    ui::Button* m_newFolderButton;
    ui::ComboBox* m_location;
    ui::VBox* m_fileViewPlaceholder;
    ui::Box* m_fileNamePlaceholder;
    ui::ComboBox* m_fileType;
    ui::Button* m_ok;
    ui::Button* m_cancel;
  };

} // namespace gen
} // namespace app

#endif
