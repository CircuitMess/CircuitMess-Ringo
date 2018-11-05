// Don't modify, generated file from C:/aseprite/data/widgets/palette_popup.xml

#ifndef GENERATED_PALETTE_POPUP_H_INCLUDED
#define GENERATED_PALETTE_POPUP_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "app/ui/search_entry.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/view.h"

namespace app {
namespace gen {

  class PalettePopup : public ui::VBox {
  public:
    PalettePopup() {
      app::load_widget("palette_popup.xml", "palette_popup", this);
      app::finder(this)
        >> "search" >> m_search
        >> "view" >> m_view
        >> "load_pal" >> m_loadPal
        >> "open_folder" >> m_openFolder
      ;
    }

    app::SearchEntry* search() const { return m_search; }
    ui::View* view() const { return m_view; }
    ui::Button* loadPal() const { return m_loadPal; }
    ui::Button* openFolder() const { return m_openFolder; }

  private:
    app::SearchEntry* m_search;
    ui::View* m_view;
    ui::Button* m_loadPal;
    ui::Button* m_openFolder;
  };

} // namespace gen
} // namespace app

#endif
