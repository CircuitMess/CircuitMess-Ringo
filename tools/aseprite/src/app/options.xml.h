// Don't modify, generated file from C:/aseprite/data/widgets/options.xml

#ifndef GENERATED_OPTIONS_H_INCLUDED
#define GENERATED_OPTIONS_H_INCLUDED
#pragma once

#include "app/find_widget.h"
#include "app/load_widget.h"
#include "app/ui/color_button.h"
#include "app/ui/pref_widget.h"
#include "ui/box.h"
#include "ui/button.h"
#include "ui/combobox.h"
#include "ui/entry.h"
#include "ui/label.h"
#include "ui/link_label.h"
#include "ui/listbox.h"
#include "ui/panel.h"
#include "ui/slider.h"
#include "ui/window.h"

namespace app {
namespace gen {

  class Options : public ui::Window {
  public:
    Options() : ui::Window(ui::Window::WithTitleBar) {
      app::load_widget("options.xml", "options", this);
      app::finder(this)
        >> "section_listbox" >> m_sectionListbox
        >> "panel" >> m_panel
        >> "section_general" >> m_sectionGeneral
        >> "screen_scale" >> m_screenScale
        >> "ui_scale" >> m_uiScale
        >> "language" >> m_language
        >> "gpu_acceleration" >> m_gpuAcceleration
        >> "show_menu_bar" >> m_showMenuBar
        >> "show_home" >> m_showHome
        >> "expand_menubar_on_mouseover" >> m_expandMenubarOnMouseover
        >> "enable_data_recovery" >> m_enableDataRecovery
        >> "data_recovery_period" >> m_dataRecoveryPeriod
        >> "locate_file" >> m_locateFile
        >> "locate_crash_folder" >> m_locateCrashFolder
        >> "section_files" >> m_sectionFiles
        >> "default_extension" >> m_defaultExtension
        >> "export_image_default_extension" >> m_exportImageDefaultExtension
        >> "export_animation_default_extension" >> m_exportAnimationDefaultExtension
        >> "export_sprite_sheet_default_extension" >> m_exportSpriteSheetDefaultExtension
        >> "recent_files" >> m_recentFiles
        >> "clear_recent_files" >> m_clearRecentFiles
        >> "show_full_path" >> m_showFullPath
        >> "section_editor" >> m_sectionEditor
        >> "wheel_zoom" >> m_wheelZoom
        >> "slide_zoom" >> m_slideZoom
        >> "zoom_from_center_with_wheel" >> m_zoomFromCenterWithWheel
        >> "zoom_from_center_with_keys" >> m_zoomFromCenterWithKeys
        >> "show_scrollbars" >> m_showScrollbars
        >> "auto_scroll" >> m_autoScroll
        >> "straight_line_preview" >> m_straightLinePreview
        >> "discard_brush" >> m_discardBrush
        >> "right_click_behavior" >> m_rightClickBehavior
        >> "section_selection" >> m_sectionSelection
        >> "auto_opaque" >> m_autoOpaque
        >> "keep_selection_after_clear" >> m_keepSelectionAfterClear
        >> "auto_show_selection_edges" >> m_autoShowSelectionEdges
        >> "move_edges" >> m_moveEdges
        >> "modifiers_disable_handles" >> m_modifiersDisableHandles
        >> "move_on_add_mode" >> m_moveOnAddMode
        >> "section_timeline" >> m_sectionTimeline
        >> "autotimeline" >> m_autotimeline
        >> "rewind_on_stop" >> m_rewindOnStop
        >> "first_frame" >> m_firstFrame
        >> "section_cursors" >> m_sectionCursors
        >> "native_cursor" >> m_nativeCursor
        >> "cursor_scale_label" >> m_cursorScaleLabel
        >> "cursor_scale" >> m_cursorScale
        >> "painting_cursor_type" >> m_paintingCursorType
        >> "brush_preview" >> m_brushPreview
        >> "cursor_color_type" >> m_cursorColorType
        >> "cursor_color" >> m_cursorColor
        >> "section_bg" >> m_sectionBg
        >> "bg_scope" >> m_bgScope
        >> "checked_bg_size" >> m_checkedBgSize
        >> "checked_bg_zoom" >> m_checkedBgZoom
        >> "checked_bg_color1" >> m_checkedBgColor1
        >> "checked_bg_color2" >> m_checkedBgColor2
        >> "reset_bg" >> m_resetBg
        >> "section_grid" >> m_sectionGrid
        >> "grid_scope" >> m_gridScope
        >> "grid_visible" >> m_gridVisible
        >> "grid_x" >> m_gridX
        >> "grid_y" >> m_gridY
        >> "grid_w" >> m_gridW
        >> "grid_h" >> m_gridH
        >> "grid_color" >> m_gridColor
        >> "grid_opacity" >> m_gridOpacity
        >> "grid_auto_opacity" >> m_gridAutoOpacity
        >> "pixel_grid_visible" >> m_pixelGridVisible
        >> "pixel_grid_color" >> m_pixelGridColor
        >> "pixel_grid_opacity" >> m_pixelGridOpacity
        >> "pixel_grid_auto_opacity" >> m_pixelGridAutoOpacity
        >> "reset_grid" >> m_resetGrid
        >> "section_guides_and_slices" >> m_sectionGuidesAndSlices
        >> "layer_edges_color" >> m_layerEdgesColor
        >> "auto_guides_color" >> m_autoGuidesColor
        >> "default_slice_color" >> m_defaultSliceColor
        >> "section_undo" >> m_sectionUndo
        >> "limit_undo" >> m_limitUndo
        >> "undo_size_limit" >> m_undoSizeLimit
        >> "undo_goto_modified" >> m_undoGotoModified
        >> "undo_allow_nonlinear_history" >> m_undoAllowNonlinearHistory
        >> "section_alerts" >> m_sectionAlerts
        >> "file_format_doesnt_support_alert" >> m_fileFormatDoesntSupportAlert
        >> "export_animation_in_sequence_alert" >> m_exportAnimationInSequenceAlert
        >> "overwrite_files_on_export_alert" >> m_overwriteFilesOnExportAlert
        >> "overwrite_files_on_export_sprite_sheet_alert" >> m_overwriteFilesOnExportSpriteSheetAlert
        >> "gif_options_alert" >> m_gifOptionsAlert
        >> "jpeg_options_alert" >> m_jpegOptionsAlert
        >> "advanced_mode_alert" >> m_advancedModeAlert
        >> "reset_alerts" >> m_resetAlerts
        >> "section_theme" >> m_sectionTheme
        >> "theme_list" >> m_themeList
        >> "select_theme" >> m_selectTheme
        >> "open_theme_folder" >> m_openThemeFolder
        >> "section_extensions" >> m_sectionExtensions
        >> "extensions_list" >> m_extensionsList
        >> "add_extension" >> m_addExtension
        >> "disable_extension" >> m_disableExtension
        >> "uninstall_extension" >> m_uninstallExtension
        >> "open_extension_folder" >> m_openExtensionFolder
        >> "section_experimental" >> m_sectionExperimental
        >> "native_clipboard" >> m_nativeClipboard
        >> "native_file_dialog" >> m_nativeFileDialog
        >> "one_finger_as_mouse_movement" >> m_oneFingerAsMouseMovement
        >> "load_wintab_driver_box" >> m_loadWintabDriverBox
        >> "load_wintab_driver" >> m_loadWintabDriver
        >> "flash_layer" >> m_flashLayer
        >> "nonactive_layers_opacity" >> m_nonactiveLayersOpacity
        >> "button_ok" >> m_buttonOk
        >> "button_apply" >> m_buttonApply
      ;
    }

