// Don't modify, generated file from C:/aseprite/data/pref.xml

#ifndef GENERATED_PREF_TYPES_H_INCLUDED
#define GENERATED_PREF_TYPES_H_INCLUDED
#pragma once

#include <string>

namespace app {
namespace gen {

  enum class RightClickMode {
    DEFAULT = 0,
    PAINT_BGCOLOR = 0,
    PICK_FGCOLOR = 1,
    ERASE = 2,
    SCROLL = 3,
    RECTANGULAR_MARQUEE = 4,
    LASSO = 5,
  };

  enum class OnionskinType {
    MERGE = 0,
    RED_BLUE_TINT = 1,
  };

  enum class BrushType {
    CIRCLE = 0,
    SQUARE = 1,
    LINE = 2,
  };

  enum class BrushPreview {
    NONE = 0,
    EDGES = 1,
    FULL = 2,
  };

  enum class BgType {
    CHECKED_16x16 = 0,
    CHECKED_8x8 = 1,
    CHECKED_4x4 = 2,
    CHECKED_2x2 = 3,
    CHECKED_1x1 = 4,
  };

  enum class StopAtGrid {
    NEVER = 0,
    IF_VISIBLE = 1,
    ALWAYS = 2,
  };

  enum class FillReferTo {
    ACTIVE_LAYER = 0,
    ALL_LAYERS = 1,
  };

  enum class EyedropperChannel {
    COLOR_ALPHA = 0,
    COLOR = 1,
    ALPHA = 2,
    RGBA = 3,
    RGB = 4,
    HSVA = 5,
    HSV = 6,
    HSLA = 7,
    HSL = 8,
    GRAYA = 9,
    GRAY = 10,
    INDEX = 11,
  };

  enum class EyedropperSample {
    ALL_LAYERS = 0,
    CURRENT_LAYER = 1,
    FIRST_REFERENCE_LAYER = 2,
  };

  enum class SelectionMode {
    DEFAULT = 0,
    ADD = 1,
    SUBTRACT = 2,
  };

  enum class PivotPosition {
    NORTHWEST = 0,
    NORTH = 1,
    NORTHEAST = 2,
    WEST = 3,
    CENTER = 4,
    EAST = 5,
    SOUTHWEST = 6,
    SOUTH = 7,
    SOUTHEAST = 8,
  };

  enum class SymmetryMode {
    NONE = 0,
    HORIZONTAL = 1,
    VERTICAL = 2,
    BOTH = 3,
  };

  enum class PaintingCursorType {
    SIMPLE_CROSSHAIR = 0,
    CROSSHAIR_ON_SPRITE = 1,
  };

  enum class TimelinePosition {
    BOTTOM = 0,
    LEFT = 1,
    RIGHT = 2,
  };

  enum class HueSaturationMode {
    HSV = 0,
    HSL = 1,
  };

  class GlobalPref : public Section {
  public:
    GlobalPref(const std::string& name);
    void load();
    void save();
    Section* section(const char* id) override;
    OptionBase* option(const char* id) override;

    class General : public Section {
    public:
      General(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<std::string> language;
      Option<int> screenScale;
      Option<int> uiScale;
      Option<bool> gpuAcceleration;
      Option<bool> showHome;
      Option<bool> visibleTimeline;
      Option<bool> autoshowTimeline;
      Option<bool> rewindOnStop;
      Option<bool> expandMenubarOnMouseover;
      Option<bool> dataRecovery;
      Option<double> dataRecoveryPeriod;
      Option<bool> showFullPath;
      Option<TimelinePosition> timelinePosition;
      Option<bool> showMenuBar;
      Option<int> recentItems;
    };
    General general;

    class Undo : public Section {
    public:
      Undo(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<int> sizeLimit;
      Option<bool> gotoModified;
      Option<bool> allowNonlinearHistory;
    };
    Undo undo;

