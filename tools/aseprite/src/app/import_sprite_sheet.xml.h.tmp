// Don't modify, generated file from C:/aseprite/data/widgets/import_sprite_sheet.xml

#ifndef GENERATED_IMPORT_SPRITE_SHEET_H_INCLUDED
#define GENERATED_IMPORT_SPRITE_SHEET_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/combobox.h"
#include "ui/entry.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class ImportSpriteSheet : public ui::Window {
  public:
    ImportSpriteSheet() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("import_sprite_sheet.xml", "import_sprite_sheet", this);
      app::finder(this)
        >> "select_file" >> m_selectFile
        >> "sheet_type" >> m_sheetType
        >> "x" >> m_x
        >> "y" >> m_y
        >> "width" >> m_width
        >> "height" >> m_height
        >> "partial_tiles" >> m_partialTiles
        >> "import" >> m_import
        >> "cancel" >> m_cancel
      ;
    }

    ui::Button* selectFile() const { return m_selectFile; }
    ui::ComboBox* sheetType() const { return m_sheetType; }
    ui::Entry* x() const { return m_x; }
    ui::Entry* y() const { return m_y; }
    ui::Entry* width() const { return m_width; }
    ui::Entry* height() const { return m_height; }
    ui::CheckBox* partialTiles() const { return m_partialTiles; }
    ui::Button* import() const { return m_import; }
    ui::Button* cancel() const { return m_cancel; }

  private:
    ui::Button* m_selectFile;
    ui::ComboBox* m_sheetType;
    ui::Entry* m_x;
    ui::Entry* m_y;
    ui::Entry* m_width;
    ui::Entry* m_height;
    ui::CheckBox* m_partialTiles;
    ui::Button* m_import;
    ui::Button* m_cancel;
  };

} // namespace gen
} // namespace app

#endif
