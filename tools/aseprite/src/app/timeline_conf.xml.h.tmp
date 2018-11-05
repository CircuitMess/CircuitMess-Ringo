// Don't modify, generated file from C:/aseprite/data/widgets/timeline_conf.xml

#ifndef GENERATED_TIMELINE_CONF_H_INCLUDED
#define GENERATED_TIMELINE_CONF_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "app/ui/button_set.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/entry.h"
#include "ui/grid.h"
#include "ui/separator.h"
#include "ui/slider.h"

namespace app {
namespace gen {

  class TimelineConf : public ui::VBox {
  public:
    TimelineConf() {
      app::load_widget("timeline_conf.xml", "timeline_conf", this);
      app::finder(this)
        >> "position" >> m_position
        >> "first_frame" >> m_firstFrame
        >> "thumb_enabled" >> m_thumbEnabled
        >> "thumb_h_separator" >> m_thumbHSeparator
        >> "thumb_box" >> m_thumbBox
        >> "zoom" >> m_zoom
        >> "thumb_overlay_enabled" >> m_thumbOverlayEnabled
        >> "thumb_overlay_size" >> m_thumbOverlaySize
        >> "merge" >> m_merge
        >> "tint" >> m_tint
        >> "reset_onionskin" >> m_resetOnionskin
        >> "opacity" >> m_opacity
        >> "opacity_step" >> m_opacityStep
        >> "loop_tag" >> m_loopTag
        >> "current_layer" >> m_currentLayer
        >> "behind" >> m_behind
        >> "infront" >> m_infront
      ;
    }

    app::ButtonSet* position() const { return m_position; }
    ui::Entry* firstFrame() const { return m_firstFrame; }
    ui::CheckBox* thumbEnabled() const { return m_thumbEnabled; }
    ui::Separator* thumbHSeparator() const { return m_thumbHSeparator; }
    ui::Grid* thumbBox() const { return m_thumbBox; }
    ui::Slider* zoom() const { return m_zoom; }
    ui::CheckBox* thumbOverlayEnabled() const { return m_thumbOverlayEnabled; }
    ui::Slider* thumbOverlaySize() const { return m_thumbOverlaySize; }
    ui::RadioButton* merge() const { return m_merge; }
    ui::RadioButton* tint() const { return m_tint; }
    ui::Button* resetOnionskin() const { return m_resetOnionskin; }
    ui::Slider* opacity() const { return m_opacity; }
    ui::Slider* opacityStep() const { return m_opacityStep; }
    ui::CheckBox* loopTag() const { return m_loopTag; }
    ui::CheckBox* currentLayer() const { return m_currentLayer; }
    ui::RadioButton* behind() const { return m_behind; }
    ui::RadioButton* infront() const { return m_infront; }

  private:
    app::ButtonSet* m_position;
    ui::Entry* m_firstFrame;
    ui::CheckBox* m_thumbEnabled;
    ui::Separator* m_thumbHSeparator;
    ui::Grid* m_thumbBox;
    ui::Slider* m_zoom;
    ui::CheckBox* m_thumbOverlayEnabled;
    ui::Slider* m_thumbOverlaySize;
    ui::RadioButton* m_merge;
    ui::RadioButton* m_tint;
    ui::Button* m_resetOnionskin;
    ui::Slider* m_opacity;
    ui::Slider* m_opacityStep;
    ui::CheckBox* m_loopTag;
    ui::CheckBox* m_currentLayer;
    ui::RadioButton* m_behind;
    ui::RadioButton* m_infront;
  };

} // namespace gen
} // namespace app

#endif