    class Editor : public Section {
    public:
      Editor(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> zoomWithWheel;
      Option<bool> zoomWithSlide;
      Option<bool> zoomFromCenterWithWheel;
      Option<bool> zoomFromCenterWithKeys;
      Option<bool> showScrollbars;
      Option<bool> autoScroll;
      Option<RightClickMode> rightClickMode;
      Option<bool> autoSelectLayer;
      Option<bool> straightLinePreview;
      Option<bool> playOnce;
      Option<bool> playAll;
    };
    Editor editor;

    class Cursor : public Section {
    public:
      Cursor(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> useNativeCursor;
      Option<int> cursorScale;
      Option<app::Color> cursorColor;
      Option<PaintingCursorType> paintingCursorType;
      Option<BrushPreview> brushPreview;
    };
    Cursor cursor;

    class Preview : public Section {
    public:
      Preview(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> playOnce;
      Option<bool> playAll;
    };
    Preview preview;

    class Theme : public Section {
    public:
      Theme(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<std::string> selected;
      Option<std::string> font;
      Option<std::string> miniFont;
    };
    Theme theme;

    class Experimental : public Section {
    public:
      Experimental(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> newRenderEngine;
      Option<bool> useNativeClipboard;
      Option<bool> useNativeFileDialog;
      Option<bool> oneFingerAsMouseMovement;
      Option<bool> loadWintabDriver;
      Option<bool> flashLayer;
      Option<int> nonactiveLayersOpacity;
    };
    Experimental experimental;

    class News : public Section {
    public:
      News(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<std::string> cacheFile;
    };
    News news;

    class StatusBar : public Section {
    public:
      StatusBar(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> focusFrameFieldOnMouseover;
    };
    StatusBar statusBar;

    class ColorBar : public Section {
    public:
      ColorBar(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<int> boxSize;
      Option<app::Color> fgColor;
      Option<app::Color> bgColor;
      Option<app::ColorBar::ColorSelector> selector;
      Option<bool> discreteWheel;
      Option<int> wheelModel;
      Option<int> harmony;
    };
    ColorBar colorBar;

    class Updater : public Section {
    public:
      Updater(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<int> inits;
      Option<int> exits;
      Option<bool> isDeveloper;
      Option<double> waitDays;
      Option<int> lastCheck;
      Option<std::string> uuid;
      Option<std::string> currentVersion;
      Option<std::string> newVersion;
      Option<std::string> newUrl;
    };
    Updater updater;

    class Brush : public Section {
    public:
      Brush(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<doc::BrushPattern> pattern;
    };
    Brush brush;

    class SaveBrush : public Section {
    public:
      SaveBrush(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> brushType;
      Option<bool> brushSize;
      Option<bool> brushAngle;
      Option<bool> fgColor;
      Option<bool> bgColor;
      Option<bool> imageColor;
      Option<bool> inkType;
      Option<bool> inkOpacity;
      Option<bool> shade;
      Option<bool> pixelPerfect;
    };
    SaveBrush saveBrush;

    class Selection : public Section {
    public:
      Selection(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<SelectionMode> mode;
      Option<bool> moveEdges;
      Option<bool> modifiersDisableHandles;
      Option<bool> moveOnAddMode;
      Option<bool> pivotVisibility;
      Option<PivotPosition> pivotPosition;
      Option<bool> opaque;
      Option<bool> autoOpaque;
      Option<bool> keepSelectionAfterClear;
      Option<bool> autoShowSelectionEdges;
      Option<app::Color> transparentColor;
      Option<app::tools::RotationAlgorithm> rotationAlgorithm;
      Option<int> modifySelectionQuantity;
      Option<BrushType> modifySelectionBrush;
    };
    Selection selection;

    class Quantization : public Section {
    public:
      Quantization(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> withAlpha;
    };
    Quantization quantization;

    class Eyedropper : public Section {
    public:
      Eyedropper(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<EyedropperChannel> channel;
      Option<EyedropperSample> sample;
      Option<bool> discardBrush;
    };
    Eyedropper eyedropper;

