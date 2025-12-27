#include "UTheme.hpp"
#include <fstream>
#include <ryml.hpp>

#define LOAD_YAML_STYLE_VAR(x) parse_style_var<decltype(style.x)>(root[#x], style.x);
#define LOAD_YAML_SEMANTIC_COLOUR(x, y) read_vec4(root[#x], x->y)

static void read_vec4(const ryml::ConstNodeRef& node, ImVec4& vec)
{
    if (node.is_seq() && node.num_children() >= 4)
    {
        node[0] >> vec.x;
        node[1] >> vec.y;
        node[2] >> vec.z;
        node[3] >> vec.w;
    }
}

static void read_vec4(const ryml::ConstNodeRef& node, UImGui_ThemeVec4& vec)
{
    if (node.is_seq() && node.num_children() >= 4)
    {
        node[0] >> vec.x;
        node[1] >> vec.y;
        node[2] >> vec.z;
        node[3] >> vec.w;
    }
}

template<typename T>
void parse_style_var(const ryml::ConstNodeRef& node, T& data) noexcept
{
    if (!node.invalid())
    {
        // Handling for ImGuiDir and other enum types
        if constexpr (std::is_enum_v<T>)
        {
            // Get as standard integer
            std::underlying_type_t<T> tmp = data;
            node >> tmp; // Get the data
            data = static_cast<T>(tmp); // Cast back to enum
        }
        else
            node >> data;
    }
}

template<>
void parse_style_var<ImVec2>(const ryml::ConstNodeRef& node, ImVec2& data) noexcept
{
    if (!node.invalid() && node.is_seq() && node.num_children() >= 2)
    {
        node[0] >> data.x;
        node[1] >> data.y;
    }
}


