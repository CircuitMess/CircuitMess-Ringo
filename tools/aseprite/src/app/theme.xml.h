// Don't modify, generated file from C:/aseprite/data/extensions/aseprite-theme/theme.xml

#ifndef GENERATED_THEME_H_INCLUDED
#define GENERATED_THEME_H_INCLUDED
#pragma once

namespace app {
namespace gen {

  template<typename T>
  class ThemeFile {
  public:

    class Dimensions {
      template<typename> friend class ThemeFile;
    public:
      int scrollbarSize() const { return m_scrollbarSize; }
      int miniScrollbarSize() const { return m_miniScrollbarSize; }
      int tabsWidth() const { return m_tabsWidth; }
      int tabsHeight() const { return m_tabsHeight; }
      int tabsBottomHeight() const { return m_tabsBottomHeight; }
      int dockedTabsHeight() const { return m_dockedTabsHeight; }
      int tabsCloseIconWidth() const { return m_tabsCloseIconWidth; }
      int tabsCloseIconHeight() const { return m_tabsCloseIconHeight; }
      int tabsIconWidth() const { return m_tabsIconWidth; }
      int timelineTopBorder() const { return m_timelineTopBorder; }
      int timelineTagsAreaHeight() const { return m_timelineTagsAreaHeight; }
      int timelineOutlineWidth() const { return m_timelineOutlineWidth; }
      int paletteOutlineWidth() const { return m_paletteOutlineWidth; }
      int colorSliderHeight() const { return m_colorSliderHeight; }
      int timelineBaseSize() const { return m_timelineBaseSize; }
    private:
      int m_scrollbarSize;
      int m_miniScrollbarSize;
      int m_tabsWidth;
      int m_tabsHeight;
      int m_tabsBottomHeight;
      int m_dockedTabsHeight;
      int m_tabsCloseIconWidth;
      int m_tabsCloseIconHeight;
      int m_tabsIconWidth;
      int m_timelineTopBorder;
      int m_timelineTagsAreaHeight;
      int m_timelineOutlineWidth;
      int m_paletteOutlineWidth;
      int m_colorSliderHeight;
      int m_timelineBaseSize;
    };
    class Colors {
      template<typename> friend class ThemeFile;
    public:
      gfx::Color text() const { return m_text; }
      gfx::Color disabled() const { return m_disabled; }
      gfx::Color face() const { return m_face; }
      gfx::Color hotFace() const { return m_hotFace; }
      gfx::Color selected() const { return m_selected; }
      gfx::Color selectedText() const { return m_selectedText; }
      gfx::Color separatorLabel() const { return m_separatorLabel; }
      gfx::Color background() const { return m_background; }
      gfx::Color textboxText() const { return m_textboxText; }
      gfx::Color textboxFace() const { return m_textboxFace; }
      gfx::Color textboxCodeFace() const { return m_textboxCodeFace; }
      gfx::Color entrySuffix() const { return m_entrySuffix; }
      gfx::Color linkText() const { return m_linkText; }
      gfx::Color linkHover() const { return m_linkHover; }
      gfx::Color buttonNormalText() const { return m_buttonNormalText; }
      gfx::Color buttonHotText() const { return m_buttonHotText; }
      gfx::Color buttonSelectedText() const { return m_buttonSelectedText; }
      gfx::Color checkHotFace() const { return m_checkHotFace; }
      gfx::Color checkFocusFace() const { return m_checkFocusFace; }
      gfx::Color radioHotFace() const { return m_radioHotFace; }
      gfx::Color radioFocusFace() const { return m_radioFocusFace; }
      gfx::Color menuitemNormalText() const { return m_menuitemNormalText; }
      gfx::Color menuitemNormalFace() const { return m_menuitemNormalFace; }
      gfx::Color menuitemHotText() const { return m_menuitemHotText; }
      gfx::Color menuitemHotFace() const { return m_menuitemHotFace; }
      gfx::Color menuitemHighlightText() const { return m_menuitemHighlightText; }
      gfx::Color menuitemHighlightFace() const { return m_menuitemHighlightFace; }
      gfx::Color windowFace() const { return m_windowFace; }
      gfx::Color windowTitlebarText() const { return m_windowTitlebarText; }
      gfx::Color windowTitlebarFace() const { return m_windowTitlebarFace; }
      gfx::Color editorFace() const { return m_editorFace; }
      gfx::Color editorSpriteBorder() const { return m_editorSpriteBorder; }
      gfx::Color editorSpriteBottomBorder() const { return m_editorSpriteBottomBorder; }
      gfx::Color editorViewFace() const { return m_editorViewFace; }
      gfx::Color listitemNormalText() const { return m_listitemNormalText; }
      gfx::Color listitemNormalFace() const { return m_listitemNormalFace; }
      gfx::Color listitemSelectedText() const { return m_listitemSelectedText; }
      gfx::Color listitemSelectedFace() const { return m_listitemSelectedFace; }
      gfx::Color sliderEmptyText() const { return m_sliderEmptyText; }
      gfx::Color sliderFullText() const { return m_sliderFullText; }
      gfx::Color tabNormalText() const { return m_tabNormalText; }
      gfx::Color tabActiveText() const { return m_tabActiveText; }
      gfx::Color tabActiveFace() const { return m_tabActiveFace; }
      gfx::Color popupWindowBorder() const { return m_popupWindowBorder; }
      gfx::Color tooltipText() const { return m_tooltipText; }
      gfx::Color tooltipFace() const { return m_tooltipFace; }
      gfx::Color filelistEvenRowText() const { return m_filelistEvenRowText; }
      gfx::Color filelistEvenRowFace() const { return m_filelistEvenRowFace; }
      gfx::Color filelistOddRowText() const { return m_filelistOddRowText; }
      gfx::Color filelistOddRowFace() const { return m_filelistOddRowFace; }
      gfx::Color filelistSelectedRowText() const { return m_filelistSelectedRowText; }
      gfx::Color filelistSelectedRowFace() const { return m_filelistSelectedRowFace; }
      gfx::Color filelistDisabledRowText() const { return m_filelistDisabledRowText; }
      gfx::Color workspace() const { return m_workspace; }
      gfx::Color workspaceText() const { return m_workspaceText; }
      gfx::Color workspaceLink() const { return m_workspaceLink; }
      gfx::Color workspaceLinkHover() const { return m_workspaceLinkHover; }
      gfx::Color timelineNormal() const { return m_timelineNormal; }
      gfx::Color timelineNormalText() const { return m_timelineNormalText; }
      gfx::Color timelineHover() const { return m_timelineHover; }
      gfx::Color timelineHoverText() const { return m_timelineHoverText; }
      gfx::Color timelineActive() const { return m_timelineActive; }
      gfx::Color timelineActiveText() const { return m_timelineActiveText; }
      gfx::Color timelineActiveHover() const { return m_timelineActiveHover; }
      gfx::Color timelineActiveHoverText() const { return m_timelineActiveHoverText; }
      gfx::Color timelineClicked() const { return m_timelineClicked; }
      gfx::Color timelineClickedText() const { return m_timelineClickedText; }
      gfx::Color timelineFocusedText() const { return m_timelineFocusedText; }
      gfx::Color timelinePadding() const { return m_timelinePadding; }
      gfx::Color timelineBandHighlight() const { return m_timelineBandHighlight; }
      gfx::Color timelineBandBg() const { return m_timelineBandBg; }
      gfx::Color statusBarText() const { return m_statusBarText; }
      gfx::Color statusBarFace() const { return m_statusBarFace; }
      gfx::Color flagNormal() const { return m_flagNormal; }
      gfx::Color flagActive() const { return m_flagActive; }
      gfx::Color flagClicked() const { return m_flagClicked; }
      gfx::Color selectBoxRuler() const { return m_selectBoxRuler; }
      gfx::Color selectBoxGrid() const { return m_selectBoxGrid; }
      gfx::Color editPalFace() const { return m_editPalFace; }
    private:
      gfx::Color m_text;
      gfx::Color m_disabled;
      gfx::Color m_face;
      gfx::Color m_hotFace;
      gfx::Color m_selected;
      gfx::Color m_selectedText;
      gfx::Color m_separatorLabel;
      gfx::Color m_background;
      gfx::Color m_textboxText;
      gfx::Color m_textboxFace;
      gfx::Color m_textboxCodeFace;
      gfx::Color m_entrySuffix;
      gfx::Color m_linkText;
      gfx::Color m_linkHover;
      gfx::Color m_buttonNormalText;
      gfx::Color m_buttonHotText;
      gfx::Color m_buttonSelectedText;
      gfx::Color m_checkHotFace;
      gfx::Color m_checkFocusFace;
      gfx::Color m_radioHotFace;
      gfx::Color m_radioFocusFace;
      gfx::Color m_menuitemNormalText;
      gfx::Color m_menuitemNormalFace;
      gfx::Color m_menuitemHotText;
      gfx::Color m_menuitemHotFace;
      gfx::Color m_menuitemHighlightText;
      gfx::Color m_menuitemHighlightFace;
      gfx::Color m_windowFace;
      gfx::Color m_windowTitlebarText;
      gfx::Color m_windowTitlebarFace;
      gfx::Color m_editorFace;
      gfx::Color m_editorSpriteBorder;
      gfx::Color m_editorSpriteBottomBorder;
      gfx::Color m_editorViewFace;
      gfx::Color m_listitemNormalText;
      gfx::Color m_listitemNormalFace;
      gfx::Color m_listitemSelectedText;
      gfx::Color m_listitemSelectedFace;
      gfx::Color m_sliderEmptyText;
      gfx::Color m_sliderFullText;
      gfx::Color m_tabNormalText;
      gfx::Color m_tabActiveText;
      gfx::Color m_tabActiveFace;
      gfx::Color m_popupWindowBorder;
      gfx::Color m_tooltipText;
      gfx::Color m_tooltipFace;
      gfx::Color m_filelistEvenRowText;
      gfx::Color m_filelistEvenRowFace;
      gfx::Color m_filelistOddRowText;
      gfx::Color m_filelistOddRowFace;
      gfx::Color m_filelistSelectedRowText;
      gfx::Color m_filelistSelectedRowFace;
      gfx::Color m_filelistDisabledRowText;
      gfx::Color m_workspace;
      gfx::Color m_workspaceText;
      gfx::Color m_workspaceLink;
      gfx::Color m_workspaceLinkHover;
      gfx::Color m_timelineNormal;
      gfx::Color m_timelineNormalText;
      gfx::Color m_timelineHover;
      gfx::Color m_timelineHoverText;
      gfx::Color m_timelineActive;
      gfx::Color m_timelineActiveText;
      gfx::Color m_timelineActiveHover;
      gfx::Color m_timelineActiveHoverText;
      gfx::Color m_timelineClicked;
      gfx::Color m_timelineClickedText;
      gfx::Color m_timelineFocusedText;
      gfx::Color m_timelinePadding;
      gfx::Color m_timelineBandHighlight;
      gfx::Color m_timelineBandBg;
      gfx::Color m_statusBarText;
      gfx::Color m_statusBarFace;
      gfx::Color m_flagNormal;
      gfx::Color m_flagActive;
      gfx::Color m_flagClicked;
      gfx::Color m_selectBoxRuler;
      gfx::Color m_selectBoxGrid;
      gfx::Color m_editPalFace;
    };
    class Parts {
      template<typename> friend class ThemeFile;
    public:
      const skin::SkinPartPtr& radioNormal() const { return m_radioNormal; }
      const skin::SkinPartPtr& radioSelected() const { return m_radioSelected; }
      const skin::SkinPartPtr& radioDisabled() const { return m_radioDisabled; }
      const skin::SkinPartPtr& checkNormal() const { return m_checkNormal; }
      const skin::SkinPartPtr& checkSelected() const { return m_checkSelected; }
      const skin::SkinPartPtr& checkDisabled() const { return m_checkDisabled; }
      const skin::SkinPartPtr& checkFocus() const { return m_checkFocus; }
      const skin::SkinPartPtr& radioFocus() const { return m_radioFocus; }
      const skin::SkinPartPtr& buttonNormal() const { return m_buttonNormal; }
      const skin::SkinPartPtr& buttonHot() const { return m_buttonHot; }
      const skin::SkinPartPtr& buttonFocused() const { return m_buttonFocused; }
      const skin::SkinPartPtr& buttonSelected() const { return m_buttonSelected; }
      const skin::SkinPartPtr& sunkenNormal() const { return m_sunkenNormal; }
      const skin::SkinPartPtr& sunkenFocused() const { return m_sunkenFocused; }
      const skin::SkinPartPtr& sunken2Normal() const { return m_sunken2Normal; }
      const skin::SkinPartPtr& sunken2Focused() const { return m_sunken2Focused; }
      const skin::SkinPartPtr& sunkenMiniNormal() const { return m_sunkenMiniNormal; }
      const skin::SkinPartPtr& sunkenMiniFocused() const { return m_sunkenMiniFocused; }
      const skin::SkinPartPtr& window() const { return m_window; }
      const skin::SkinPartPtr& menu() const { return m_menu; }
      const skin::SkinPartPtr& windowButtonNormal() const { return m_windowButtonNormal; }
      const skin::SkinPartPtr& windowButtonHot() const { return m_windowButtonHot; }
      const skin::SkinPartPtr& windowButtonSelected() const { return m_windowButtonSelected; }
      const skin::SkinPartPtr& windowCloseIcon() const { return m_windowCloseIcon; }
      const skin::SkinPartPtr& windowPlayIcon() const { return m_windowPlayIcon; }
      const skin::SkinPartPtr& windowStopIcon() const { return m_windowStopIcon; }
      const skin::SkinPartPtr& windowCenterIcon() const { return m_windowCenterIcon; }
      const skin::SkinPartPtr& sliderFull() const { return m_sliderFull; }
      const skin::SkinPartPtr& sliderEmpty() const { return m_sliderEmpty; }
      const skin::SkinPartPtr& sliderFullFocused() const { return m_sliderFullFocused; }
      const skin::SkinPartPtr& sliderEmptyFocused() const { return m_sliderEmptyFocused; }
      const skin::SkinPartPtr& miniSliderFull() const { return m_miniSliderFull; }
      const skin::SkinPartPtr& miniSliderEmpty() const { return m_miniSliderEmpty; }
      const skin::SkinPartPtr& miniSliderFullFocused() const { return m_miniSliderFullFocused; }
      const skin::SkinPartPtr& miniSliderEmptyFocused() const { return m_miniSliderEmptyFocused; }
      const skin::SkinPartPtr& miniSliderThumb() const { return m_miniSliderThumb; }
      const skin::SkinPartPtr& miniSliderThumbFocused() const { return m_miniSliderThumbFocused; }
      const skin::SkinPartPtr& separatorHorz() const { return m_separatorHorz; }
      const skin::SkinPartPtr& separatorVert() const { return m_separatorVert; }
      const skin::SkinPartPtr& comboboxArrowDown() const { return m_comboboxArrowDown; }
      const skin::SkinPartPtr& comboboxArrowDownSelected() const { return m_comboboxArrowDownSelected; }
      const skin::SkinPartPtr& comboboxArrowDownDisabled() const { return m_comboboxArrowDownDisabled; }
      const skin::SkinPartPtr& comboboxArrowUp() const { return m_comboboxArrowUp; }
      const skin::SkinPartPtr& comboboxArrowUpSelected() const { return m_comboboxArrowUpSelected; }
      const skin::SkinPartPtr& comboboxArrowUpDisabled() const { return m_comboboxArrowUpDisabled; }
      const skin::SkinPartPtr& comboboxArrowLeft() const { return m_comboboxArrowLeft; }
      const skin::SkinPartPtr& comboboxArrowLeftSelected() const { return m_comboboxArrowLeftSelected; }
      const skin::SkinPartPtr& comboboxArrowLeftDisabled() const { return m_comboboxArrowLeftDisabled; }
      const skin::SkinPartPtr& comboboxArrowRight() const { return m_comboboxArrowRight; }
      const skin::SkinPartPtr& comboboxArrowRightSelected() const { return m_comboboxArrowRightSelected; }
      const skin::SkinPartPtr& comboboxArrowRightDisabled() const { return m_comboboxArrowRightDisabled; }
      const skin::SkinPartPtr& newfolder() const { return m_newfolder; }
      const skin::SkinPartPtr& newfolderSelected() const { return m_newfolderSelected; }
      const skin::SkinPartPtr& toolbuttonNormal() const { return m_toolbuttonNormal; }
      const skin::SkinPartPtr& toolbuttonHot() const { return m_toolbuttonHot; }
      const skin::SkinPartPtr& toolbuttonLast() const { return m_toolbuttonLast; }
      const skin::SkinPartPtr& toolbuttonPushed() const { return m_toolbuttonPushed; }
      const skin::SkinPartPtr& buttonsetItemNormal() const { return m_buttonsetItemNormal; }
      const skin::SkinPartPtr& buttonsetItemHot() const { return m_buttonsetItemHot; }
      const skin::SkinPartPtr& buttonsetItemHotFocused() const { return m_buttonsetItemHotFocused; }
      const skin::SkinPartPtr& buttonsetItemFocused() const { return m_buttonsetItemFocused; }
      const skin::SkinPartPtr& buttonsetItemPushed() const { return m_buttonsetItemPushed; }
      const skin::SkinPartPtr& tabNormal() const { return m_tabNormal; }
      const skin::SkinPartPtr& tabActive() const { return m_tabActive; }
      const skin::SkinPartPtr& tabBottomActive() const { return m_tabBottomActive; }
      const skin::SkinPartPtr& tabBottomNormal() const { return m_tabBottomNormal; }
      const skin::SkinPartPtr& tabFiller() const { return m_tabFiller; }
      const skin::SkinPartPtr& tabModifiedIconNormal() const { return m_tabModifiedIconNormal; }
      const skin::SkinPartPtr& tabModifiedIconActive() const { return m_tabModifiedIconActive; }
      const skin::SkinPartPtr& tabCloseIconNormal() const { return m_tabCloseIconNormal; }
      const skin::SkinPartPtr& tabCloseIconActive() const { return m_tabCloseIconActive; }
      const skin::SkinPartPtr& tabIconBgClicked() const { return m_tabIconBgClicked; }
      const skin::SkinPartPtr& tabIconBgHover() const { return m_tabIconBgHover; }
      const skin::SkinPartPtr& tabHomeIconNormal() const { return m_tabHomeIconNormal; }
      const skin::SkinPartPtr& tabHomeIconActive() const { return m_tabHomeIconActive; }
      const skin::SkinPartPtr& editorNormal() const { return m_editorNormal; }
      const skin::SkinPartPtr& editorSelected() const { return m_editorSelected; }
      const skin::SkinPartPtr& colorbar0() const { return m_colorbar0; }
      const skin::SkinPartPtr& colorbar1() const { return m_colorbar1; }
      const skin::SkinPartPtr& colorbar2() const { return m_colorbar2; }
      const skin::SkinPartPtr& colorbar3() const { return m_colorbar3; }
      const skin::SkinPartPtr& colorbarSelectionHot() const { return m_colorbarSelectionHot; }
      const skin::SkinPartPtr& colorbarSelection() const { return m_colorbarSelection; }
      const skin::SkinPartPtr& scrollbarBg() const { return m_scrollbarBg; }
      const skin::SkinPartPtr& scrollbarThumb() const { return m_scrollbarThumb; }
      const skin::SkinPartPtr& miniScrollbarBg() const { return m_miniScrollbarBg; }
      const skin::SkinPartPtr& miniScrollbarThumb() const { return m_miniScrollbarThumb; }
      const skin::SkinPartPtr& miniScrollbarBgHot() const { return m_miniScrollbarBgHot; }
      const skin::SkinPartPtr& miniScrollbarThumbHot() const { return m_miniScrollbarThumbHot; }
      const skin::SkinPartPtr& transparentScrollbarBg() const { return m_transparentScrollbarBg; }
      const skin::SkinPartPtr& transparentScrollbarThumb() const { return m_transparentScrollbarThumb; }
      const skin::SkinPartPtr& transparentScrollbarBgHot() const { return m_transparentScrollbarBgHot; }
      const skin::SkinPartPtr& transparentScrollbarThumbHot() const { return m_transparentScrollbarThumbHot; }
      const skin::SkinPartPtr& tooltip() const { return m_tooltip; }
      const skin::SkinPartPtr& tooltipArrow() const { return m_tooltipArrow; }
      const skin::SkinPartPtr& aniFirst() const { return m_aniFirst; }
      const skin::SkinPartPtr& aniPrevious() const { return m_aniPrevious; }
      const skin::SkinPartPtr& aniPlay() const { return m_aniPlay; }
      const skin::SkinPartPtr& aniStop() const { return m_aniStop; }
      const skin::SkinPartPtr& aniNext() const { return m_aniNext; }
      const skin::SkinPartPtr& aniLast() const { return m_aniLast; }
      const skin::SkinPartPtr& palEdit() const { return m_palEdit; }
      const skin::SkinPartPtr& palSort() const { return m_palSort; }
      const skin::SkinPartPtr& palPresets() const { return m_palPresets; }
      const skin::SkinPartPtr& palOptions() const { return m_palOptions; }
      const skin::SkinPartPtr& palResize() const { return m_palResize; }
      const skin::SkinPartPtr& selectionReplace() const { return m_selectionReplace; }
      const skin::SkinPartPtr& selectionAdd() const { return m_selectionAdd; }
      const skin::SkinPartPtr& selectionSubtract() const { return m_selectionSubtract; }
      const skin::SkinPartPtr& unpinned() const { return m_unpinned; }
      const skin::SkinPartPtr& pinned() const { return m_pinned; }
      const skin::SkinPartPtr& dropDownButtonLeftNormal() const { return m_dropDownButtonLeftNormal; }
      const skin::SkinPartPtr& dropDownButtonLeftHot() const { return m_dropDownButtonLeftHot; }
      const skin::SkinPartPtr& dropDownButtonLeftFocused() const { return m_dropDownButtonLeftFocused; }
      const skin::SkinPartPtr& dropDownButtonLeftSelected() const { return m_dropDownButtonLeftSelected; }
      const skin::SkinPartPtr& dropDownButtonRightNormal() const { return m_dropDownButtonRightNormal; }
      const skin::SkinPartPtr& dropDownButtonRightHot() const { return m_dropDownButtonRightHot; }
      const skin::SkinPartPtr& dropDownButtonRightFocused() const { return m_dropDownButtonRightFocused; }
      const skin::SkinPartPtr& dropDownButtonRightSelected() const { return m_dropDownButtonRightSelected; }
      const skin::SkinPartPtr& transformationHandle() const { return m_transformationHandle; }
      const skin::SkinPartPtr& pivotHandle() const { return m_pivotHandle; }
      const skin::SkinPartPtr& timelineNone() const { return m_timelineNone; }
      const skin::SkinPartPtr& timelineNormal() const { return m_timelineNormal; }
      const skin::SkinPartPtr& timelineActive() const { return m_timelineActive; }
      const skin::SkinPartPtr& timelineHover() const { return m_timelineHover; }
      const skin::SkinPartPtr& timelineActiveHover() const { return m_timelineActiveHover; }
      const skin::SkinPartPtr& timelineClicked() const { return m_timelineClicked; }
      const skin::SkinPartPtr& timelineOpenEyeNormal() const { return m_timelineOpenEyeNormal; }
      const skin::SkinPartPtr& timelineOpenEyeActive() const { return m_timelineOpenEyeActive; }
      const skin::SkinPartPtr& timelineClosedEyeNormal() const { return m_timelineClosedEyeNormal; }
      const skin::SkinPartPtr& timelineClosedEyeActive() const { return m_timelineClosedEyeActive; }
      const skin::SkinPartPtr& timelineOpenPadlockNormal() const { return m_timelineOpenPadlockNormal; }
      const skin::SkinPartPtr& timelineOpenPadlockActive() const { return m_timelineOpenPadlockActive; }
      const skin::SkinPartPtr& timelineClosedPadlockNormal() const { return m_timelineClosedPadlockNormal; }
      const skin::SkinPartPtr& timelineClosedPadlockActive() const { return m_timelineClosedPadlockActive; }
      const skin::SkinPartPtr& timelineContinuousNormal() const { return m_timelineContinuousNormal; }
      const skin::SkinPartPtr& timelineContinuousActive() const { return m_timelineContinuousActive; }
      const skin::SkinPartPtr& timelineDiscontinuousNormal() const { return m_timelineDiscontinuousNormal; }
      const skin::SkinPartPtr& timelineDiscontinuousActive() const { return m_timelineDiscontinuousActive; }
      const skin::SkinPartPtr& timelineClosedGroupNormal() const { return m_timelineClosedGroupNormal; }
      const skin::SkinPartPtr& timelineClosedGroupActive() const { return m_timelineClosedGroupActive; }
      const skin::SkinPartPtr& timelineOpenGroupNormal() const { return m_timelineOpenGroupNormal; }
      const skin::SkinPartPtr& timelineOpenGroupActive() const { return m_timelineOpenGroupActive; }
      const skin::SkinPartPtr& timelineEmptyFrameNormal() const { return m_timelineEmptyFrameNormal; }
      const skin::SkinPartPtr& timelineEmptyFrameActive() const { return m_timelineEmptyFrameActive; }
      const skin::SkinPartPtr& timelineKeyframeNormal() const { return m_timelineKeyframeNormal; }
      const skin::SkinPartPtr& timelineKeyframeActive() const { return m_timelineKeyframeActive; }
      const skin::SkinPartPtr& timelineFromLeftNormal() const { return m_timelineFromLeftNormal; }
      const skin::SkinPartPtr& timelineFromLeftActive() const { return m_timelineFromLeftActive; }
      const skin::SkinPartPtr& timelineFromRightNormal() const { return m_timelineFromRightNormal; }
      const skin::SkinPartPtr& timelineFromRightActive() const { return m_timelineFromRightActive; }
      const skin::SkinPartPtr& timelineFromBothNormal() const { return m_timelineFromBothNormal; }
      const skin::SkinPartPtr& timelineFromBothActive() const { return m_timelineFromBothActive; }
      const skin::SkinPartPtr& timelineLeftLinkActive() const { return m_timelineLeftLinkActive; }
      const skin::SkinPartPtr& timelineBothLinksActive() const { return m_timelineBothLinksActive; }
      const skin::SkinPartPtr& timelineRightLinkActive() const { return m_timelineRightLinkActive; }
      const skin::SkinPartPtr& timelineGear() const { return m_timelineGear; }
      const skin::SkinPartPtr& timelineGearActive() const { return m_timelineGearActive; }
      const skin::SkinPartPtr& timelineOnionskin() const { return m_timelineOnionskin; }
      const skin::SkinPartPtr& timelineOnionskinActive() const { return m_timelineOnionskinActive; }
      const skin::SkinPartPtr& timelineOnionskinRange() const { return m_timelineOnionskinRange; }
      const skin::SkinPartPtr& timelinePadding() const { return m_timelinePadding; }
      const skin::SkinPartPtr& timelinePaddingTr() const { return m_timelinePaddingTr; }
      const skin::SkinPartPtr& timelinePaddingBl() const { return m_timelinePaddingBl; }
      const skin::SkinPartPtr& timelinePaddingBr() const { return m_timelinePaddingBr; }
      const skin::SkinPartPtr& timelineDropLayerDeco() const { return m_timelineDropLayerDeco; }
      const skin::SkinPartPtr& timelineDropFrameDeco() const { return m_timelineDropFrameDeco; }
      const skin::SkinPartPtr& timelineLoopRange() const { return m_timelineLoopRange; }
      const skin::SkinPartPtr& timelineFocused() const { return m_timelineFocused; }
      const skin::SkinPartPtr& flagNormal() const { return m_flagNormal; }
      const skin::SkinPartPtr& flagHighlight() const { return m_flagHighlight; }
      const skin::SkinPartPtr& dropPixelsOk() const { return m_dropPixelsOk; }
      const skin::SkinPartPtr& dropPixelsOkSelected() const { return m_dropPixelsOkSelected; }
      const skin::SkinPartPtr& dropPixelsCancel() const { return m_dropPixelsCancel; }
      const skin::SkinPartPtr& dropPixelsCancelSelected() const { return m_dropPixelsCancelSelected; }
      const skin::SkinPartPtr& warningBox() const { return m_warningBox; }
      const skin::SkinPartPtr& canvasNw() const { return m_canvasNw; }
      const skin::SkinPartPtr& canvasN() const { return m_canvasN; }
      const skin::SkinPartPtr& canvasNe() const { return m_canvasNe; }
      const skin::SkinPartPtr& canvasW() const { return m_canvasW; }
      const skin::SkinPartPtr& canvasC() const { return m_canvasC; }
      const skin::SkinPartPtr& canvasE() const { return m_canvasE; }
      const skin::SkinPartPtr& canvasSw() const { return m_canvasSw; }
      const skin::SkinPartPtr& canvasS() const { return m_canvasS; }
      const skin::SkinPartPtr& canvasSe() const { return m_canvasSe; }
      const skin::SkinPartPtr& canvasEmpty() const { return m_canvasEmpty; }
      const skin::SkinPartPtr& inkSimple() const { return m_inkSimple; }
      const skin::SkinPartPtr& inkAlphaCompositing() const { return m_inkAlphaCompositing; }
      const skin::SkinPartPtr& inkCopyColor() const { return m_inkCopyColor; }
      const skin::SkinPartPtr& inkLockAlpha() const { return m_inkLockAlpha; }
      const skin::SkinPartPtr& inkShading() const { return m_inkShading; }
      const skin::SkinPartPtr& selectionOpaque() const { return m_selectionOpaque; }
      const skin::SkinPartPtr& selectionMasked() const { return m_selectionMasked; }
      const skin::SkinPartPtr& pivotNorthwest() const { return m_pivotNorthwest; }
      const skin::SkinPartPtr& pivotNorth() const { return m_pivotNorth; }
      const skin::SkinPartPtr& pivotNortheast() const { return m_pivotNortheast; }
      const skin::SkinPartPtr& pivotWest() const { return m_pivotWest; }
      const skin::SkinPartPtr& pivotCenter() const { return m_pivotCenter; }
      const skin::SkinPartPtr& pivotEast() const { return m_pivotEast; }
      const skin::SkinPartPtr& pivotSouthwest() const { return m_pivotSouthwest; }
      const skin::SkinPartPtr& pivotSouth() const { return m_pivotSouth; }
      const skin::SkinPartPtr& pivotSoutheast() const { return m_pivotSoutheast; }
      const skin::SkinPartPtr& iconRgb() const { return m_iconRgb; }
      const skin::SkinPartPtr& iconGrayscale() const { return m_iconGrayscale; }
      const skin::SkinPartPtr& iconIndexed() const { return m_iconIndexed; }
      const skin::SkinPartPtr& iconBlack() const { return m_iconBlack; }
      const skin::SkinPartPtr& iconWhite() const { return m_iconWhite; }
      const skin::SkinPartPtr& iconTransparent() const { return m_iconTransparent; }
      const skin::SkinPartPtr& colorWheelIndicator() const { return m_colorWheelIndicator; }
      const skin::SkinPartPtr& noSymmetry() const { return m_noSymmetry; }
      const skin::SkinPartPtr& horizontalSymmetry() const { return m_horizontalSymmetry; }
      const skin::SkinPartPtr& verticalSymmetry() const { return m_verticalSymmetry; }
      const skin::SkinPartPtr& iconArrowDown() const { return m_iconArrowDown; }
      const skin::SkinPartPtr& iconClose() const { return m_iconClose; }
      const skin::SkinPartPtr& iconSearch() const { return m_iconSearch; }
      const skin::SkinPartPtr& iconUserData() const { return m_iconUserData; }
      const skin::SkinPartPtr& iconPos() const { return m_iconPos; }
      const skin::SkinPartPtr& iconSize() const { return m_iconSize; }
      const skin::SkinPartPtr& iconSelsize() const { return m_iconSelsize; }
      const skin::SkinPartPtr& iconFrame() const { return m_iconFrame; }
      const skin::SkinPartPtr& iconClock() const { return m_iconClock; }
      const skin::SkinPartPtr& iconStart() const { return m_iconStart; }
      const skin::SkinPartPtr& iconEnd() const { return m_iconEnd; }
      const skin::SkinPartPtr& iconAngle() const { return m_iconAngle; }
      const skin::SkinPartPtr& iconKey() const { return m_iconKey; }
      const skin::SkinPartPtr& iconDistance() const { return m_iconDistance; }
      const skin::SkinPartPtr& iconGrid() const { return m_iconGrid; }
      const skin::SkinPartPtr& iconSave() const { return m_iconSave; }
      const skin::SkinPartPtr& iconSaveSmall() const { return m_iconSaveSmall; }
      const skin::SkinPartPtr& iconSlice() const { return m_iconSlice; }
      const skin::SkinPartPtr& toolRectangularMarquee() const { return m_toolRectangularMarquee; }
      const skin::SkinPartPtr& toolEllipticalMarquee() const { return m_toolEllipticalMarquee; }
      const skin::SkinPartPtr& toolLasso() const { return m_toolLasso; }
      const skin::SkinPartPtr& toolPolygonalLasso() const { return m_toolPolygonalLasso; }
      const skin::SkinPartPtr& toolMagicWand() const { return m_toolMagicWand; }
      const skin::SkinPartPtr& toolPencil() const { return m_toolPencil; }
      const skin::SkinPartPtr& toolSpray() const { return m_toolSpray; }
      const skin::SkinPartPtr& toolEraser() const { return m_toolEraser; }
      const skin::SkinPartPtr& toolEyedropper() const { return m_toolEyedropper; }
      const skin::SkinPartPtr& toolHand() const { return m_toolHand; }
      const skin::SkinPartPtr& toolMove() const { return m_toolMove; }
      const skin::SkinPartPtr& toolZoom() const { return m_toolZoom; }
      const skin::SkinPartPtr& toolSlice() const { return m_toolSlice; }
      const skin::SkinPartPtr& toolPaintBucket() const { return m_toolPaintBucket; }
      const skin::SkinPartPtr& toolGradient() const { return m_toolGradient; }
      const skin::SkinPartPtr& toolLine() const { return m_toolLine; }
      const skin::SkinPartPtr& toolCurve() const { return m_toolCurve; }
      const skin::SkinPartPtr& toolRectangle() const { return m_toolRectangle; }
      const skin::SkinPartPtr& toolFilledRectangle() const { return m_toolFilledRectangle; }
      const skin::SkinPartPtr& toolEllipse() const { return m_toolEllipse; }
      const skin::SkinPartPtr& toolFilledEllipse() const { return m_toolFilledEllipse; }
      const skin::SkinPartPtr& toolContour() const { return m_toolContour; }
      const skin::SkinPartPtr& toolPolygon() const { return m_toolPolygon; }
      const skin::SkinPartPtr& toolBlur() const { return m_toolBlur; }
      const skin::SkinPartPtr& toolJumble() const { return m_toolJumble; }
      const skin::SkinPartPtr& toolConfiguration() const { return m_toolConfiguration; }
      const skin::SkinPartPtr& toolMinieditor() const { return m_toolMinieditor; }
      const skin::SkinPartPtr& simpleColorBorder() const { return m_simpleColorBorder; }
      const skin::SkinPartPtr& simpleColorSelected() const { return m_simpleColorSelected; }
      const skin::SkinPartPtr& asepriteFace() const { return m_asepriteFace; }
      const skin::SkinPartPtr& asepriteFaceMouse() const { return m_asepriteFaceMouse; }
      const skin::SkinPartPtr& asepriteFacePushed() const { return m_asepriteFacePushed; }
    private:
      skin::SkinPartPtr m_radioNormal;
      skin::SkinPartPtr m_radioSelected;
      skin::SkinPartPtr m_radioDisabled;
      skin::SkinPartPtr m_checkNormal;
      skin::SkinPartPtr m_checkSelected;
      skin::SkinPartPtr m_checkDisabled;
      skin::SkinPartPtr m_checkFocus;
      skin::SkinPartPtr m_radioFocus;
      skin::SkinPartPtr m_buttonNormal;
      skin::SkinPartPtr m_buttonHot;
      skin::SkinPartPtr m_buttonFocused;
      skin::SkinPartPtr m_buttonSelected;
      skin::SkinPartPtr m_sunkenNormal;
      skin::SkinPartPtr m_sunkenFocused;
      skin::SkinPartPtr m_sunken2Normal;
      skin::SkinPartPtr m_sunken2Focused;
      skin::SkinPartPtr m_sunkenMiniNormal;
      skin::SkinPartPtr m_sunkenMiniFocused;
      skin::SkinPartPtr m_window;
      skin::SkinPartPtr m_menu;
      skin::SkinPartPtr m_windowButtonNormal;
      skin::SkinPartPtr m_windowButtonHot;
      skin::SkinPartPtr m_windowButtonSelected;
      skin::SkinPartPtr m_windowCloseIcon;
      skin::SkinPartPtr m_windowPlayIcon;
      skin::SkinPartPtr m_windowStopIcon;
      skin::SkinPartPtr m_windowCenterIcon;
      skin::SkinPartPtr m_sliderFull;
      skin::SkinPartPtr m_sliderEmpty;
      skin::SkinPartPtr m_sliderFullFocused;
      skin::SkinPartPtr m_sliderEmptyFocused;
      skin::SkinPartPtr m_miniSliderFull;
      skin::SkinPartPtr m_miniSliderEmpty;
      skin::SkinPartPtr m_miniSliderFullFocused;
      skin::SkinPartPtr m_miniSliderEmptyFocused;
      skin::SkinPartPtr m_miniSliderThumb;
      skin::SkinPartPtr m_miniSliderThumbFocused;
      skin::SkinPartPtr m_separatorHorz;
      skin::SkinPartPtr m_separatorVert;
      skin::SkinPartPtr m_comboboxArrowDown;
      skin::SkinPartPtr m_comboboxArrowDownSelected;
      skin::SkinPartPtr m_comboboxArrowDownDisabled;
      skin::SkinPartPtr m_comboboxArrowUp;
      skin::SkinPartPtr m_comboboxArrowUpSelected;
      skin::SkinPartPtr m_comboboxArrowUpDisabled;
      skin::SkinPartPtr m_comboboxArrowLeft;
      skin::SkinPartPtr m_comboboxArrowLeftSelected;
      skin::SkinPartPtr m_comboboxArrowLeftDisabled;
      skin::SkinPartPtr m_comboboxArrowRight;
      skin::SkinPartPtr m_comboboxArrowRightSelected;
      skin::SkinPartPtr m_comboboxArrowRightDisabled;
      skin::SkinPartPtr m_newfolder;
      skin::SkinPartPtr m_newfolderSelected;
      skin::SkinPartPtr m_toolbuttonNormal;
      skin::SkinPartPtr m_toolbuttonHot;
      skin::SkinPartPtr m_toolbuttonLast;
      skin::SkinPartPtr m_toolbuttonPushed;
      skin::SkinPartPtr m_buttonsetItemNormal;
      skin::SkinPartPtr m_buttonsetItemHot;
      skin::SkinPartPtr m_buttonsetItemHotFocused;
      skin::SkinPartPtr m_buttonsetItemFocused;
      skin::SkinPartPtr m_buttonsetItemPushed;
      skin::SkinPartPtr m_tabNormal;
      skin::SkinPartPtr m_tabActive;
      skin::SkinPartPtr m_tabBottomActive;
      skin::SkinPartPtr m_tabBottomNormal;
      skin::SkinPartPtr m_tabFiller;
      skin::SkinPartPtr m_tabModifiedIconNormal;
      skin::SkinPartPtr m_tabModifiedIconActive;
      skin::SkinPartPtr m_tabCloseIconNormal;
      skin::SkinPartPtr m_tabCloseIconActive;
      skin::SkinPartPtr m_tabIconBgClicked;
      skin::SkinPartPtr m_tabIconBgHover;
      skin::SkinPartPtr m_tabHomeIconNormal;
      skin::SkinPartPtr m_tabHomeIconActive;
      skin::SkinPartPtr m_editorNormal;
      skin::SkinPartPtr m_editorSelected;
      skin::SkinPartPtr m_colorbar0;
      skin::SkinPartPtr m_colorbar1;
      skin::SkinPartPtr m_colorbar2;
      skin::SkinPartPtr m_colorbar3;
      skin::SkinPartPtr m_colorbarSelectionHot;
      skin::SkinPartPtr m_colorbarSelection;
      skin::SkinPartPtr m_scrollbarBg;
      skin::SkinPartPtr m_scrollbarThumb;
      skin::SkinPartPtr m_miniScrollbarBg;
      skin::SkinPartPtr m_miniScrollbarThumb;
      skin::SkinPartPtr m_miniScrollbarBgHot;
      skin::SkinPartPtr m_miniScrollbarThumbHot;
      skin::SkinPartPtr m_transparentScrollbarBg;
      skin::SkinPartPtr m_transparentScrollbarThumb;
      skin::SkinPartPtr m_transparentScrollbarBgHot;
      skin::SkinPartPtr m_transparentScrollbarThumbHot;
      skin::SkinPartPtr m_tooltip;
      skin::SkinPartPtr m_tooltipArrow;
      skin::SkinPartPtr m_aniFirst;
      skin::SkinPartPtr m_aniPrevious;
      skin::SkinPartPtr m_aniPlay;
      skin::SkinPartPtr m_aniStop;
      skin::SkinPartPtr m_aniNext;
      skin::SkinPartPtr m_aniLast;
      skin::SkinPartPtr m_palEdit;
      skin::SkinPartPtr m_palSort;
      skin::SkinPartPtr m_palPresets;
      skin::SkinPartPtr m_palOptions;
      skin::SkinPartPtr m_palResize;
      skin::SkinPartPtr m_selectionReplace;
      skin::SkinPartPtr m_selectionAdd;
      skin::SkinPartPtr m_selectionSubtract;
      skin::SkinPartPtr m_unpinned;
      skin::SkinPartPtr m_pinned;
      skin::SkinPartPtr m_dropDownButtonLeftNormal;
      skin::SkinPartPtr m_dropDownButtonLeftHot;
      skin::SkinPartPtr m_dropDownButtonLeftFocused;
      skin::SkinPartPtr m_dropDownButtonLeftSelected;
      skin::SkinPartPtr m_dropDownButtonRightNormal;
      skin::SkinPartPtr m_dropDownButtonRightHot;
      skin::SkinPartPtr m_dropDownButtonRightFocused;
      skin::SkinPartPtr m_dropDownButtonRightSelected;
      skin::SkinPartPtr m_transformationHandle;
      skin::SkinPartPtr m_pivotHandle;
      skin::SkinPartPtr m_timelineNone;
      skin::SkinPartPtr m_timelineNormal;
      skin::SkinPartPtr m_timelineActive;
      skin::SkinPartPtr m_timelineHover;
      skin::SkinPartPtr m_timelineActiveHover;
      skin::SkinPartPtr m_timelineClicked;
      skin::SkinPartPtr m_timelineOpenEyeNormal;
      skin::SkinPartPtr m_timelineOpenEyeActive;
      skin::SkinPartPtr m_timelineClosedEyeNormal;
      skin::SkinPartPtr m_timelineClosedEyeActive;
      skin::SkinPartPtr m_timelineOpenPadlockNormal;
      skin::SkinPartPtr m_timelineOpenPadlockActive;
      skin::SkinPartPtr m_timelineClosedPadlockNormal;
      skin::SkinPartPtr m_timelineClosedPadlockActive;
      skin::SkinPartPtr m_timelineContinuousNormal;
      skin::SkinPartPtr m_timelineContinuousActive;
      skin::SkinPartPtr m_timelineDiscontinuousNormal;
      skin::SkinPartPtr m_timelineDiscontinuousActive;
      skin::SkinPartPtr m_timelineClosedGroupNormal;
      skin::SkinPartPtr m_timelineClosedGroupActive;
      skin::SkinPartPtr m_timelineOpenGroupNormal;
      skin::SkinPartPtr m_timelineOpenGroupActive;
      skin::SkinPartPtr m_timelineEmptyFrameNormal;
      skin::SkinPartPtr m_timelineEmptyFrameActive;
      skin::SkinPartPtr m_timelineKeyframeNormal;
      skin::SkinPartPtr m_timelineKeyframeActive;
      skin::SkinPartPtr m_timelineFromLeftNormal;
      skin::SkinPartPtr m_timelineFromLeftActive;
      skin::SkinPartPtr m_timelineFromRightNormal;
      skin::SkinPartPtr m_timelineFromRightActive;
      skin::SkinPartPtr m_timelineFromBothNormal;
      skin::SkinPartPtr m_timelineFromBothActive;
      skin::SkinPartPtr m_timelineLeftLinkActive;
      skin::SkinPartPtr m_timelineBothLinksActive;
      skin::SkinPartPtr m_timelineRightLinkActive;
      skin::SkinPartPtr m_timelineGear;
      skin::SkinPartPtr m_timelineGearActive;
      skin::SkinPartPtr m_timelineOnionskin;
      skin::SkinPartPtr m_timelineOnionskinActive;
      skin::SkinPartPtr m_timelineOnionskinRange;
      skin::SkinPartPtr m_timelinePadding;
      skin::SkinPartPtr m_timelinePaddingTr;
      skin::SkinPartPtr m_timelinePaddingBl;
      skin::SkinPartPtr m_timelinePaddingBr;
      skin::SkinPartPtr m_timelineDropLayerDeco;
      skin::SkinPartPtr m_timelineDropFrameDeco;
      skin::SkinPartPtr m_timelineLoopRange;
      skin::SkinPartPtr m_timelineFocused;
      skin::SkinPartPtr m_flagNormal;
      skin::SkinPartPtr m_flagHighlight;
      skin::SkinPartPtr m_dropPixelsOk;
      skin::SkinPartPtr m_dropPixelsOkSelected;
      skin::SkinPartPtr m_dropPixelsCancel;
      skin::SkinPartPtr m_dropPixelsCancelSelected;
      skin::SkinPartPtr m_warningBox;
      skin::SkinPartPtr m_canvasNw;
      skin::SkinPartPtr m_canvasN;
      skin::SkinPartPtr m_canvasNe;
      skin::SkinPartPtr m_canvasW;
      skin::SkinPartPtr m_canvasC;
      skin::SkinPartPtr m_canvasE;
      skin::SkinPartPtr m_canvasSw;
      skin::SkinPartPtr m_canvasS;
      skin::SkinPartPtr m_canvasSe;
      skin::SkinPartPtr m_canvasEmpty;
      skin::SkinPartPtr m_inkSimple;
      skin::SkinPartPtr m_inkAlphaCompositing;
      skin::SkinPartPtr m_inkCopyColor;
      skin::SkinPartPtr m_inkLockAlpha;
      skin::SkinPartPtr m_inkShading;
      skin::SkinPartPtr m_selectionOpaque;
      skin::SkinPartPtr m_selectionMasked;
      skin::SkinPartPtr m_pivotNorthwest;
      skin::SkinPartPtr m_pivotNorth;
      skin::SkinPartPtr m_pivotNortheast;
      skin::SkinPartPtr m_pivotWest;
      skin::SkinPartPtr m_pivotCenter;
      skin::SkinPartPtr m_pivotEast;
      skin::SkinPartPtr m_pivotSouthwest;
      skin::SkinPartPtr m_pivotSouth;
      skin::SkinPartPtr m_pivotSoutheast;
      skin::SkinPartPtr m_iconRgb;
      skin::SkinPartPtr m_iconGrayscale;
      skin::SkinPartPtr m_iconIndexed;
      skin::SkinPartPtr m_iconBlack;
      skin::SkinPartPtr m_iconWhite;
      skin::SkinPartPtr m_iconTransparent;
      skin::SkinPartPtr m_colorWheelIndicator;
      skin::SkinPartPtr m_noSymmetry;
      skin::SkinPartPtr m_horizontalSymmetry;
      skin::SkinPartPtr m_verticalSymmetry;
      skin::SkinPartPtr m_iconArrowDown;
      skin::SkinPartPtr m_iconClose;
      skin::SkinPartPtr m_iconSearch;
      skin::SkinPartPtr m_iconUserData;
      skin::SkinPartPtr m_iconPos;
      skin::SkinPartPtr m_iconSize;
      skin::SkinPartPtr m_iconSelsize;
      skin::SkinPartPtr m_iconFrame;
      skin::SkinPartPtr m_iconClock;
      skin::SkinPartPtr m_iconStart;
      skin::SkinPartPtr m_iconEnd;
      skin::SkinPartPtr m_iconAngle;
      skin::SkinPartPtr m_iconKey;
      skin::SkinPartPtr m_iconDistance;
      skin::SkinPartPtr m_iconGrid;
      skin::SkinPartPtr m_iconSave;
      skin::SkinPartPtr m_iconSaveSmall;
      skin::SkinPartPtr m_iconSlice;
      skin::SkinPartPtr m_toolRectangularMarquee;
      skin::SkinPartPtr m_toolEllipticalMarquee;
      skin::SkinPartPtr m_toolLasso;
      skin::SkinPartPtr m_toolPolygonalLasso;
      skin::SkinPartPtr m_toolMagicWand;
      skin::SkinPartPtr m_toolPencil;
      skin::SkinPartPtr m_toolSpray;
      skin::SkinPartPtr m_toolEraser;
      skin::SkinPartPtr m_toolEyedropper;
      skin::SkinPartPtr m_toolHand;
      skin::SkinPartPtr m_toolMove;
      skin::SkinPartPtr m_toolZoom;
      skin::SkinPartPtr m_toolSlice;
      skin::SkinPartPtr m_toolPaintBucket;
      skin::SkinPartPtr m_toolGradient;
      skin::SkinPartPtr m_toolLine;
      skin::SkinPartPtr m_toolCurve;
      skin::SkinPartPtr m_toolRectangle;
      skin::SkinPartPtr m_toolFilledRectangle;
      skin::SkinPartPtr m_toolEllipse;
      skin::SkinPartPtr m_toolFilledEllipse;
      skin::SkinPartPtr m_toolContour;
      skin::SkinPartPtr m_toolPolygon;
      skin::SkinPartPtr m_toolBlur;
      skin::SkinPartPtr m_toolJumble;
      skin::SkinPartPtr m_toolConfiguration;
      skin::SkinPartPtr m_toolMinieditor;
      skin::SkinPartPtr m_simpleColorBorder;
      skin::SkinPartPtr m_simpleColorSelected;
      skin::SkinPartPtr m_asepriteFace;
      skin::SkinPartPtr m_asepriteFaceMouse;
      skin::SkinPartPtr m_asepriteFacePushed;
    };
    class Cursors {
      template<typename> friend class ThemeFile;
    public:
      const ui::Cursor* normal() const { return m_normal; }
      const ui::Cursor* normalAdd() const { return m_normalAdd; }
      const ui::Cursor* crosshair() const { return m_crosshair; }
      const ui::Cursor* forbidden() const { return m_forbidden; }
      const ui::Cursor* hand() const { return m_hand; }
      const ui::Cursor* scroll() const { return m_scroll; }
      const ui::Cursor* move() const { return m_move; }
      const ui::Cursor* moveSelection() const { return m_moveSelection; }
      const ui::Cursor* sizeNs() const { return m_sizeNs; }
      const ui::Cursor* sizeWe() const { return m_sizeWe; }
      const ui::Cursor* sizeN() const { return m_sizeN; }
      const ui::Cursor* sizeNe() const { return m_sizeNe; }
      const ui::Cursor* sizeE() const { return m_sizeE; }
      const ui::Cursor* sizeSe() const { return m_sizeSe; }
      const ui::Cursor* sizeS() const { return m_sizeS; }
      const ui::Cursor* sizeSw() const { return m_sizeSw; }
      const ui::Cursor* sizeW() const { return m_sizeW; }
      const ui::Cursor* sizeNw() const { return m_sizeNw; }
      const ui::Cursor* rotateN() const { return m_rotateN; }
      const ui::Cursor* rotateNe() const { return m_rotateNe; }
      const ui::Cursor* rotateE() const { return m_rotateE; }
      const ui::Cursor* rotateSe() const { return m_rotateSe; }
      const ui::Cursor* rotateS() const { return m_rotateS; }
      const ui::Cursor* rotateSw() const { return m_rotateSw; }
      const ui::Cursor* rotateW() const { return m_rotateW; }
      const ui::Cursor* rotateNw() const { return m_rotateNw; }
      const ui::Cursor* eyedropper() const { return m_eyedropper; }
      const ui::Cursor* magnifier() const { return m_magnifier; }
    private:
      ui::Cursor* m_normal;
      ui::Cursor* m_normalAdd;
      ui::Cursor* m_crosshair;
      ui::Cursor* m_forbidden;
      ui::Cursor* m_hand;
      ui::Cursor* m_scroll;
      ui::Cursor* m_move;
      ui::Cursor* m_moveSelection;
      ui::Cursor* m_sizeNs;
      ui::Cursor* m_sizeWe;
      ui::Cursor* m_sizeN;
      ui::Cursor* m_sizeNe;
      ui::Cursor* m_sizeE;
      ui::Cursor* m_sizeSe;
      ui::Cursor* m_sizeS;
      ui::Cursor* m_sizeSw;
      ui::Cursor* m_sizeW;
      ui::Cursor* m_sizeNw;
      ui::Cursor* m_rotateN;
      ui::Cursor* m_rotateNe;
      ui::Cursor* m_rotateE;
      ui::Cursor* m_rotateSe;
      ui::Cursor* m_rotateS;
      ui::Cursor* m_rotateSw;
      ui::Cursor* m_rotateW;
      ui::Cursor* m_rotateNw;
      ui::Cursor* m_eyedropper;
      ui::Cursor* m_magnifier;
    };