    class Shared : public Section {
    public:
      Shared(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> shareInk;
      Option<app::tools::InkType> ink;
    };
    Shared shared;

    class NewFile : public Section {
    public:
      NewFile(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<int> width;
      Option<int> height;
      Option<doc::PixelFormat> colorMode;
      Option<int> backgroundColor;
      Option<bool> advanced;
      Option<std::string> pixelRatio;
    };
    NewFile newFile;

    class TextTool : public Section {
    public:
      TextTool(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<std::string> fontFace;
      Option<int> fontSize;
      Option<bool> antialias;
    };
    TextTool textTool;

    class SymmetryMode : public Section {
    public:
      SymmetryMode(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> enabled;
    };
    SymmetryMode symmetryMode;

    class Perf : public Section {
    public:
      Perf(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> showRenderTime;
    };
    Perf perf;

    class Guides : public Section {
    public:
      Guides(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<app::Color> layerEdgesColor;
      Option<app::Color> autoGuidesColor;
    };
    Guides guides;

    class Slices : public Section {
    public:
      Slices(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<app::Color> defaultColor;
    };
    Slices slices;

    class AdvancedMode : public Section {
    public:
      AdvancedMode(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> showAlert;
    };
    AdvancedMode advancedMode;

    class SaveFile : public Section {
    public:
      SaveFile(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> showFileFormatDoesntSupportAlert;
      Option<bool> showExportAnimationInSequenceAlert;
      Option<std::string> defaultExtension;
    };
    SaveFile saveFile;

    class ExportFile : public Section {
    public:
      ExportFile(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> showOverwriteFilesAlert;
      Option<std::string> imageDefaultExtension;
      Option<std::string> animationDefaultExtension;
    };
    ExportFile exportFile;

    class SpriteSheet : public Section {
    public:
      SpriteSheet(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> showOverwriteFilesAlert;
      Option<std::string> defaultExtension;
    };
    SpriteSheet spriteSheet;

    class Gif : public Section {
    public:
      Gif(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> showAlert;
      Option<bool> interlaced;
      Option<bool> loop;
    };
    Gif gif;

    class Jpeg : public Section {
    public:
      Jpeg(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> showAlert;
      Option<double> quality;
    };
    Jpeg jpeg;

    class Webp : public Section {
    public:
      Webp(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> showAlert;
      Option<bool> loop;
      Option<int> type;
      Option<int> quality;
      Option<int> compression;
      Option<int> imageHint;
      Option<int> imagePreset;
    };
    Webp webp;

    class HueSaturation : public Section {
    public:
      HueSaturation(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<HueSaturationMode> mode;
    };
    HueSaturation hueSaturation;

    class Filters : public Section {
    public:
      Filters(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<CelsTarget> celsTarget;
    };
    Filters filters;
  };

  class ToolPref : public Section {
  public:
    ToolPref(const std::string& name);
    void load();
    void save();
    Section* section(const char* id) override;
    OptionBase* option(const char* id) override;
    Option<int> opacity;
    Option<int> tolerance;
    Option<bool> contiguous;
    Option<bool> filled;
    Option<bool> filledPreview;
    Option<app::tools::InkType> ink;
    Option<app::tools::FreehandAlgorithm> freehandAlgorithm;

    class Brush : public Section {
    public:
      Brush(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<BrushType> type;
      Option<int> size;
      Option<int> angle;
    };
    Brush brush;

    class Spray : public Section {
    public:
      Spray(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<int> width;
      Option<int> speed;
    };
    Spray spray;

    class Floodfill : public Section {
    public:
      Floodfill(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<StopAtGrid> stopAtGrid;
      Option<FillReferTo> referTo;
    };
    Floodfill floodfill;
  };

  class DocPref : public Section {
  public:
    DocPref(const std::string& name);
    void load();
    void save();
    Section* section(const char* id) override;
    OptionBase* option(const char* id) override;