int UImGui::Theme::load(const char* file, SemanticColourData* semanticColorData) noexcept
{
    std::ifstream f(file);
    f.seekg(0, std::ios::end);
    const size_t size = f.tellg();

    std::string buffer(size, ' ');

    f.seekg(0);
    f.read(&buffer[0], static_cast<long>(size));
    f.close();

    auto tree = ryml::parse_in_arena(buffer.c_str());
    if (tree.empty())
        return -1;

    const auto root = tree.rootref();
    auto& style = ImGui::GetStyle();

    for (size_t i = 0; i < ImGuiCol_COUNT; i++)
        if (!root[colourStrings[i]].invalid())
            read_vec4(root[colourStrings[i]], style.Colors[i]);

    LOAD_YAML_STYLE_VAR(FontSizeBase)
    LOAD_YAML_STYLE_VAR(FontScaleMain)
    LOAD_YAML_STYLE_VAR(FontScaleDpi)

    LOAD_YAML_STYLE_VAR(Alpha)
    LOAD_YAML_STYLE_VAR(DisabledAlpha)
    LOAD_YAML_STYLE_VAR(WindowPadding)
    LOAD_YAML_STYLE_VAR(WindowRounding)
    LOAD_YAML_STYLE_VAR(WindowBorderSize)
    LOAD_YAML_STYLE_VAR(WindowBorderHoverPadding)
    LOAD_YAML_STYLE_VAR(WindowMinSize)
    LOAD_YAML_STYLE_VAR(WindowTitleAlign)
    LOAD_YAML_STYLE_VAR(WindowMenuButtonPosition)
    LOAD_YAML_STYLE_VAR(ChildRounding)
    LOAD_YAML_STYLE_VAR(ChildBorderSize)
    LOAD_YAML_STYLE_VAR(PopupRounding)
    LOAD_YAML_STYLE_VAR(PopupBorderSize)
    LOAD_YAML_STYLE_VAR(FramePadding)
    LOAD_YAML_STYLE_VAR(FrameRounding)
    LOAD_YAML_STYLE_VAR(FrameBorderSize)
    LOAD_YAML_STYLE_VAR(ItemSpacing)
    LOAD_YAML_STYLE_VAR(ItemInnerSpacing)
    LOAD_YAML_STYLE_VAR(CellPadding)
    LOAD_YAML_STYLE_VAR(TouchExtraPadding)
    LOAD_YAML_STYLE_VAR(IndentSpacing)
    LOAD_YAML_STYLE_VAR(ColumnsMinSpacing)
    LOAD_YAML_STYLE_VAR(ScrollbarSize)
    LOAD_YAML_STYLE_VAR(ScrollbarRounding)
    LOAD_YAML_STYLE_VAR(ScrollbarPadding)
    LOAD_YAML_STYLE_VAR(GrabMinSize)
    LOAD_YAML_STYLE_VAR(GrabRounding)
    LOAD_YAML_STYLE_VAR(LogSliderDeadzone)
    LOAD_YAML_STYLE_VAR(ImageBorderSize)
    LOAD_YAML_STYLE_VAR(TabRounding)
    LOAD_YAML_STYLE_VAR(TabBorderSize)
    LOAD_YAML_STYLE_VAR(TabMinWidthBase)
    LOAD_YAML_STYLE_VAR(TabMinWidthShrink)
    LOAD_YAML_STYLE_VAR(TabCloseButtonMinWidthSelected)
    LOAD_YAML_STYLE_VAR(TabCloseButtonMinWidthUnselected)
    LOAD_YAML_STYLE_VAR(TabBarBorderSize)
    LOAD_YAML_STYLE_VAR(TabBarOverlineSize)
    LOAD_YAML_STYLE_VAR(TableAngledHeadersAngle)
    LOAD_YAML_STYLE_VAR(TableAngledHeadersTextAlign)
    LOAD_YAML_STYLE_VAR(TreeLinesFlags)
    LOAD_YAML_STYLE_VAR(TreeLinesSize)
    LOAD_YAML_STYLE_VAR(TreeLinesRounding)
    LOAD_YAML_STYLE_VAR(DragDropTargetRounding)
    LOAD_YAML_STYLE_VAR(DragDropTargetBorderSize)
    LOAD_YAML_STYLE_VAR(DragDropTargetPadding)
    LOAD_YAML_STYLE_VAR(ColorMarkerSize)
    LOAD_YAML_STYLE_VAR(ColorButtonPosition)
    LOAD_YAML_STYLE_VAR(ButtonTextAlign)
    LOAD_YAML_STYLE_VAR(SelectableTextAlign)
    LOAD_YAML_STYLE_VAR(SeparatorTextBorderSize)
    LOAD_YAML_STYLE_VAR(SeparatorTextAlign)
    LOAD_YAML_STYLE_VAR(SeparatorTextPadding)
    LOAD_YAML_STYLE_VAR(DisplayWindowPadding)
    LOAD_YAML_STYLE_VAR(DisplaySafeAreaPadding)

    #ifdef IMGUI_HAS_DOCK
        LOAD_YAML_STYLE_VAR(DockingNodeHasCloseButton)
        LOAD_YAML_STYLE_VAR(DockingSeparatorSize)
    #endif

    LOAD_YAML_STYLE_VAR(MouseCursorScale)
    LOAD_YAML_STYLE_VAR(AntiAliasedLines)
    LOAD_YAML_STYLE_VAR(AntiAliasedLinesUseTex)
    LOAD_YAML_STYLE_VAR(AntiAliasedFill)
    LOAD_YAML_STYLE_VAR(CurveTessellationTol)
    LOAD_YAML_STYLE_VAR(CircleTessellationMaxError)

    LOAD_YAML_STYLE_VAR(HoverStationaryDelay)
    LOAD_YAML_STYLE_VAR(HoverDelayShort)
    LOAD_YAML_STYLE_VAR(HoverDelayNormal)
    LOAD_YAML_STYLE_VAR(HoverFlagsForTooltipMouse)
    LOAD_YAML_STYLE_VAR(HoverFlagsForTooltipNav)

    if (semanticColorData != nullptr)
    {
        LOAD_YAML_SEMANTIC_COLOUR(semanticColorData, DestructiveColor);
        LOAD_YAML_SEMANTIC_COLOUR(semanticColorData, DestructiveColorActive);
        LOAD_YAML_SEMANTIC_COLOUR(semanticColorData, SuccessColor);
        LOAD_YAML_SEMANTIC_COLOUR(semanticColorData, SuccessColorActive);
        LOAD_YAML_SEMANTIC_COLOUR(semanticColorData, WarningColor);
        LOAD_YAML_SEMANTIC_COLOUR(semanticColorData, WarningColorActive);
    }

    return 0;
}