    class Styles {
      template<typename> friend class ThemeFile;
    public:
      ui::Style* box() const { return m_box; }
      ui::Style* grid() const { return m_grid; }
      ui::Style* windowWithoutTitle() const { return m_windowWithoutTitle; }
      ui::Style* windowWithTitle() const { return m_windowWithTitle; }
      ui::Style* windowTitleLabel() const { return m_windowTitleLabel; }
      ui::Style* windowButton() const { return m_windowButton; }
      ui::Style* windowCloseButton() const { return m_windowCloseButton; }
      ui::Style* windowCenterButton() const { return m_windowCenterButton; }
      ui::Style* windowPlayButton() const { return m_windowPlayButton; }
      ui::Style* windowStopButton() const { return m_windowStopButton; }
      ui::Style* popupWindow() const { return m_popupWindow; }
      ui::Style* transparentPopupWindow() const { return m_transparentPopupWindow; }
      ui::Style* menu() const { return m_menu; }
      ui::Style* menubox() const { return m_menubox; }
      ui::Style* menubar() const { return m_menubar; }
      ui::Style* desktop() const { return m_desktop; }
      ui::Style* tooltipWindow() const { return m_tooltipWindow; }
      ui::Style* tooltipWindowArrow() const { return m_tooltipWindowArrow; }
      ui::Style* tooltipFace() const { return m_tooltipFace; }
      ui::Style* tooltipText() const { return m_tooltipText; }
      ui::Style* label() const { return m_label; }
      ui::Style* miniLabel() const { return m_miniLabel; }
      ui::Style* listHeaderLabel() const { return m_listHeaderLabel; }
      ui::Style* link() const { return m_link; }
      ui::Style* browserLink() const { return m_browserLink; }
      ui::Style* workspaceLabel() const { return m_workspaceLabel; }
      ui::Style* workspaceLink() const { return m_workspaceLink; }
      ui::Style* workspaceUpdateLink() const { return m_workspaceUpdateLink; }
      ui::Style* view() const { return m_view; }
      ui::Style* editorView() const { return m_editorView; }
      ui::Style* workspaceView() const { return m_workspaceView; }
      ui::Style* colorbarView() const { return m_colorbarView; }
      ui::Style* topShadeView() const { return m_topShadeView; }
      ui::Style* normalShadeView() const { return m_normalShadeView; }
      ui::Style* menuShadeView() const { return m_menuShadeView; }
      ui::Style* button() const { return m_button; }
      ui::Style* checkBox() const { return m_checkBox; }
      ui::Style* radioButton() const { return m_radioButton; }
      ui::Style* miniButton() const { return m_miniButton; }
      ui::Style* miniCheckBox() const { return m_miniCheckBox; }
      ui::Style* comboboxButton() const { return m_comboboxButton; }
      ui::Style* dropDownButton() const { return m_dropDownButton; }
      ui::Style* dropDownExpandButton() const { return m_dropDownExpandButton; }
      ui::Style* goBackButton() const { return m_goBackButton; }
      ui::Style* goForwardButton() const { return m_goForwardButton; }
      ui::Style* goUpButton() const { return m_goUpButton; }
      ui::Style* newFolderButton() const { return m_newFolderButton; }
      ui::Style* colorWheelOptions() const { return m_colorWheelOptions; }
      ui::Style* recoverSpritesButton() const { return m_recoverSpritesButton; }
      ui::Style* newFrameButton() const { return m_newFrameButton; }
      ui::Style* colorButton() const { return m_colorButton; }
      ui::Style* splitter() const { return m_splitter; }
      ui::Style* workspaceSplitter() const { return m_workspaceSplitter; }
      ui::Style* horizontalSeparator() const { return m_horizontalSeparator; }
      ui::Style* menuSeparator() const { return m_menuSeparator; }
      ui::Style* separatorInView() const { return m_separatorInView; }
      ui::Style* verticalSeparator() const { return m_verticalSeparator; }
      ui::Style* recentItem() const { return m_recentItem; }
      ui::Style* recentFile() const { return m_recentFile; }
      ui::Style* recentFileDetail() const { return m_recentFileDetail; }
      ui::Style* newsItem() const { return m_newsItem; }
      ui::Style* newsItemDetail() const { return m_newsItemDetail; }
      ui::Style* scrollbar() const { return m_scrollbar; }
      ui::Style* scrollbarThumb() const { return m_scrollbarThumb; }
      ui::Style* miniScrollbar() const { return m_miniScrollbar; }
      ui::Style* miniScrollbarThumb() const { return m_miniScrollbarThumb; }
      ui::Style* transparentScrollbar() const { return m_transparentScrollbar; }
      ui::Style* transparentScrollbarThumb() const { return m_transparentScrollbarThumb; }
      ui::Style* mainTabs() const { return m_mainTabs; }
      ui::Style* workspaceTabs() const { return m_workspaceTabs; }
      ui::Style* tab() const { return m_tab; }
      ui::Style* tabText() const { return m_tabText; }
      ui::Style* tabBottom() const { return m_tabBottom; }
      ui::Style* tabFiller() const { return m_tabFiller; }
      ui::Style* tabIcon() const { return m_tabIcon; }
      ui::Style* tabCloseIcon() const { return m_tabCloseIcon; }
      ui::Style* tabModifiedIcon() const { return m_tabModifiedIcon; }
      ui::Style* tabHome() const { return m_tabHome; }
      ui::Style* flag() const { return m_flag; }
      ui::Style* warningBox() const { return m_warningBox; }
      ui::Style* timeline() const { return m_timeline; }
      ui::Style* timelineBox() const { return m_timelineBox; }
      ui::Style* timelineOpenEye() const { return m_timelineOpenEye; }
      ui::Style* timelineClosedEye() const { return m_timelineClosedEye; }
      ui::Style* timelineOpenPadlock() const { return m_timelineOpenPadlock; }
      ui::Style* timelineClosedPadlock() const { return m_timelineClosedPadlock; }
      ui::Style* timelineContinuous() const { return m_timelineContinuous; }
      ui::Style* timelineDiscontinuous() const { return m_timelineDiscontinuous; }
      ui::Style* timelineClosedGroup() const { return m_timelineClosedGroup; }
      ui::Style* timelineOpenGroup() const { return m_timelineOpenGroup; }
      ui::Style* timelineLayer() const { return m_timelineLayer; }
      ui::Style* timelineLayerTextOnly() const { return m_timelineLayerTextOnly; }
      ui::Style* timelineHeaderFrame() const { return m_timelineHeaderFrame; }
      ui::Style* timelineSelectedCel() const { return m_timelineSelectedCel; }
      ui::Style* timelineFocusedCel() const { return m_timelineFocusedCel; }
      ui::Style* timelineEmptyFrame() const { return m_timelineEmptyFrame; }
      ui::Style* timelineKeyframe() const { return m_timelineKeyframe; }
      ui::Style* timelineFromLeft() const { return m_timelineFromLeft; }
      ui::Style* timelineFromRight() const { return m_timelineFromRight; }
      ui::Style* timelineFromBoth() const { return m_timelineFromBoth; }
      ui::Style* timelineLeftLink() const { return m_timelineLeftLink; }
      ui::Style* timelineRightLink() const { return m_timelineRightLink; }
      ui::Style* timelineBothLinks() const { return m_timelineBothLinks; }
      ui::Style* timelineGear() const { return m_timelineGear; }
      ui::Style* timelineOnionskin() const { return m_timelineOnionskin; }
      ui::Style* timelineOnionskinRange() const { return m_timelineOnionskinRange; }
      ui::Style* timelinePadding() const { return m_timelinePadding; }
      ui::Style* timelinePaddingTr() const { return m_timelinePaddingTr; }
      ui::Style* timelinePaddingBl() const { return m_timelinePaddingBl; }
      ui::Style* timelinePaddingBr() const { return m_timelinePaddingBr; }
      ui::Style* timelineRangeOutline() const { return m_timelineRangeOutline; }
      ui::Style* timelineDropLayerDeco() const { return m_timelineDropLayerDeco; }
      ui::Style* timelineDropFrameDeco() const { return m_timelineDropFrameDeco; }
      ui::Style* timelineLoopRange() const { return m_timelineLoopRange; }
      ui::Style* timelineSwitchBandButton() const { return m_timelineSwitchBandButton; }
      ui::Style* shadeSelection() const { return m_shadeSelection; }
      ui::Style* colorbarSelection() const { return m_colorbarSelection; }
      ui::Style* simpleColor() const { return m_simpleColor; }
      ui::Style* listItem() const { return m_listItem; }
      ui::Style* asepriteFace() const { return m_asepriteFace; }
    private:
      ui::Style* m_box;
      ui::Style* m_grid;
      ui::Style* m_windowWithoutTitle;
      ui::Style* m_windowWithTitle;
      ui::Style* m_windowTitleLabel;
      ui::Style* m_windowButton;
      ui::Style* m_windowCloseButton;
      ui::Style* m_windowCenterButton;
      ui::Style* m_windowPlayButton;
      ui::Style* m_windowStopButton;
      ui::Style* m_popupWindow;
      ui::Style* m_transparentPopupWindow;
      ui::Style* m_menu;
      ui::Style* m_menubox;
      ui::Style* m_menubar;
      ui::Style* m_desktop;
      ui::Style* m_tooltipWindow;
      ui::Style* m_tooltipWindowArrow;
      ui::Style* m_tooltipFace;
      ui::Style* m_tooltipText;
      ui::Style* m_label;
      ui::Style* m_miniLabel;
      ui::Style* m_listHeaderLabel;
      ui::Style* m_link;
      ui::Style* m_browserLink;
      ui::Style* m_workspaceLabel;
      ui::Style* m_workspaceLink;
      ui::Style* m_workspaceUpdateLink;
      ui::Style* m_view;
      ui::Style* m_editorView;
      ui::Style* m_workspaceView;
      ui::Style* m_colorbarView;
      ui::Style* m_topShadeView;
      ui::Style* m_normalShadeView;
      ui::Style* m_menuShadeView;
      ui::Style* m_button;
      ui::Style* m_checkBox;
      ui::Style* m_radioButton;
      ui::Style* m_miniButton;
      ui::Style* m_miniCheckBox;
      ui::Style* m_comboboxButton;
      ui::Style* m_dropDownButton;
      ui::Style* m_dropDownExpandButton;
      ui::Style* m_goBackButton;
      ui::Style* m_goForwardButton;
      ui::Style* m_goUpButton;
      ui::Style* m_newFolderButton;
      ui::Style* m_colorWheelOptions;
      ui::Style* m_recoverSpritesButton;
      ui::Style* m_newFrameButton;
      ui::Style* m_colorButton;
      ui::Style* m_splitter;
      ui::Style* m_workspaceSplitter;
      ui::Style* m_horizontalSeparator;
      ui::Style* m_menuSeparator;
      ui::Style* m_separatorInView;
      ui::Style* m_verticalSeparator;
      ui::Style* m_recentItem;
      ui::Style* m_recentFile;
      ui::Style* m_recentFileDetail;
      ui::Style* m_newsItem;
      ui::Style* m_newsItemDetail;
      ui::Style* m_scrollbar;
      ui::Style* m_scrollbarThumb;
      ui::Style* m_miniScrollbar;
      ui::Style* m_miniScrollbarThumb;
      ui::Style* m_transparentScrollbar;
      ui::Style* m_transparentScrollbarThumb;
      ui::Style* m_mainTabs;
      ui::Style* m_workspaceTabs;
      ui::Style* m_tab;
      ui::Style* m_tabText;
      ui::Style* m_tabBottom;
      ui::Style* m_tabFiller;
      ui::Style* m_tabIcon;
      ui::Style* m_tabCloseIcon;
      ui::Style* m_tabModifiedIcon;
      ui::Style* m_tabHome;
      ui::Style* m_flag;
      ui::Style* m_warningBox;
      ui::Style* m_timeline;
      ui::Style* m_timelineBox;
      ui::Style* m_timelineOpenEye;
      ui::Style* m_timelineClosedEye;
      ui::Style* m_timelineOpenPadlock;
      ui::Style* m_timelineClosedPadlock;
      ui::Style* m_timelineContinuous;
      ui::Style* m_timelineDiscontinuous;
      ui::Style* m_timelineClosedGroup;
      ui::Style* m_timelineOpenGroup;
      ui::Style* m_timelineLayer;
      ui::Style* m_timelineLayerTextOnly;
      ui::Style* m_timelineHeaderFrame;
      ui::Style* m_timelineSelectedCel;
      ui::Style* m_timelineFocusedCel;
      ui::Style* m_timelineEmptyFrame;
      ui::Style* m_timelineKeyframe;
      ui::Style* m_timelineFromLeft;
      ui::Style* m_timelineFromRight;
      ui::Style* m_timelineFromBoth;
      ui::Style* m_timelineLeftLink;
      ui::Style* m_timelineRightLink;
      ui::Style* m_timelineBothLinks;
      ui::Style* m_timelineGear;
      ui::Style* m_timelineOnionskin;
      ui::Style* m_timelineOnionskinRange;
      ui::Style* m_timelinePadding;
      ui::Style* m_timelinePaddingTr;
      ui::Style* m_timelinePaddingBl;
      ui::Style* m_timelinePaddingBr;
      ui::Style* m_timelineRangeOutline;
      ui::Style* m_timelineDropLayerDeco;
      ui::Style* m_timelineDropFrameDeco;
      ui::Style* m_timelineLoopRange;
      ui::Style* m_timelineSwitchBandButton;
      ui::Style* m_shadeSelection;
      ui::Style* m_colorbarSelection;
      ui::Style* m_simpleColor;
      ui::Style* m_listItem;
      ui::Style* m_asepriteFace;
    };

