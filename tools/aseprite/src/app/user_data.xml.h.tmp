// Don't modify, generated file from C:/aseprite/data/widgets/user_data.xml

#ifndef GENERATED_USER_DATA_H_INCLUDED
#define GENERATED_USER_DATA_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "app/ui/color_button.h"
#include "ui/entry.h"
#include "ui/tooltips.h"

namespace app {
namespace gen {

  class UserData : public ui::TipWindow {
  public:
    UserData() {
      app::load_widget("user_data.xml", "user_data", this);
      app::finder(this)
        >> "text" >> m_text
        >> "color" >> m_color
      ;
    }

    ui::Entry* text() const { return m_text; }
    app::ColorButton* color() const { return m_color; }

  private:
    ui::Entry* m_text;
    app::ColorButton* m_color;
  };

} // namespace gen
} // namespace app

#endif