static void emit_vec4(ryml::NodeRef& node, const ImVec4& vec)
{
    node |= ryml::SEQ | ryml::FLOW_SL;
    if (!node.invalid())
    {
        node.append_child() << vec.x;
        node.append_child() << vec.y;
        node.append_child() << vec.z;
        node.append_child() << vec.w;
    }
}

static void emit_vec4(ryml::NodeRef& node, const UImGui_ThemeVec4& vec)
{
    node |= ryml::SEQ | ryml::FLOW_SL;
    if (!node.invalid())
    {
        node.append_child() << vec.x;
        node.append_child() << vec.y;
        node.append_child() << vec.z;
        node.append_child() << vec.w;
    }
}

template<typename T>
void emit_style_var(ryml::NodeRef& node, const T& data) noexcept
{
    if (!node.invalid())
    {
        // Handling for ImGuiDir and other enum types
        if constexpr (std::is_enum_v<T>)
        {
            // Get as standard integer
            std::underlying_type_t<T> tmp = data;
            node << tmp; // Get the data
        }
        else
            node << data;
    }
}

template<>
void emit_style_var<ImVec2>(ryml::NodeRef& node, const ImVec2& data) noexcept
{
    node |= ryml::SEQ | ryml::FLOW_SL;
    if (!node.invalid())
    {
        node.append_child() << data.x;
        node.append_child() << data.y;
    }
}

