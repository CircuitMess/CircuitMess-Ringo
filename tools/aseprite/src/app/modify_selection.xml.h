// Don't modify, generated file from C:/aseprite/data/widgets/modify_selection.xml

#ifndef GENERATED_MODIFY_SELECTION_H_INCLUDED
#define GENERATED_MODIFY_SELECTION_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/entry.h"
#include "ui/label.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class ModifySelection : public ui::Window {
  public:
    ModifySelection() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("modify_selection.xml", "modify_selection", this);
      app::finder(this)
        >> "by_label" >> m_byLabel
        >> "quantity" >> m_quantity
        >> "circle" >> m_circle
        >> "square" >> m_square
        >> "ok" >> m_ok
      ;
    }

    ui::Label* byLabel() const { return m_byLabel; }
    ui::Entry* quantity() const { return m_quantity; }
    ui::RadioButton* circle() const { return m_circle; }
    ui::RadioButton* square() const { return m_square; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::Label* m_byLabel;
    ui::Entry* m_quantity;
    ui::RadioButton* m_circle;
    ui::RadioButton* m_square;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
