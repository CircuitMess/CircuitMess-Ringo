// Don't modify, generated file from C:/aseprite/data/widgets/main_window.xml

#ifndef GENERATED_MAIN_WINDOW_H_INCLUDED
#define GENERATED_MAIN_WINDOW_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/box.h"
#include "ui/splitter.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class MainWindow : public ui::Window {
  public:
    MainWindow() : ui::Window(ui::Window::DesktopWindow) {
      app::load_widget("main_window.xml", "main_window", this);
      app::finder(this)
        >> "menu_bar_placeholder" >> m_menuBarPlaceholder
        >> "tabs_placeholder" >> m_tabsPlaceholder
        >> "color_bar_splitter" >> m_colorBarSplitter
        >> "color_bar_placeholder" >> m_colorBarPlaceholder
        >> "context_bar_placeholder" >> m_contextBarPlaceholder
        >> "timeline_splitter" >> m_timelineSplitter
        >> "workspace_placeholder" >> m_workspacePlaceholder
        >> "timeline_placeholder" >> m_timelinePlaceholder
        >> "tool_bar_placeholder" >> m_toolBarPlaceholder
        >> "status_bar_placeholder" >> m_statusBarPlaceholder
      ;
    }

    ui::HBox* menuBarPlaceholder() const { return m_menuBarPlaceholder; }
    ui::HBox* tabsPlaceholder() const { return m_tabsPlaceholder; }
    ui::Splitter* colorBarSplitter() const { return m_colorBarSplitter; }
    ui::VBox* colorBarPlaceholder() const { return m_colorBarPlaceholder; }
    ui::VBox* contextBarPlaceholder() const { return m_contextBarPlaceholder; }
    ui::Splitter* timelineSplitter() const { return m_timelineSplitter; }
    ui::HBox* workspacePlaceholder() const { return m_workspacePlaceholder; }
    ui::VBox* timelinePlaceholder() const { return m_timelinePlaceholder; }
    ui::VBox* toolBarPlaceholder() const { return m_toolBarPlaceholder; }
    ui::HBox* statusBarPlaceholder() const { return m_statusBarPlaceholder; }

  private:
    ui::HBox* m_menuBarPlaceholder;
    ui::HBox* m_tabsPlaceholder;
    ui::Splitter* m_colorBarSplitter;
    ui::VBox* m_colorBarPlaceholder;
    ui::VBox* m_contextBarPlaceholder;
    ui::Splitter* m_timelineSplitter;
    ui::HBox* m_workspacePlaceholder;
    ui::VBox* m_timelinePlaceholder;
    ui::VBox* m_toolBarPlaceholder;
    ui::HBox* m_statusBarPlaceholder;
  };

} // namespace gen
} // namespace app

#endif