#define OUTPUT_YAML_STYLE_VAR(x)        \
{                                       \
    auto ref = root[#x];                \
    emit_style_var(ref, style.x);       \
}

#define OUTPUT_YAML_SEMANTIC_COLOUR(x, y)   \
{                                           \
    auto ref = root[#y];                    \
    emit_vec4(ref, x->y);                    \
}

void UImGui::Theme::save(const char* file, SemanticColourData* semanticColorData) noexcept
{
    auto& style = ImGui::GetStyle();
    ryml::Tree tree;

    ryml::NodeRef root = tree.rootref();
    root |= ryml::MAP;

    for (size_t i = 0; i < ImGuiCol_COUNT; i++)
    {
        auto ref = root[colourStrings[i]];
        emit_vec4(ref, style.Colors[i]);
    }

    OUTPUT_YAML_STYLE_VAR(FontSizeBase)
    OUTPUT_YAML_STYLE_VAR(FontScaleMain)
    OUTPUT_YAML_STYLE_VAR(FontScaleDpi)

    OUTPUT_YAML_STYLE_VAR(Alpha)
    OUTPUT_YAML_STYLE_VAR(DisabledAlpha)
    OUTPUT_YAML_STYLE_VAR(WindowPadding)
    OUTPUT_YAML_STYLE_VAR(WindowRounding)
    OUTPUT_YAML_STYLE_VAR(WindowBorderSize)
    OUTPUT_YAML_STYLE_VAR(WindowBorderHoverPadding)
    OUTPUT_YAML_STYLE_VAR(WindowMinSize)
    OUTPUT_YAML_STYLE_VAR(WindowTitleAlign)
    OUTPUT_YAML_STYLE_VAR(WindowMenuButtonPosition)
    OUTPUT_YAML_STYLE_VAR(ChildRounding)
    OUTPUT_YAML_STYLE_VAR(ChildBorderSize)
    OUTPUT_YAML_STYLE_VAR(PopupRounding)
    OUTPUT_YAML_STYLE_VAR(PopupBorderSize)
    OUTPUT_YAML_STYLE_VAR(FramePadding)
    OUTPUT_YAML_STYLE_VAR(FrameRounding)
    OUTPUT_YAML_STYLE_VAR(FrameBorderSize)
    OUTPUT_YAML_STYLE_VAR(ItemSpacing)
    OUTPUT_YAML_STYLE_VAR(ItemInnerSpacing)
    OUTPUT_YAML_STYLE_VAR(CellPadding)
    OUTPUT_YAML_STYLE_VAR(TouchExtraPadding)
    OUTPUT_YAML_STYLE_VAR(IndentSpacing)
    OUTPUT_YAML_STYLE_VAR(ColumnsMinSpacing)
    OUTPUT_YAML_STYLE_VAR(ScrollbarSize)
    OUTPUT_YAML_STYLE_VAR(ScrollbarRounding)
    OUTPUT_YAML_STYLE_VAR(ScrollbarPadding)
    OUTPUT_YAML_STYLE_VAR(GrabMinSize)
    OUTPUT_YAML_STYLE_VAR(GrabRounding)
    OUTPUT_YAML_STYLE_VAR(LogSliderDeadzone)
    OUTPUT_YAML_STYLE_VAR(ImageBorderSize)
    OUTPUT_YAML_STYLE_VAR(TabRounding)
    OUTPUT_YAML_STYLE_VAR(TabBorderSize)
    OUTPUT_YAML_STYLE_VAR(TabMinWidthBase)
    OUTPUT_YAML_STYLE_VAR(TabMinWidthShrink)
    OUTPUT_YAML_STYLE_VAR(TabCloseButtonMinWidthSelected)
    OUTPUT_YAML_STYLE_VAR(TabCloseButtonMinWidthUnselected)
    OUTPUT_YAML_STYLE_VAR(TabBarBorderSize)
    OUTPUT_YAML_STYLE_VAR(TabBarOverlineSize)
    OUTPUT_YAML_STYLE_VAR(TableAngledHeadersAngle)
    OUTPUT_YAML_STYLE_VAR(TableAngledHeadersTextAlign)
    OUTPUT_YAML_STYLE_VAR(TreeLinesFlags)
    OUTPUT_YAML_STYLE_VAR(TreeLinesSize)
    OUTPUT_YAML_STYLE_VAR(TreeLinesRounding)
    OUTPUT_YAML_STYLE_VAR(DragDropTargetRounding)
    OUTPUT_YAML_STYLE_VAR(DragDropTargetBorderSize)
    OUTPUT_YAML_STYLE_VAR(DragDropTargetPadding)
    OUTPUT_YAML_STYLE_VAR(ColorMarkerSize)
    OUTPUT_YAML_STYLE_VAR(ColorButtonPosition)
    OUTPUT_YAML_STYLE_VAR(ButtonTextAlign)
    OUTPUT_YAML_STYLE_VAR(SelectableTextAlign)
    OUTPUT_YAML_STYLE_VAR(SeparatorTextBorderSize)
    OUTPUT_YAML_STYLE_VAR(SeparatorTextAlign)
    OUTPUT_YAML_STYLE_VAR(SeparatorTextPadding)
    OUTPUT_YAML_STYLE_VAR(DisplayWindowPadding)
    OUTPUT_YAML_STYLE_VAR(DisplaySafeAreaPadding)

    #ifdef IMGUI_HAS_DOCK
        OUTPUT_YAML_STYLE_VAR(DockingNodeHasCloseButton)
        OUTPUT_YAML_STYLE_VAR(DockingSeparatorSize)
    #endif

    OUTPUT_YAML_STYLE_VAR(MouseCursorScale)
    OUTPUT_YAML_STYLE_VAR(AntiAliasedLines)
    OUTPUT_YAML_STYLE_VAR(AntiAliasedLinesUseTex)
    OUTPUT_YAML_STYLE_VAR(AntiAliasedFill)
    OUTPUT_YAML_STYLE_VAR(CurveTessellationTol)
    OUTPUT_YAML_STYLE_VAR(CircleTessellationMaxError)

    OUTPUT_YAML_STYLE_VAR(HoverStationaryDelay)
    OUTPUT_YAML_STYLE_VAR(HoverDelayShort)
    OUTPUT_YAML_STYLE_VAR(HoverDelayNormal)
    OUTPUT_YAML_STYLE_VAR(HoverFlagsForTooltipMouse)
    OUTPUT_YAML_STYLE_VAR(HoverFlagsForTooltipNav)


    if (semanticColorData != nullptr)
    {
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, DestructiveColor);
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, DestructiveColorActive);
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, SuccessColor);
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, SuccessColorActive);
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, WarningColor);
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, WarningColorActive);
    }

    std::ofstream o(file);
    o << tree;
    o.close();
}

