#pragma once
#ifdef __has_include
    #if !__has_include(<imgui.h>)
        #error "Couldn't find imgui.h in the header include path, please add it to the path!"
    #endif // !<imgui.h>
#endif
#include <imgui.h>

namespace UImGui
{
    struct IMGUI_API SemanticColorData
    {
        ImVec4 DestructiveColor;
        ImVec4 DestructiveColorActive;
        ImVec4 SuccessColor;
        ImVec4 SuccessColorActive;
        ImVec4 WarningColor;
        ImVec4 WarningColorActive;
    };

    class IMGUI_API Theme
    {
    public:
        /**
         * @brief Loads a theme from file location
         * @param file - The location of the theme file
         * @return Result state. 0 on success, -1 on bad file
         */
        static int load(const char* file, SemanticColorData* semanticColorData = nullptr) noexcept;
        static void save(const char* file, SemanticColorData* semanticColorData = nullptr) noexcept;

        static void showThemeEditor(void* bOpen) noexcept;
        static void showThemeEditorInline() noexcept;
    private:
        constexpr static const char* colourStrings[] =
        {
            "Text",
            "TextDisabled",
            "WindowBg",
            "ChildBg",
            "PopupBg",
            "Border",
            "BorderShadow",
            "FrameBg",
            "FrameBgHovered",
            "FrameBgActive",
            "TitleBg",
            "TitleBgActive",
            "TitleBgCollapsed",
            "MenuBarBg",
            "ScrollbarBg",
            "ScrollbarGrab",
            "ScrollbarGrabHovered",
            "ScrollbarGrabActive",
            "CheckMark",
            "SliderGrab",
            "SliderGrabActive",
            "Button",
            "ButtonHovered",
            "ButtonActive",
            "Header",
            "HeaderHovered",
            "HeaderActive",
            "Separator",
            "SeparatorHovered",
            "SeparatorActive",
            "ResizeGrip",
            "ResizeGripHovered",
            "ResizeGripActive",
            "Tab",
            "TabHovered",
            "TabActive",
            "TabUnfocused",
            "TabUnfocusedActive",
#ifdef IMGUI_HAS_DOCK
            "DockingPreview",
            "DockingEmptyBg",
#endif
            "PlotLines",
            "PlotLinesHovered",
            "PlotHistogram",
            "PlotHistogramHovered",
            "TableHeaderBg",
            "TableBorderStrong",
            "TableBorderLight",
            "TableRowBg",
            "TableRowBgAlt",
            "TextSelectedBg",
            "DragDropTarget",
            "NavHighlight",
            "NavWindowingHighlight",
            "NavWindowingDimBg",
            "ModalWindowDimBg",

            "DestructiveColor",
            "DestructiveColorActive",
            "SuccessColor",
            "SuccessColorActive",
            "WarningColor",
            "WarningColorActive"
        };
    };
}