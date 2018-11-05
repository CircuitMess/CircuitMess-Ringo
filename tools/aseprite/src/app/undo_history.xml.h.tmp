// Don't modify, generated file from C:/aseprite/data/widgets/undo_history.xml

#ifndef GENERATED_UNDO_HISTORY_H_INCLUDED
#define GENERATED_UNDO_HISTORY_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/listbox.h"
#include "ui/view.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class UndoHistory : public ui::Window {
  public:
    UndoHistory() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("undo_history.xml", "undo_history", this);
      app::finder(this)
        >> "view" >> m_view
        >> "actions" >> m_actions
      ;
    }

    ui::View* view() const { return m_view; }
    ui::ListBox* actions() const { return m_actions; }

  private:
    ui::View* m_view;
    ui::ListBox* m_actions;
  };

} // namespace gen
} // namespace app

#endif