void renderStyleVar(const char* name, float& t) noexcept
{
    ImGui::DragFloat(name, &t);
}

void renderStyleVar(const char* name, ImVec2& t) noexcept
{
    ImGui::DragFloat2(name, reinterpret_cast<float*>(&t));
}

void renderStyleVar(const char* name, ImGuiDir& dir) noexcept
{
    int tmpdir = dir + 1;
    if (ImGui::Combo(name, &tmpdir, "None\0Left\0Right\0Up\0Down\0"))
        dir = static_cast<ImGuiDir>(tmpdir - 1);
}

void renderStyleVar(const char* name, bool& t) noexcept
{
    ImGui::Checkbox(name, &t);
}

void UImGui::Theme::showThemeEditor(void* bOpen) noexcept
{
    if (ImGui::Begin("UntitledImGuiTheme Theme Editor", static_cast<bool*>(bOpen)))
    {
        showThemeEditorInline();
        ImGui::End();
    }
}

#define RENDER_STYLE_VAR_EDIT(x) renderStyleVar(#x, style.x)

void UImGui::Theme::showThemeEditorInline() noexcept
{
    auto& style = ImGui::GetStyle();
    for (size_t i = 0; i < ImGuiCol_COUNT; i++)
        ImGui::ColorEdit4(colourStrings[i], reinterpret_cast<float*>(&style.Colors[i]));

    RENDER_STYLE_VAR_EDIT(FontSizeBase);
    RENDER_STYLE_VAR_EDIT(FontScaleMain);
    RENDER_STYLE_VAR_EDIT(FontScaleDpi);

    RENDER_STYLE_VAR_EDIT(Alpha);
    RENDER_STYLE_VAR_EDIT(DisabledAlpha);
    RENDER_STYLE_VAR_EDIT(WindowPadding);
    RENDER_STYLE_VAR_EDIT(WindowRounding);
    RENDER_STYLE_VAR_EDIT(WindowBorderSize);
    RENDER_STYLE_VAR_EDIT(WindowBorderHoverPadding);
    RENDER_STYLE_VAR_EDIT(WindowMinSize);
    RENDER_STYLE_VAR_EDIT(WindowTitleAlign);
    RENDER_STYLE_VAR_EDIT(WindowMenuButtonPosition);
    RENDER_STYLE_VAR_EDIT(ChildRounding);
    RENDER_STYLE_VAR_EDIT(ChildBorderSize);
    RENDER_STYLE_VAR_EDIT(PopupRounding);
    RENDER_STYLE_VAR_EDIT(PopupBorderSize);
    RENDER_STYLE_VAR_EDIT(FramePadding);
    RENDER_STYLE_VAR_EDIT(FrameRounding);
    RENDER_STYLE_VAR_EDIT(FrameBorderSize);
    RENDER_STYLE_VAR_EDIT(ItemSpacing);
    RENDER_STYLE_VAR_EDIT(ItemInnerSpacing);
    RENDER_STYLE_VAR_EDIT(CellPadding);
    RENDER_STYLE_VAR_EDIT(TouchExtraPadding);
    RENDER_STYLE_VAR_EDIT(IndentSpacing);
    RENDER_STYLE_VAR_EDIT(ColumnsMinSpacing);
    RENDER_STYLE_VAR_EDIT(ScrollbarSize);
    RENDER_STYLE_VAR_EDIT(ScrollbarRounding);
    RENDER_STYLE_VAR_EDIT(ScrollbarPadding);
    RENDER_STYLE_VAR_EDIT(GrabMinSize);
    RENDER_STYLE_VAR_EDIT(GrabRounding);
    RENDER_STYLE_VAR_EDIT(LogSliderDeadzone);
    RENDER_STYLE_VAR_EDIT(ImageBorderSize);
    RENDER_STYLE_VAR_EDIT(TabRounding);
    RENDER_STYLE_VAR_EDIT(TabBorderSize);
    RENDER_STYLE_VAR_EDIT(TabMinWidthBase);
    RENDER_STYLE_VAR_EDIT(TabMinWidthShrink);
    RENDER_STYLE_VAR_EDIT(TabCloseButtonMinWidthSelected);
    RENDER_STYLE_VAR_EDIT(TabCloseButtonMinWidthUnselected);
    RENDER_STYLE_VAR_EDIT(TabBarBorderSize);
    RENDER_STYLE_VAR_EDIT(TabBarOverlineSize);
    RENDER_STYLE_VAR_EDIT(TableAngledHeadersAngle);
    RENDER_STYLE_VAR_EDIT(TableAngledHeadersTextAlign);
    RENDER_STYLE_VAR_EDIT(TreeLinesSize);
    RENDER_STYLE_VAR_EDIT(TreeLinesRounding);
    RENDER_STYLE_VAR_EDIT(DragDropTargetRounding);
    RENDER_STYLE_VAR_EDIT(DragDropTargetBorderSize);
    RENDER_STYLE_VAR_EDIT(DragDropTargetPadding);
    RENDER_STYLE_VAR_EDIT(ColorMarkerSize);
    RENDER_STYLE_VAR_EDIT(ColorButtonPosition);
    RENDER_STYLE_VAR_EDIT(ButtonTextAlign);
    RENDER_STYLE_VAR_EDIT(SelectableTextAlign);
    RENDER_STYLE_VAR_EDIT(SeparatorTextBorderSize);
    RENDER_STYLE_VAR_EDIT(SeparatorTextAlign);
    RENDER_STYLE_VAR_EDIT(SeparatorTextPadding);
    RENDER_STYLE_VAR_EDIT(DisplayWindowPadding);
    RENDER_STYLE_VAR_EDIT(DisplaySafeAreaPadding);

    #ifdef IMGUI_HAS_DOCK
        RENDER_STYLE_VAR_EDIT(DockingNodeHasCloseButton);
        RENDER_STYLE_VAR_EDIT(DockingSeparatorSize);
    #endif

    RENDER_STYLE_VAR_EDIT(MouseCursorScale);
    RENDER_STYLE_VAR_EDIT(AntiAliasedLines);
    RENDER_STYLE_VAR_EDIT(AntiAliasedLinesUseTex);
    RENDER_STYLE_VAR_EDIT(AntiAliasedFill);
    RENDER_STYLE_VAR_EDIT(CurveTessellationTol);
    RENDER_STYLE_VAR_EDIT(CircleTessellationMaxError);

    RENDER_STYLE_VAR_EDIT(HoverStationaryDelay);
    RENDER_STYLE_VAR_EDIT(HoverDelayShort);
    RENDER_STYLE_VAR_EDIT(HoverDelayNormal);
}
