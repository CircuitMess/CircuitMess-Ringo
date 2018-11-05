// Don't modify, generated file from C:/aseprite/data/widgets/keyboard_shortcuts.xml

#ifndef GENERATED_KEYBOARD_SHORTCUTS_H_INCLUDED
#define GENERATED_KEYBOARD_SHORTCUTS_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "app/ui/search_entry.h"
#include "ui/button.h"
#include "ui/listbox.h"
#include "ui/view.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class KeyboardShortcuts : public ui::Window {
  public:
    KeyboardShortcuts() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("keyboard_shortcuts.xml", "keyboard_shortcuts", this);
      app::finder(this)
        >> "search" >> m_search
        >> "section" >> m_section
        >> "import_button" >> m_importButton
        >> "export_button" >> m_exportButton
        >> "reset_button" >> m_resetButton
        >> "search_view" >> m_searchView
        >> "search_list" >> m_searchList
        >> "menus_view" >> m_menusView
        >> "menus" >> m_menus
        >> "commands_view" >> m_commandsView
        >> "commands" >> m_commands
        >> "tools_view" >> m_toolsView
        >> "tools" >> m_tools
        >> "actions_view" >> m_actionsView
        >> "actions" >> m_actions
        >> "ok" >> m_ok
      ;
    }

    app::SearchEntry* search() const { return m_search; }
    ui::ListBox* section() const { return m_section; }
    ui::Button* importButton() const { return m_importButton; }
    ui::Button* exportButton() const { return m_exportButton; }
    ui::Button* resetButton() const { return m_resetButton; }
    ui::View* searchView() const { return m_searchView; }
    ui::ListBox* searchList() const { return m_searchList; }
    ui::View* menusView() const { return m_menusView; }
    ui::ListBox* menus() const { return m_menus; }
    ui::View* commandsView() const { return m_commandsView; }
    ui::ListBox* commands() const { return m_commands; }
    ui::View* toolsView() const { return m_toolsView; }
    ui::ListBox* tools() const { return m_tools; }
    ui::View* actionsView() const { return m_actionsView; }
    ui::ListBox* actions() const { return m_actions; }
    ui::Button* ok() const { return m_ok; }

  private:
    app::SearchEntry* m_search;
    ui::ListBox* m_section;
    ui::Button* m_importButton;
    ui::Button* m_exportButton;
    ui::Button* m_resetButton;
    ui::View* m_searchView;
    ui::ListBox* m_searchList;
    ui::View* m_menusView;
    ui::ListBox* m_menus;
    ui::View* m_commandsView;
    ui::ListBox* m_commands;
    ui::View* m_toolsView;
    ui::ListBox* m_tools;
    ui::View* m_actionsView;
    ui::ListBox* m_actions;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
