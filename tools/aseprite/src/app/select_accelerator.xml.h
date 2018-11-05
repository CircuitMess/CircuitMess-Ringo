// Don't modify, generated file from C:/aseprite/data/widgets/select_accelerator.xml

#ifndef GENERATED_SELECT_ACCELERATOR_H_INCLUDED
#define GENERATED_SELECT_ACCELERATOR_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/label.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class SelectAccelerator : public ui::Window {
  public:
    SelectAccelerator() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("select_accelerator.xml", "select_accelerator", this);
      app::finder(this)
        >> "key_placeholder" >> m_keyPlaceholder
        >> "clear_button" >> m_clearButton
        >> "ctrl" >> m_ctrl
        >> "cmd" >> m_cmd
        >> "alt" >> m_alt
        >> "shift" >> m_shift
        >> "space" >> m_space
        >> "win" >> m_win
        >> "assigned_to" >> m_assignedTo
        >> "ok_button" >> m_okButton
        >> "cancel_button" >> m_cancelButton
      ;
    }

    ui::HBox* keyPlaceholder() const { return m_keyPlaceholder; }
    ui::Button* clearButton() const { return m_clearButton; }
    ui::CheckBox* ctrl() const { return m_ctrl; }
    ui::CheckBox* cmd() const { return m_cmd; }
    ui::CheckBox* alt() const { return m_alt; }
    ui::CheckBox* shift() const { return m_shift; }
    ui::CheckBox* space() const { return m_space; }
    ui::CheckBox* win() const { return m_win; }
    ui::Label* assignedTo() const { return m_assignedTo; }
    ui::Button* okButton() const { return m_okButton; }
    ui::Button* cancelButton() const { return m_cancelButton; }

  private:
    ui::HBox* m_keyPlaceholder;
    ui::Button* m_clearButton;
    ui::CheckBox* m_ctrl;
    ui::CheckBox* m_cmd;
    ui::CheckBox* m_alt;
    ui::CheckBox* m_shift;
    ui::CheckBox* m_space;
    ui::CheckBox* m_win;
    ui::Label* m_assignedTo;
    ui::Button* m_okButton;
    ui::Button* m_cancelButton;
  };

} // namespace gen
} // namespace app

#endif