    ui::ListBox* sectionListbox() const { return m_sectionListbox; }
    ui::Panel* panel() const { return m_panel; }
    ui::VBox* sectionGeneral() const { return m_sectionGeneral; }
    ui::ComboBox* screenScale() const { return m_screenScale; }
    ui::ComboBox* uiScale() const { return m_uiScale; }
    ui::ComboBox* language() const { return m_language; }
    ui::CheckBox* gpuAcceleration() const { return m_gpuAcceleration; }
    ui::CheckBox* showMenuBar() const { return m_showMenuBar; }
    ui::CheckBox* showHome() const { return m_showHome; }
    ui::CheckBox* expandMenubarOnMouseover() const { return m_expandMenubarOnMouseover; }
    ui::CheckBox* enableDataRecovery() const { return m_enableDataRecovery; }
    ui::ComboBox* dataRecoveryPeriod() const { return m_dataRecoveryPeriod; }
    ui::LinkLabel* locateFile() const { return m_locateFile; }
    ui::LinkLabel* locateCrashFolder() const { return m_locateCrashFolder; }
    ui::VBox* sectionFiles() const { return m_sectionFiles; }
    ui::ComboBox* defaultExtension() const { return m_defaultExtension; }
    ui::ComboBox* exportImageDefaultExtension() const { return m_exportImageDefaultExtension; }
    ui::ComboBox* exportAnimationDefaultExtension() const { return m_exportAnimationDefaultExtension; }
    ui::ComboBox* exportSpriteSheetDefaultExtension() const { return m_exportSpriteSheetDefaultExtension; }
    ui::Slider* recentFiles() const { return m_recentFiles; }
    ui::Button* clearRecentFiles() const { return m_clearRecentFiles; }
    ui::CheckBox* showFullPath() const { return m_showFullPath; }
    ui::VBox* sectionEditor() const { return m_sectionEditor; }
    ui::CheckBox* wheelZoom() const { return m_wheelZoom; }
    ui::CheckBox* slideZoom() const { return m_slideZoom; }
    ui::CheckBox* zoomFromCenterWithWheel() const { return m_zoomFromCenterWithWheel; }
    ui::CheckBox* zoomFromCenterWithKeys() const { return m_zoomFromCenterWithKeys; }
    ui::CheckBox* showScrollbars() const { return m_showScrollbars; }
    ui::CheckBox* autoScroll() const { return m_autoScroll; }
    ui::CheckBox* straightLinePreview() const { return m_straightLinePreview; }
    ui::CheckBox* discardBrush() const { return m_discardBrush; }
    ui::ComboBox* rightClickBehavior() const { return m_rightClickBehavior; }
    ui::VBox* sectionSelection() const { return m_sectionSelection; }
    ui::CheckBox* autoOpaque() const { return m_autoOpaque; }
    ui::CheckBox* keepSelectionAfterClear() const { return m_keepSelectionAfterClear; }
    ui::CheckBox* autoShowSelectionEdges() const { return m_autoShowSelectionEdges; }
    ui::CheckBox* moveEdges() const { return m_moveEdges; }
    ui::CheckBox* modifiersDisableHandles() const { return m_modifiersDisableHandles; }
    ui::CheckBox* moveOnAddMode() const { return m_moveOnAddMode; }
    ui::VBox* sectionTimeline() const { return m_sectionTimeline; }
    BoolPrefWidget<ui::CheckBox>* autotimeline() const { return m_autotimeline; }
    BoolPrefWidget<ui::CheckBox>* rewindOnStop() const { return m_rewindOnStop; }
    ui::Entry* firstFrame() const { return m_firstFrame; }
    ui::VBox* sectionCursors() const { return m_sectionCursors; }
    ui::CheckBox* nativeCursor() const { return m_nativeCursor; }
    ui::Label* cursorScaleLabel() const { return m_cursorScaleLabel; }
    ui::ComboBox* cursorScale() const { return m_cursorScale; }
    ui::ComboBox* paintingCursorType() const { return m_paintingCursorType; }
    ui::ComboBox* brushPreview() const { return m_brushPreview; }
    ui::ComboBox* cursorColorType() const { return m_cursorColorType; }
    app::ColorButton* cursorColor() const { return m_cursorColor; }
    ui::VBox* sectionBg() const { return m_sectionBg; }
    ui::ComboBox* bgScope() const { return m_bgScope; }
    ui::ComboBox* checkedBgSize() const { return m_checkedBgSize; }
    ui::CheckBox* checkedBgZoom() const { return m_checkedBgZoom; }
    app::ColorButton* checkedBgColor1() const { return m_checkedBgColor1; }
    app::ColorButton* checkedBgColor2() const { return m_checkedBgColor2; }
    ui::Button* resetBg() const { return m_resetBg; }
    ui::VBox* sectionGrid() const { return m_sectionGrid; }
    ui::ComboBox* gridScope() const { return m_gridScope; }
    ui::CheckBox* gridVisible() const { return m_gridVisible; }
    ui::Entry* gridX() const { return m_gridX; }
    ui::Entry* gridY() const { return m_gridY; }
    ui::Entry* gridW() const { return m_gridW; }
    ui::Entry* gridH() const { return m_gridH; }
    app::ColorButton* gridColor() const { return m_gridColor; }
    ui::Slider* gridOpacity() const { return m_gridOpacity; }
    ui::CheckBox* gridAutoOpacity() const { return m_gridAutoOpacity; }
    ui::CheckBox* pixelGridVisible() const { return m_pixelGridVisible; }
    app::ColorButton* pixelGridColor() const { return m_pixelGridColor; }
    ui::Slider* pixelGridOpacity() const { return m_pixelGridOpacity; }
    ui::CheckBox* pixelGridAutoOpacity() const { return m_pixelGridAutoOpacity; }
    ui::Button* resetGrid() const { return m_resetGrid; }
    ui::VBox* sectionGuidesAndSlices() const { return m_sectionGuidesAndSlices; }
    app::ColorButton* layerEdgesColor() const { return m_layerEdgesColor; }
    app::ColorButton* autoGuidesColor() const { return m_autoGuidesColor; }
    app::ColorButton* defaultSliceColor() const { return m_defaultSliceColor; }
    ui::VBox* sectionUndo() const { return m_sectionUndo; }
    ui::CheckBox* limitUndo() const { return m_limitUndo; }
    ui::Entry* undoSizeLimit() const { return m_undoSizeLimit; }
    ui::CheckBox* undoGotoModified() const { return m_undoGotoModified; }
    ui::CheckBox* undoAllowNonlinearHistory() const { return m_undoAllowNonlinearHistory; }
    ui::VBox* sectionAlerts() const { return m_sectionAlerts; }
    BoolPrefWidget<ui::CheckBox>* fileFormatDoesntSupportAlert() const { return m_fileFormatDoesntSupportAlert; }
    BoolPrefWidget<ui::CheckBox>* exportAnimationInSequenceAlert() const { return m_exportAnimationInSequenceAlert; }
    BoolPrefWidget<ui::CheckBox>* overwriteFilesOnExportAlert() const { return m_overwriteFilesOnExportAlert; }
    BoolPrefWidget<ui::CheckBox>* overwriteFilesOnExportSpriteSheetAlert() const { return m_overwriteFilesOnExportSpriteSheetAlert; }
    BoolPrefWidget<ui::CheckBox>* gifOptionsAlert() const { return m_gifOptionsAlert; }
    BoolPrefWidget<ui::CheckBox>* jpegOptionsAlert() const { return m_jpegOptionsAlert; }
    BoolPrefWidget<ui::CheckBox>* advancedModeAlert() const { return m_advancedModeAlert; }
    ui::Button* resetAlerts() const { return m_resetAlerts; }
    ui::VBox* sectionTheme() const { return m_sectionTheme; }
    ui::ListBox* themeList() const { return m_themeList; }
    ui::Button* selectTheme() const { return m_selectTheme; }
    ui::Button* openThemeFolder() const { return m_openThemeFolder; }
    ui::VBox* sectionExtensions() const { return m_sectionExtensions; }
    ui::ListBox* extensionsList() const { return m_extensionsList; }
    ui::Button* addExtension() const { return m_addExtension; }
    ui::Button* disableExtension() const { return m_disableExtension; }
    ui::Button* uninstallExtension() const { return m_uninstallExtension; }
    ui::Button* openExtensionFolder() const { return m_openExtensionFolder; }
    ui::VBox* sectionExperimental() const { return m_sectionExperimental; }
    ui::CheckBox* nativeClipboard() const { return m_nativeClipboard; }
    ui::CheckBox* nativeFileDialog() const { return m_nativeFileDialog; }
    BoolPrefWidget<ui::CheckBox>* oneFingerAsMouseMovement() const { return m_oneFingerAsMouseMovement; }
    ui::HBox* loadWintabDriverBox() const { return m_loadWintabDriverBox; }
    BoolPrefWidget<ui::CheckBox>* loadWintabDriver() const { return m_loadWintabDriver; }
    ui::CheckBox* flashLayer() const { return m_flashLayer; }
    ui::Slider* nonactiveLayersOpacity() const { return m_nonactiveLayersOpacity; }
    ui::Button* buttonOk() const { return m_buttonOk; }
    ui::Button* buttonApply() const { return m_buttonApply; }