    class Site : public Section {
    public:
      Site(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<doc::frame_t> frame;
      Option<doc::layer_t> layer;
    };
    Site site;

    class Tiled : public Section {
    public:
      Tiled(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<filters::TiledMode> mode;
    };
    Tiled tiled;

    class Symmetry : public Section {
    public:
      Symmetry(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<SymmetryMode> mode;
      Option<double> xAxis;
      Option<double> yAxis;
    };
    Symmetry symmetry;

    class Grid : public Section {
    public:
      Grid(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> snap;
      Option<gfx::Rect> bounds;
      Option<app::Color> color;
      Option<int> opacity;
      Option<bool> autoOpacity;
    };
    Grid grid;

    class PixelGrid : public Section {
    public:
      PixelGrid(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<app::Color> color;
      Option<int> opacity;
      Option<bool> autoOpacity;
    };
    PixelGrid pixelGrid;

    class Bg : public Section {
    public:
      Bg(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<BgType> type;
      Option<bool> zoom;
      Option<app::Color> color1;
      Option<app::Color> color2;
    };
    Bg bg;

    class Timeline : public Section {
    public:
      Timeline(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<int> firstFrame;
    };
    Timeline timeline;

    class Thumbnails : public Section {
    public:
      Thumbnails(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<double> zoom;
      Option<bool> enabled;
      Option<bool> overlayEnabled;
      Option<int> overlaySize;
    };
    Thumbnails thumbnails;

    class Onionskin : public Section {
    public:
      Onionskin(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> active;
      Option<int> prevFrames;
      Option<int> nextFrames;
      Option<int> opacityBase;
      Option<int> opacityStep;
      Option<OnionskinType> type;
      Option<bool> loopTag;
      Option<bool> currentLayer;
      Option<render::OnionskinPosition> position;
    };
    Onionskin onionskin;

    class SaveCopy : public Section {
    public:
      SaveCopy(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<std::string> filename;
      Option<double> resizeScale;
      Option<std::string> layer;
      Option<std::string> frameTag;
      Option<doc::AniDir> aniDir;
      Option<bool> applyPixelRatio;
      Option<bool> forTwitter;
    };
    SaveCopy saveCopy;

    class SpriteSheet : public Section {
    public:
      SpriteSheet(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> defined;
      Option<app::SpriteSheetType> type;
      Option<int> columns;
      Option<int> rows;
      Option<int> width;
      Option<int> height;
      Option<bool> bestFit;
      Option<std::string> textureFilename;
      Option<std::string> dataFilename;
      Option<DocumentExporter::DataFormat> dataFormat;
      Option<int> borderPadding;
      Option<int> shapePadding;
      Option<int> innerPadding;
      Option<bool> trim;
      Option<bool> openGenerated;
      Option<std::string> layer;
      Option<std::string> frameTag;
      Option<bool> listLayers;
      Option<bool> listFrameTags;
      Option<bool> listSlices;
    };
    SpriteSheet spriteSheet;

    class ImportSpriteSheet : public Section {
    public:
      ImportSpriteSheet(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<app::SpriteSheetType> type;
      Option<gfx::Rect> bounds;
      Option<bool> partialTiles;
    };
    ImportSpriteSheet importSpriteSheet;

    class Preview : public Section {
    public:
      Preview(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<double> zoom;
      Option<gfx::Point> scroll;
      Option<bool> autoScroll;
    };
    Preview preview;

    class Show : public Section {
    public:
      Show(const std::string& name);
      void load();
      void save();
      Section* section(const char* id) override;
      OptionBase* option(const char* id) override;
      Option<bool> layerEdges;
      Option<bool> selectionEdges;
      Option<bool> grid;
      Option<bool> pixelGrid;
      Option<bool> brushPreview;
      Option<bool> slices;
      Option<bool> autoGuides;
    };
    Show show;
  };

} // namespace gen
} // namespace app

#endif
