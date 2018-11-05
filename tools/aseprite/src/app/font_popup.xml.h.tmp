// Don't modify, generated file from C:/aseprite/data/widgets/font_popup.xml

#ifndef GENERATED_FONT_POPUP_H_INCLUDED
#define GENERATED_FONT_POPUP_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "app/ui/search_entry.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/view.h"

namespace app {
namespace gen {

  class FontPopup : public ui::VBox {
  public:
    FontPopup() {
      app::load_widget("font_popup.xml", "font_popup", this);
      app::finder(this)
        >> "search" >> m_search
        >> "view" >> m_view
        >> "load_font" >> m_loadFont
      ;
    }

    app::SearchEntry* search() const { return m_search; }
    ui::View* view() const { return m_view; }
    ui::Button* loadFont() const { return m_loadFont; }

  private:
    app::SearchEntry* m_search;
    ui::View* m_view;
    ui::Button* m_loadFont;
  };

} // namespace gen
} // namespace app

#endif