    Dimensions dimensions;
    Colors colors;
    Parts parts;
    Cursors cursors;
    Styles styles;

  protected:
    void updateInternals() {
      byId(dimensions.m_scrollbarSize, "scrollbar_size");
      byId(dimensions.m_miniScrollbarSize, "mini_scrollbar_size");
      byId(dimensions.m_tabsWidth, "tabs_width");
      byId(dimensions.m_tabsHeight, "tabs_height");
      byId(dimensions.m_tabsBottomHeight, "tabs_bottom_height");
      byId(dimensions.m_dockedTabsHeight, "docked_tabs_height");
      byId(dimensions.m_tabsCloseIconWidth, "tabs_close_icon_width");
      byId(dimensions.m_tabsCloseIconHeight, "tabs_close_icon_height");
      byId(dimensions.m_tabsIconWidth, "tabs_icon_width");
      byId(dimensions.m_timelineTopBorder, "timeline_top_border");
      byId(dimensions.m_timelineTagsAreaHeight, "timeline_tags_area_height");
      byId(dimensions.m_timelineOutlineWidth, "timeline_outline_width");
      byId(dimensions.m_paletteOutlineWidth, "palette_outline_width");
      byId(dimensions.m_colorSliderHeight, "color_slider_height");
      byId(dimensions.m_timelineBaseSize, "timeline_base_size");
      byId(colors.m_text, "text");
      byId(colors.m_disabled, "disabled");
      byId(colors.m_face, "face");
      byId(colors.m_hotFace, "hot_face");
      byId(colors.m_selected, "selected");
      byId(colors.m_selectedText, "selected_text");
      byId(colors.m_separatorLabel, "separator_label");
      byId(colors.m_background, "background");
      byId(colors.m_textboxText, "textbox_text");
      byId(colors.m_textboxFace, "textbox_face");
      byId(colors.m_textboxCodeFace, "textbox_code_face");
      byId(colors.m_entrySuffix, "entry_suffix");
      byId(colors.m_linkText, "link_text");
      byId(colors.m_linkHover, "link_hover");
      byId(colors.m_buttonNormalText, "button_normal_text");
      byId(colors.m_buttonHotText, "button_hot_text");
      byId(colors.m_buttonSelectedText, "button_selected_text");
      byId(colors.m_checkHotFace, "check_hot_face");
      byId(colors.m_checkFocusFace, "check_focus_face");
      byId(colors.m_radioHotFace, "radio_hot_face");
      byId(colors.m_radioFocusFace, "radio_focus_face");
      byId(colors.m_menuitemNormalText, "menuitem_normal_text");
      byId(colors.m_menuitemNormalFace, "menuitem_normal_face");
      byId(colors.m_menuitemHotText, "menuitem_hot_text");
      byId(colors.m_menuitemHotFace, "menuitem_hot_face");
      byId(colors.m_menuitemHighlightText, "menuitem_highlight_text");
      byId(colors.m_menuitemHighlightFace, "menuitem_highlight_face");
      byId(colors.m_windowFace, "window_face");
      byId(colors.m_windowTitlebarText, "window_titlebar_text");
      byId(colors.m_windowTitlebarFace, "window_titlebar_face");
      byId(colors.m_editorFace, "editor_face");
      byId(colors.m_editorSpriteBorder, "editor_sprite_border");
      byId(colors.m_editorSpriteBottomBorder, "editor_sprite_bottom_border");
      byId(colors.m_editorViewFace, "editor_view_face");
      byId(colors.m_listitemNormalText, "listitem_normal_text");
      byId(colors.m_listitemNormalFace, "listitem_normal_face");
      byId(colors.m_listitemSelectedText, "listitem_selected_text");
      byId(colors.m_listitemSelectedFace, "listitem_selected_face");
      byId(colors.m_sliderEmptyText, "slider_empty_text");
      byId(colors.m_sliderFullText, "slider_full_text");
      byId(colors.m_tabNormalText, "tab_normal_text");
      byId(colors.m_tabActiveText, "tab_active_text");
      byId(colors.m_tabActiveFace, "tab_active_face");
      byId(colors.m_popupWindowBorder, "popup_window_border");
      byId(colors.m_tooltipText, "tooltip_text");
      byId(colors.m_tooltipFace, "tooltip_face");
      byId(colors.m_filelistEvenRowText, "filelist_even_row_text");
      byId(colors.m_filelistEvenRowFace, "filelist_even_row_face");
      byId(colors.m_filelistOddRowText, "filelist_odd_row_text");
      byId(colors.m_filelistOddRowFace, "filelist_odd_row_face");
      byId(colors.m_filelistSelectedRowText, "filelist_selected_row_text");
      byId(colors.m_filelistSelectedRowFace, "filelist_selected_row_face");
      byId(colors.m_filelistDisabledRowText, "filelist_disabled_row_text");
      byId(colors.m_workspace, "workspace");
      byId(colors.m_workspaceText, "workspace_text");
      byId(colors.m_workspaceLink, "workspace_link");
      byId(colors.m_workspaceLinkHover, "workspace_link_hover");
      byId(colors.m_timelineNormal, "timeline_normal");
      byId(colors.m_timelineNormalText, "timeline_normal_text");
      byId(colors.m_timelineHover, "timeline_hover");
      byId(colors.m_timelineHoverText, "timeline_hover_text");
      byId(colors.m_timelineActive, "timeline_active");
      byId(colors.m_timelineActiveText, "timeline_active_text");
      byId(colors.m_timelineActiveHover, "timeline_active_hover");
      byId(colors.m_timelineActiveHoverText, "timeline_active_hover_text");
      byId(colors.m_timelineClicked, "timeline_clicked");
      byId(colors.m_timelineClickedText, "timeline_clicked_text");
      byId(colors.m_timelineFocusedText, "timeline_focused_text");
      byId(colors.m_timelinePadding, "timeline_padding");
      byId(colors.m_timelineBandHighlight, "timeline_band_highlight");
      byId(colors.m_timelineBandBg, "timeline_band_bg");
      byId(colors.m_statusBarText, "status_bar_text");
      byId(colors.m_statusBarFace, "status_bar_face");
      byId(colors.m_flagNormal, "flag_normal");
      byId(colors.m_flagActive, "flag_active");
      byId(colors.m_flagClicked, "flag_clicked");
      byId(colors.m_selectBoxRuler, "select_box_ruler");
      byId(colors.m_selectBoxGrid, "select_box_grid");
      byId(colors.m_editPalFace, "edit_pal_face");
      byId(parts.m_radioNormal, "radio_normal");
      byId(parts.m_radioSelected, "radio_selected");
      byId(parts.m_radioDisabled, "radio_disabled");
      byId(parts.m_checkNormal, "check_normal");
      byId(parts.m_checkSelected, "check_selected");
      byId(parts.m_checkDisabled, "check_disabled");
      byId(parts.m_checkFocus, "check_focus");
      byId(parts.m_radioFocus, "radio_focus");
      byId(parts.m_buttonNormal, "button_normal");
      byId(parts.m_buttonHot, "button_hot");
      byId(parts.m_buttonFocused, "button_focused");
      byId(parts.m_buttonSelected, "button_selected");
      byId(parts.m_sunkenNormal, "sunken_normal");
      byId(parts.m_sunkenFocused, "sunken_focused");
      byId(parts.m_sunken2Normal, "sunken2_normal");
      byId(parts.m_sunken2Focused, "sunken2_focused");
      byId(parts.m_sunkenMiniNormal, "sunken_mini_normal");
      byId(parts.m_sunkenMiniFocused, "sunken_mini_focused");
      byId(parts.m_window, "window");
      byId(parts.m_menu, "menu");
      byId(parts.m_windowButtonNormal, "window_button_normal");
      byId(parts.m_windowButtonHot, "window_button_hot");
      byId(parts.m_windowButtonSelected, "window_button_selected");
      byId(parts.m_windowCloseIcon, "window_close_icon");
      byId(parts.m_windowPlayIcon, "window_play_icon");
      byId(parts.m_windowStopIcon, "window_stop_icon");
      byId(parts.m_windowCenterIcon, "window_center_icon");
      byId(parts.m_sliderFull, "slider_full");
      byId(parts.m_sliderEmpty, "slider_empty");
      byId(parts.m_sliderFullFocused, "slider_full_focused");
      byId(parts.m_sliderEmptyFocused, "slider_empty_focused");
      byId(parts.m_miniSliderFull, "mini_slider_full");
      byId(parts.m_miniSliderEmpty, "mini_slider_empty");
      byId(parts.m_miniSliderFullFocused, "mini_slider_full_focused");
      byId(parts.m_miniSliderEmptyFocused, "mini_slider_empty_focused");
      byId(parts.m_miniSliderThumb, "mini_slider_thumb");
      byId(parts.m_miniSliderThumbFocused, "mini_slider_thumb_focused");
      byId(parts.m_separatorHorz, "separator_horz");
      byId(parts.m_separatorVert, "separator_vert");
      byId(parts.m_comboboxArrowDown, "combobox_arrow_down");
      byId(parts.m_comboboxArrowDownSelected, "combobox_arrow_down_selected");
      byId(parts.m_comboboxArrowDownDisabled, "combobox_arrow_down_disabled");
      byId(parts.m_comboboxArrowUp, "combobox_arrow_up");
      byId(parts.m_comboboxArrowUpSelected, "combobox_arrow_up_selected");
      byId(parts.m_comboboxArrowUpDisabled, "combobox_arrow_up_disabled");
      byId(parts.m_comboboxArrowLeft, "combobox_arrow_left");
      byId(parts.m_comboboxArrowLeftSelected, "combobox_arrow_left_selected");
      byId(parts.m_comboboxArrowLeftDisabled, "combobox_arrow_left_disabled");
      byId(parts.m_comboboxArrowRight, "combobox_arrow_right");
      byId(parts.m_comboboxArrowRightSelected, "combobox_arrow_right_selected");
      byId(parts.m_comboboxArrowRightDisabled, "combobox_arrow_right_disabled");
      byId(parts.m_newfolder, "newfolder");
      byId(parts.m_newfolderSelected, "newfolder_selected");
      byId(parts.m_toolbuttonNormal, "toolbutton_normal");
      byId(parts.m_toolbuttonHot, "toolbutton_hot");
      byId(parts.m_toolbuttonLast, "toolbutton_last");
      byId(parts.m_toolbuttonPushed, "toolbutton_pushed");
      byId(parts.m_buttonsetItemNormal, "buttonset_item_normal");
      byId(parts.m_buttonsetItemHot, "buttonset_item_hot");
      byId(parts.m_buttonsetItemHotFocused, "buttonset_item_hot_focused");
      byId(parts.m_buttonsetItemFocused, "buttonset_item_focused");
      byId(parts.m_buttonsetItemPushed, "buttonset_item_pushed");
      byId(parts.m_tabNormal, "tab_normal");
      byId(parts.m_tabActive, "tab_active");
      byId(parts.m_tabBottomActive, "tab_bottom_active");
      byId(parts.m_tabBottomNormal, "tab_bottom_normal");
      byId(parts.m_tabFiller, "tab_filler");
      byId(parts.m_tabModifiedIconNormal, "tab_modified_icon_normal");
      byId(parts.m_tabModifiedIconActive, "tab_modified_icon_active");
      byId(parts.m_tabCloseIconNormal, "tab_close_icon_normal");
      byId(parts.m_tabCloseIconActive, "tab_close_icon_active");
      byId(parts.m_tabIconBgClicked, "tab_icon_bg_clicked");
      byId(parts.m_tabIconBgHover, "tab_icon_bg_hover");
      byId(parts.m_tabHomeIconNormal, "tab_home_icon_normal");
      byId(parts.m_tabHomeIconActive, "tab_home_icon_active");
      byId(parts.m_editorNormal, "editor_normal");
      byId(parts.m_editorSelected, "editor_selected");
      byId(parts.m_colorbar0, "colorbar_0");
      byId(parts.m_colorbar1, "colorbar_1");
      byId(parts.m_colorbar2, "colorbar_2");
      byId(parts.m_colorbar3, "colorbar_3");
      byId(parts.m_colorbarSelectionHot, "colorbar_selection_hot");
      byId(parts.m_colorbarSelection, "colorbar_selection");
      byId(parts.m_scrollbarBg, "scrollbar_bg");
      byId(parts.m_scrollbarThumb, "scrollbar_thumb");
      byId(parts.m_miniScrollbarBg, "mini_scrollbar_bg");
      byId(parts.m_miniScrollbarThumb, "mini_scrollbar_thumb");
      byId(parts.m_miniScrollbarBgHot, "mini_scrollbar_bg_hot");
      byId(parts.m_miniScrollbarThumbHot, "mini_scrollbar_thumb_hot");
      byId(parts.m_transparentScrollbarBg, "transparent_scrollbar_bg");
      byId(parts.m_transparentScrollbarThumb, "transparent_scrollbar_thumb");
      byId(parts.m_transparentScrollbarBgHot, "transparent_scrollbar_bg_hot");
      byId(parts.m_transparentScrollbarThumbHot, "transparent_scrollbar_thumb_hot");
      byId(parts.m_tooltip, "tooltip");
      byId(parts.m_tooltipArrow, "tooltip_arrow");
      byId(parts.m_aniFirst, "ani_first");
      byId(parts.m_aniPrevious, "ani_previous");
      byId(parts.m_aniPlay, "ani_play");
      byId(parts.m_aniStop, "ani_stop");
      byId(parts.m_aniNext, "ani_next");
      byId(parts.m_aniLast, "ani_last");
      byId(parts.m_palEdit, "pal_edit");
      byId(parts.m_palSort, "pal_sort");
      byId(parts.m_palPresets, "pal_presets");
      byId(parts.m_palOptions, "pal_options");
      byId(parts.m_palResize, "pal_resize");
      byId(parts.m_selectionReplace, "selection_replace");
      byId(parts.m_selectionAdd, "selection_add");
      byId(parts.m_selectionSubtract, "selection_subtract");
      byId(parts.m_unpinned, "unpinned");
      byId(parts.m_pinned, "pinned");
      byId(parts.m_dropDownButtonLeftNormal, "drop_down_button_left_normal");
      byId(parts.m_dropDownButtonLeftHot, "drop_down_button_left_hot");
      byId(parts.m_dropDownButtonLeftFocused, "drop_down_button_left_focused");
      byId(parts.m_dropDownButtonLeftSelected, "drop_down_button_left_selected");
      byId(parts.m_dropDownButtonRightNormal, "drop_down_button_right_normal");
      byId(parts.m_dropDownButtonRightHot, "drop_down_button_right_hot");
      byId(parts.m_dropDownButtonRightFocused, "drop_down_button_right_focused");
      byId(parts.m_dropDownButtonRightSelected, "drop_down_button_right_selected");
      byId(parts.m_transformationHandle, "transformation_handle");
      byId(parts.m_pivotHandle, "pivot_handle");
      byId(parts.m_timelineNone, "timeline_none");
      byId(parts.m_timelineNormal, "timeline_normal");
      byId(parts.m_timelineActive, "timeline_active");
      byId(parts.m_timelineHover, "timeline_hover");
      byId(parts.m_timelineActiveHover, "timeline_active_hover");
      byId(parts.m_timelineClicked, "timeline_clicked");
      byId(parts.m_timelineOpenEyeNormal, "timeline_open_eye_normal");
      byId(parts.m_timelineOpenEyeActive, "timeline_open_eye_active");
      byId(parts.m_timelineClosedEyeNormal, "timeline_closed_eye_normal");
      byId(parts.m_timelineClosedEyeActive, "timeline_closed_eye_active");
      byId(parts.m_timelineOpenPadlockNormal, "timeline_open_padlock_normal");
      byId(parts.m_timelineOpenPadlockActive, "timeline_open_padlock_active");
      byId(parts.m_timelineClosedPadlockNormal, "timeline_closed_padlock_normal");
      byId(parts.m_timelineClosedPadlockActive, "timeline_closed_padlock_active");
      byId(parts.m_timelineContinuousNormal, "timeline_continuous_normal");
      byId(parts.m_timelineContinuousActive, "timeline_continuous_active");
      byId(parts.m_timelineDiscontinuousNormal, "timeline_discontinuous_normal");
      byId(parts.m_timelineDiscontinuousActive, "timeline_discontinuous_active");
      byId(parts.m_timelineClosedGroupNormal, "timeline_closed_group_normal");
      byId(parts.m_timelineClosedGroupActive, "timeline_closed_group_active");
      byId(parts.m_timelineOpenGroupNormal, "timeline_open_group_normal");
      byId(parts.m_timelineOpenGroupActive, "timeline_open_group_active");
      byId(parts.m_timelineEmptyFrameNormal, "timeline_empty_frame_normal");
      byId(parts.m_timelineEmptyFrameActive, "timeline_empty_frame_active");
      byId(parts.m_timelineKeyframeNormal, "timeline_keyframe_normal");
      byId(parts.m_timelineKeyframeActive, "timeline_keyframe_active");
      byId(parts.m_timelineFromLeftNormal, "timeline_from_left_normal");
      byId(parts.m_timelineFromLeftActive, "timeline_from_left_active");
      byId(parts.m_timelineFromRightNormal, "timeline_from_right_normal");
      byId(parts.m_timelineFromRightActive, "timeline_from_right_active");
      byId(parts.m_timelineFromBothNormal, "timeline_from_both_normal");
      byId(parts.m_timelineFromBothActive, "timeline_from_both_active");
      byId(parts.m_timelineLeftLinkActive, "timeline_left_link_active");
      byId(parts.m_timelineBothLinksActive, "timeline_both_links_active");
      byId(parts.m_timelineRightLinkActive, "timeline_right_link_active");
      byId(parts.m_timelineGear, "timeline_gear");
      byId(parts.m_timelineGearActive, "timeline_gear_active");
      byId(parts.m_timelineOnionskin, "timeline_onionskin");
      byId(parts.m_timelineOnionskinActive, "timeline_onionskin_active");
      byId(parts.m_timelineOnionskinRange, "timeline_onionskin_range");
      byId(parts.m_timelinePadding, "timeline_padding");
      byId(parts.m_timelinePaddingTr, "timeline_padding_tr");
      byId(parts.m_timelinePaddingBl, "timeline_padding_bl");
      byId(parts.m_timelinePaddingBr, "timeline_padding_br");
      byId(parts.m_timelineDropLayerDeco, "timeline_drop_layer_deco");
      byId(parts.m_timelineDropFrameDeco, "timeline_drop_frame_deco");
      byId(parts.m_timelineLoopRange, "timeline_loop_range");
      byId(parts.m_timelineFocused, "timeline_focused");
      byId(parts.m_flagNormal, "flag_normal");
      byId(parts.m_flagHighlight, "flag_highlight");
      byId(parts.m_dropPixelsOk, "drop_pixels_ok");
      byId(parts.m_dropPixelsOkSelected, "drop_pixels_ok_selected");
      byId(parts.m_dropPixelsCancel, "drop_pixels_cancel");
      byId(parts.m_dropPixelsCancelSelected, "drop_pixels_cancel_selected");
      byId(parts.m_warningBox, "warning_box");
      byId(parts.m_canvasNw, "canvas_nw");
      byId(parts.m_canvasN, "canvas_n");
      byId(parts.m_canvasNe, "canvas_ne");
      byId(parts.m_canvasW, "canvas_w");
      byId(parts.m_canvasC, "canvas_c");
      byId(parts.m_canvasE, "canvas_e");
      byId(parts.m_canvasSw, "canvas_sw");
      byId(parts.m_canvasS, "canvas_s");
      byId(parts.m_canvasSe, "canvas_se");
      byId(parts.m_canvasEmpty, "canvas_empty");
      byId(parts.m_inkSimple, "ink_simple");
      byId(parts.m_inkAlphaCompositing, "ink_alpha_compositing");
      byId(parts.m_inkCopyColor, "ink_copy_color");
      byId(parts.m_inkLockAlpha, "ink_lock_alpha");
      byId(parts.m_inkShading, "ink_shading");
      byId(parts.m_selectionOpaque, "selection_opaque");
      byId(parts.m_selectionMasked, "selection_masked");
      byId(parts.m_pivotNorthwest, "pivot_northwest");
      byId(parts.m_pivotNorth, "pivot_north");
      byId(parts.m_pivotNortheast, "pivot_northeast");
      byId(parts.m_pivotWest, "pivot_west");
      byId(parts.m_pivotCenter, "pivot_center");
      byId(parts.m_pivotEast, "pivot_east");
      byId(parts.m_pivotSouthwest, "pivot_southwest");
      byId(parts.m_pivotSouth, "pivot_south");
      byId(parts.m_pivotSoutheast, "pivot_southeast");
      byId(parts.m_iconRgb, "icon_rgb");
      byId(parts.m_iconGrayscale, "icon_grayscale");
      byId(parts.m_iconIndexed, "icon_indexed");
      byId(parts.m_iconBlack, "icon_black");
      byId(parts.m_iconWhite, "icon_white");
      byId(parts.m_iconTransparent, "icon_transparent");
      byId(parts.m_colorWheelIndicator, "color_wheel_indicator");
      byId(parts.m_noSymmetry, "no_symmetry");
      byId(parts.m_horizontalSymmetry, "horizontal_symmetry");
      byId(parts.m_verticalSymmetry, "vertical_symmetry");
      byId(parts.m_iconArrowDown, "icon_arrow_down");
      byId(parts.m_iconClose, "icon_close");
      byId(parts.m_iconSearch, "icon_search");
      byId(parts.m_iconUserData, "icon_user_data");
      byId(parts.m_iconPos, "icon_pos");
      byId(parts.m_iconSize, "icon_size");
      byId(parts.m_iconSelsize, "icon_selsize");
      byId(parts.m_iconFrame, "icon_frame");
      byId(parts.m_iconClock, "icon_clock");
      byId(parts.m_iconStart, "icon_start");
      byId(parts.m_iconEnd, "icon_end");
      byId(parts.m_iconAngle, "icon_angle");
      byId(parts.m_iconKey, "icon_key");
      byId(parts.m_iconDistance, "icon_distance");
      byId(parts.m_iconGrid, "icon_grid");
      byId(parts.m_iconSave, "icon_save");
      byId(parts.m_iconSaveSmall, "icon_save_small");
      byId(parts.m_iconSlice, "icon_slice");
      byId(parts.m_toolRectangularMarquee, "tool_rectangular_marquee");
      byId(parts.m_toolEllipticalMarquee, "tool_elliptical_marquee");
      byId(parts.m_toolLasso, "tool_lasso");
      byId(parts.m_toolPolygonalLasso, "tool_polygonal_lasso");
      byId(parts.m_toolMagicWand, "tool_magic_wand");
      byId(parts.m_toolPencil, "tool_pencil");
      byId(parts.m_toolSpray, "tool_spray");
      byId(parts.m_toolEraser, "tool_eraser");
      byId(parts.m_toolEyedropper, "tool_eyedropper");
      byId(parts.m_toolHand, "tool_hand");
      byId(parts.m_toolMove, "tool_move");
      byId(parts.m_toolZoom, "tool_zoom");
      byId(parts.m_toolSlice, "tool_slice");
      byId(parts.m_toolPaintBucket, "tool_paint_bucket");
      byId(parts.m_toolGradient, "tool_gradient");
      byId(parts.m_toolLine, "tool_line");
      byId(parts.m_toolCurve, "tool_curve");
      byId(parts.m_toolRectangle, "tool_rectangle");
      byId(parts.m_toolFilledRectangle, "tool_filled_rectangle");
      byId(parts.m_toolEllipse, "tool_ellipse");
      byId(parts.m_toolFilledEllipse, "tool_filled_ellipse");
      byId(parts.m_toolContour, "tool_contour");
      byId(parts.m_toolPolygon, "tool_polygon");
      byId(parts.m_toolBlur, "tool_blur");
      byId(parts.m_toolJumble, "tool_jumble");
      byId(parts.m_toolConfiguration, "tool_configuration");
      byId(parts.m_toolMinieditor, "tool_minieditor");
      byId(parts.m_simpleColorBorder, "simple_color_border");
      byId(parts.m_simpleColorSelected, "simple_color_selected");
      byId(parts.m_asepriteFace, "aseprite_face");
      byId(parts.m_asepriteFaceMouse, "aseprite_face_mouse");
      byId(parts.m_asepriteFacePushed, "aseprite_face_pushed");
      byId(cursors.m_normal, "normal");
      byId(cursors.m_normalAdd, "normal_add");
      byId(cursors.m_crosshair, "crosshair");
      byId(cursors.m_forbidden, "forbidden");
      byId(cursors.m_hand, "hand");
      byId(cursors.m_scroll, "scroll");
      byId(cursors.m_move, "move");
      byId(cursors.m_moveSelection, "move_selection");
      byId(cursors.m_sizeNs, "size_ns");
      byId(cursors.m_sizeWe, "size_we");
      byId(cursors.m_sizeN, "size_n");
      byId(cursors.m_sizeNe, "size_ne");
      byId(cursors.m_sizeE, "size_e");
      byId(cursors.m_sizeSe, "size_se");
      byId(cursors.m_sizeS, "size_s");
      byId(cursors.m_sizeSw, "size_sw");
      byId(cursors.m_sizeW, "size_w");
      byId(cursors.m_sizeNw, "size_nw");
      byId(cursors.m_rotateN, "rotate_n");
      byId(cursors.m_rotateNe, "rotate_ne");
      byId(cursors.m_rotateE, "rotate_e");
      byId(cursors.m_rotateSe, "rotate_se");
      byId(cursors.m_rotateS, "rotate_s");
      byId(cursors.m_rotateSw, "rotate_sw");
      byId(cursors.m_rotateW, "rotate_w");
      byId(cursors.m_rotateNw, "rotate_nw");
      byId(cursors.m_eyedropper, "eyedropper");
      byId(cursors.m_magnifier, "magnifier");
      byId(styles.m_box, "box");
      byId(styles.m_grid, "grid");
      byId(styles.m_windowWithoutTitle, "window_without_title");
      byId(styles.m_windowWithTitle, "window_with_title");
      byId(styles.m_windowTitleLabel, "window_title_label");
      byId(styles.m_windowButton, "window_button");
      byId(styles.m_windowCloseButton, "window_close_button");
      byId(styles.m_windowCenterButton, "window_center_button");
      byId(styles.m_windowPlayButton, "window_play_button");
      byId(styles.m_windowStopButton, "window_stop_button");
      byId(styles.m_popupWindow, "popup_window");
      byId(styles.m_transparentPopupWindow, "transparent_popup_window");
      byId(styles.m_menu, "menu");
      byId(styles.m_menubox, "menubox");
      byId(styles.m_menubar, "menubar");
      byId(styles.m_desktop, "desktop");
      byId(styles.m_tooltipWindow, "tooltip_window");
      byId(styles.m_tooltipWindowArrow, "tooltip_window_arrow");
      byId(styles.m_tooltipFace, "tooltip_face");
      byId(styles.m_tooltipText, "tooltip_text");
      byId(styles.m_label, "label");
      byId(styles.m_miniLabel, "mini_label");
      byId(styles.m_listHeaderLabel, "list_header_label");
      byId(styles.m_link, "link");
      byId(styles.m_browserLink, "browser_link");
      byId(styles.m_workspaceLabel, "workspace_label");
      byId(styles.m_workspaceLink, "workspace_link");
      byId(styles.m_workspaceUpdateLink, "workspace_update_link");
      byId(styles.m_view, "view");
      byId(styles.m_editorView, "editor_view");
      byId(styles.m_workspaceView, "workspace_view");
      byId(styles.m_colorbarView, "colorbar_view");
      byId(styles.m_topShadeView, "top_shade_view");
      byId(styles.m_normalShadeView, "normal_shade_view");
      byId(styles.m_menuShadeView, "menu_shade_view");
      byId(styles.m_button, "button");
      byId(styles.m_checkBox, "check_box");
      byId(styles.m_radioButton, "radio_button");
      byId(styles.m_miniButton, "mini_button");
      byId(styles.m_miniCheckBox, "mini_check_box");
      byId(styles.m_comboboxButton, "combobox_button");
      byId(styles.m_dropDownButton, "drop_down_button");
      byId(styles.m_dropDownExpandButton, "drop_down_expand_button");
      byId(styles.m_goBackButton, "go_back_button");
      byId(styles.m_goForwardButton, "go_forward_button");
      byId(styles.m_goUpButton, "go_up_button");
      byId(styles.m_newFolderButton, "new_folder_button");
      byId(styles.m_colorWheelOptions, "color_wheel_options");
      byId(styles.m_recoverSpritesButton, "recover_sprites_button");
      byId(styles.m_newFrameButton, "new_frame_button");
      byId(styles.m_colorButton, "color_button");
      byId(styles.m_splitter, "splitter");
      byId(styles.m_workspaceSplitter, "workspace_splitter");
      byId(styles.m_horizontalSeparator, "horizontal_separator");
      byId(styles.m_menuSeparator, "menu_separator");
      byId(styles.m_separatorInView, "separator_in_view");
      byId(styles.m_verticalSeparator, "vertical_separator");
      byId(styles.m_recentItem, "recent_item");
      byId(styles.m_recentFile, "recent_file");
      byId(styles.m_recentFileDetail, "recent_file_detail");
      byId(styles.m_newsItem, "news_item");
      byId(styles.m_newsItemDetail, "news_item_detail");
      byId(styles.m_scrollbar, "scrollbar");
      byId(styles.m_scrollbarThumb, "scrollbar_thumb");
      byId(styles.m_miniScrollbar, "mini_scrollbar");
      byId(styles.m_miniScrollbarThumb, "mini_scrollbar_thumb");
      byId(styles.m_transparentScrollbar, "transparent_scrollbar");
      byId(styles.m_transparentScrollbarThumb, "transparent_scrollbar_thumb");
      byId(styles.m_mainTabs, "main_tabs");
      byId(styles.m_workspaceTabs, "workspace_tabs");
      byId(styles.m_tab, "tab");
      byId(styles.m_tabText, "tab_text");
      byId(styles.m_tabBottom, "tab_bottom");
      byId(styles.m_tabFiller, "tab_filler");
      byId(styles.m_tabIcon, "tab_icon");
      byId(styles.m_tabCloseIcon, "tab_close_icon");
      byId(styles.m_tabModifiedIcon, "tab_modified_icon");
      byId(styles.m_tabHome, "tab_home");
      byId(styles.m_flag, "flag");
      byId(styles.m_warningBox, "warning_box");
      byId(styles.m_timeline, "timeline");
      byId(styles.m_timelineBox, "timeline_box");
      byId(styles.m_timelineOpenEye, "timeline_open_eye");
      byId(styles.m_timelineClosedEye, "timeline_closed_eye");
      byId(styles.m_timelineOpenPadlock, "timeline_open_padlock");
      byId(styles.m_timelineClosedPadlock, "timeline_closed_padlock");
      byId(styles.m_timelineContinuous, "timeline_continuous");
      byId(styles.m_timelineDiscontinuous, "timeline_discontinuous");
      byId(styles.m_timelineClosedGroup, "timeline_closed_group");
      byId(styles.m_timelineOpenGroup, "timeline_open_group");
      byId(styles.m_timelineLayer, "timeline_layer");
      byId(styles.m_timelineLayerTextOnly, "timeline_layer_text_only");
      byId(styles.m_timelineHeaderFrame, "timeline_header_frame");
      byId(styles.m_timelineSelectedCel, "timeline_selected_cel");
      byId(styles.m_timelineFocusedCel, "timeline_focused_cel");
      byId(styles.m_timelineEmptyFrame, "timeline_empty_frame");
      byId(styles.m_timelineKeyframe, "timeline_keyframe");
      byId(styles.m_timelineFromLeft, "timeline_from_left");
      byId(styles.m_timelineFromRight, "timeline_from_right");
      byId(styles.m_timelineFromBoth, "timeline_from_both");
      byId(styles.m_timelineLeftLink, "timeline_left_link");
      byId(styles.m_timelineRightLink, "timeline_right_link");
      byId(styles.m_timelineBothLinks, "timeline_both_links");
      byId(styles.m_timelineGear, "timeline_gear");
      byId(styles.m_timelineOnionskin, "timeline_onionskin");
      byId(styles.m_timelineOnionskinRange, "timeline_onionskin_range");
      byId(styles.m_timelinePadding, "timeline_padding");
      byId(styles.m_timelinePaddingTr, "timeline_padding_tr");
      byId(styles.m_timelinePaddingBl, "timeline_padding_bl");
      byId(styles.m_timelinePaddingBr, "timeline_padding_br");
      byId(styles.m_timelineRangeOutline, "timeline_range_outline");
      byId(styles.m_timelineDropLayerDeco, "timeline_drop_layer_deco");
      byId(styles.m_timelineDropFrameDeco, "timeline_drop_frame_deco");
      byId(styles.m_timelineLoopRange, "timeline_loop_range");
      byId(styles.m_timelineSwitchBandButton, "timeline_switch_band_button");
      byId(styles.m_shadeSelection, "shade_selection");
      byId(styles.m_colorbarSelection, "colorbar_selection");
      byId(styles.m_simpleColor, "simple_color");
      byId(styles.m_listItem, "list_item");
      byId(styles.m_asepriteFace, "aseprite_face");
    }

  private:
    void byId(int& dimension, const std::string& id) {
      dimension = static_cast<T*>(this)->getDimensionById(id);
    }
    void byId(gfx::Color& color, const std::string& id) {
      color = static_cast<T*>(this)->getColorById(id);
    }
    void byId(skin::SkinPartPtr& part, const std::string& id) {
      part = static_cast<T*>(this)->getPartById(id);
    }
    void byId(ui::Cursor*& cursor, const std::string& id) {
      cursor = static_cast<T*>(this)->getCursorById(id);
    }
    void byId(ui::Style*& style, const std::string& id) {
      style = static_cast<T*>(this)->getStyleById(id);
    }
  };

} // namespace gen
} // namespace app

#endif
