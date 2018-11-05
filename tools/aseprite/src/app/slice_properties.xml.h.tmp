// Don't modify, generated file from C:/aseprite/data/widgets/slice_properties.xml

#ifndef GENERATED_SLICE_PROPERTIES_H_INCLUDED
#define GENERATED_SLICE_PROPERTIES_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "ui/button.h"
#include "ui/entry.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class SliceProperties : public ui::Window {
  public:
    SliceProperties() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("slice_properties.xml", "slice_properties", this);
      app::finder(this)
        >> "name" >> m_name
        >> "user_data" >> m_userData
        >> "bounds_x" >> m_boundsX
        >> "bounds_y" >> m_boundsY
        >> "bounds_w" >> m_boundsW
        >> "bounds_h" >> m_boundsH
        >> "center" >> m_center
        >> "center_x" >> m_centerX
        >> "center_y" >> m_centerY
        >> "center_w" >> m_centerW
        >> "center_h" >> m_centerH
        >> "pivot" >> m_pivot
        >> "pivot_x" >> m_pivotX
        >> "pivot_y" >> m_pivotY
        >> "ok" >> m_ok
      ;
    }

    ui::Entry* name() const { return m_name; }
    ui::Button* userData() const { return m_userData; }
    ui::Entry* boundsX() const { return m_boundsX; }
    ui::Entry* boundsY() const { return m_boundsY; }
    ui::Entry* boundsW() const { return m_boundsW; }
    ui::Entry* boundsH() const { return m_boundsH; }
    ui::CheckBox* center() const { return m_center; }
    ui::Entry* centerX() const { return m_centerX; }
    ui::Entry* centerY() const { return m_centerY; }
    ui::Entry* centerW() const { return m_centerW; }
    ui::Entry* centerH() const { return m_centerH; }
    ui::CheckBox* pivot() const { return m_pivot; }
    ui::Entry* pivotX() const { return m_pivotX; }
    ui::Entry* pivotY() const { return m_pivotY; }
    ui::Button* ok() const { return m_ok; }

  private:
    ui::Entry* m_name;
    ui::Button* m_userData;
    ui::Entry* m_boundsX;
    ui::Entry* m_boundsY;
    ui::Entry* m_boundsW;
    ui::Entry* m_boundsH;
    ui::CheckBox* m_center;
    ui::Entry* m_centerX;
    ui::Entry* m_centerY;
    ui::Entry* m_centerW;
    ui::Entry* m_centerH;
    ui::CheckBox* m_pivot;
    ui::Entry* m_pivotX;
    ui::Entry* m_pivotY;
    ui::Button* m_ok;
  };

} // namespace gen
} // namespace app

#endif