  private:
    ui::ListBox* m_sectionListbox;
    ui::Panel* m_panel;
    ui::VBox* m_sectionGeneral;
    ui::ComboBox* m_screenScale;
    ui::ComboBox* m_uiScale;
    ui::ComboBox* m_language;
    ui::CheckBox* m_gpuAcceleration;
    ui::CheckBox* m_showMenuBar;
    ui::CheckBox* m_showHome;
    ui::CheckBox* m_expandMenubarOnMouseover;
    ui::CheckBox* m_enableDataRecovery;
    ui::ComboBox* m_dataRecoveryPeriod;
    ui::LinkLabel* m_locateFile;
    ui::LinkLabel* m_locateCrashFolder;
    ui::VBox* m_sectionFiles;
    ui::ComboBox* m_defaultExtension;
    ui::ComboBox* m_exportImageDefaultExtension;
    ui::ComboBox* m_exportAnimationDefaultExtension;
    ui::ComboBox* m_exportSpriteSheetDefaultExtension;
    ui::Slider* m_recentFiles;
    ui::Button* m_clearRecentFiles;
    ui::CheckBox* m_showFullPath;
    ui::VBox* m_sectionEditor;
    ui::CheckBox* m_wheelZoom;
    ui::CheckBox* m_slideZoom;
    ui::CheckBox* m_zoomFromCenterWithWheel;
    ui::CheckBox* m_zoomFromCenterWithKeys;
    ui::CheckBox* m_showScrollbars;
    ui::CheckBox* m_autoScroll;
    ui::CheckBox* m_straightLinePreview;
    ui::CheckBox* m_discardBrush;
    ui::ComboBox* m_rightClickBehavior;
    ui::VBox* m_sectionSelection;
    ui::CheckBox* m_autoOpaque;
    ui::CheckBox* m_keepSelectionAfterClear;
    ui::CheckBox* m_autoShowSelectionEdges;
    ui::CheckBox* m_moveEdges;
    ui::CheckBox* m_modifiersDisableHandles;
    ui::CheckBox* m_moveOnAddMode;
    ui::VBox* m_sectionTimeline;
    BoolPrefWidget<ui::CheckBox>* m_autotimeline;
    BoolPrefWidget<ui::CheckBox>* m_rewindOnStop;
    ui::Entry* m_firstFrame;
    ui::VBox* m_sectionCursors;
    ui::CheckBox* m_nativeCursor;
    ui::Label* m_cursorScaleLabel;
    ui::ComboBox* m_cursorScale;
    ui::ComboBox* m_paintingCursorType;
    ui::ComboBox* m_brushPreview;
    ui::ComboBox* m_cursorColorType;
    app::ColorButton* m_cursorColor;
    ui::VBox* m_sectionBg;
    ui::ComboBox* m_bgScope;
    ui::ComboBox* m_checkedBgSize;
    ui::CheckBox* m_checkedBgZoom;
    app::ColorButton* m_checkedBgColor1;
    app::ColorButton* m_checkedBgColor2;
    ui::Button* m_resetBg;
    ui::VBox* m_sectionGrid;
    ui::ComboBox* m_gridScope;
    ui::CheckBox* m_gridVisible;
    ui::Entry* m_gridX;
    ui::Entry* m_gridY;
    ui::Entry* m_gridW;
    ui::Entry* m_gridH;
    app::ColorButton* m_gridColor;
    ui::Slider* m_gridOpacity;
    ui::CheckBox* m_gridAutoOpacity;
    ui::CheckBox* m_pixelGridVisible;
    app::ColorButton* m_pixelGridColor;
    ui::Slider* m_pixelGridOpacity;
    ui::CheckBox* m_pixelGridAutoOpacity;
    ui::Button* m_resetGrid;
    ui::VBox* m_sectionGuidesAndSlices;
    app::ColorButton* m_layerEdgesColor;
    app::ColorButton* m_autoGuidesColor;
    app::ColorButton* m_defaultSliceColor;
    ui::VBox* m_sectionUndo;
    ui::CheckBox* m_limitUndo;
    ui::Entry* m_undoSizeLimit;
    ui::CheckBox* m_undoGotoModified;
    ui::CheckBox* m_undoAllowNonlinearHistory;
    ui::VBox* m_sectionAlerts;
    BoolPrefWidget<ui::CheckBox>* m_fileFormatDoesntSupportAlert;
    BoolPrefWidget<ui::CheckBox>* m_exportAnimationInSequenceAlert;
    BoolPrefWidget<ui::CheckBox>* m_overwriteFilesOnExportAlert;
    BoolPrefWidget<ui::CheckBox>* m_overwriteFilesOnExportSpriteSheetAlert;
    BoolPrefWidget<ui::CheckBox>* m_gifOptionsAlert;
    BoolPrefWidget<ui::CheckBox>* m_jpegOptionsAlert;
    BoolPrefWidget<ui::CheckBox>* m_advancedModeAlert;
    ui::Button* m_resetAlerts;
    ui::VBox* m_sectionTheme;
    ui::ListBox* m_themeList;
    ui::Button* m_selectTheme;
    ui::Button* m_openThemeFolder;
    ui::VBox* m_sectionExtensions;
    ui::ListBox* m_extensionsList;
    ui::Button* m_addExtension;
    ui::Button* m_disableExtension;
    ui::Button* m_uninstallExtension;
    ui::Button* m_openExtensionFolder;
    ui::VBox* m_sectionExperimental;
    ui::CheckBox* m_nativeClipboard;
    ui::CheckBox* m_nativeFileDialog;
    BoolPrefWidget<ui::CheckBox>* m_oneFingerAsMouseMovement;
    ui::HBox* m_loadWintabDriverBox;
    BoolPrefWidget<ui::CheckBox>* m_loadWintabDriver;
    ui::CheckBox* m_flashLayer;
    ui::Slider* m_nonactiveLayersOpacity;
    ui::Button* m_buttonOk;
    ui::Button* m_buttonApply;
  };

} // namespace gen
} // namespace app

#endif
