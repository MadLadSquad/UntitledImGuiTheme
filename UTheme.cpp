#include "UTheme.hpp"
#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML
{
    template<>
    struct convert<ImVec4>
    {
        static Node encode(const ImVec4& rhs) noexcept
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, ImVec4& rhs) noexcept
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<ImVec2>
    {
        static Node encode(const ImVec2& rhs) noexcept
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, ImVec2& rhs) noexcept
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };
}

YAML::Emitter& operator<<(YAML::Emitter& out, const ImVec4& vect) noexcept
{
    out << YAML::Flow;
    out << YAML::BeginSeq << vect.x << vect.y << vect.z << vect.w << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const ImVec2& vect) noexcept
{
    out << YAML::Flow;
    out << YAML::BeginSeq << vect.x << vect.y << YAML::EndSeq;
    return out;
}

#define LOAD_YAML_STYLE_VAR(x) if (out[#x]) style.x = out[#x].as<decltype(ImGuiStyle::x)>()
#define OUTPUT_YAML_STYLE_VAR(x) out << YAML::Key << #x << YAML::Value << style.x;
#define RENDER_STYLE_VAR_EDIT(x) renderStyleVar(#x, style.x)

#define LOAD_YAML_SEMANTIC_COLOUR(x, y) if (out[#y]) (x)->y = out[#y].as<ImVec4>()
#define OUTPUT_YAML_SEMANTIC_COLOUR(x, y) out << YAML::Key << #y << YAML::Value << x->y;

int UImGui::Theme::load(const char* file, SemanticColorData* semanticColorData) noexcept
{
    YAML::Node out;
    try
    {
        out = YAML::LoadFile(file);
    }
    catch (YAML::BadFile&)
    {
        return -1;
    }
    auto& style = ImGui::GetStyle();
    for (size_t i = 0; i < ImGuiCol_COUNT; i++)
        if (out[colourStrings[i]])
            style.Colors[i] = out[colourStrings[i]].as<ImVec4>();

    LOAD_YAML_STYLE_VAR(Alpha);
    LOAD_YAML_STYLE_VAR(DisabledAlpha);
    LOAD_YAML_STYLE_VAR(WindowPadding);
    LOAD_YAML_STYLE_VAR(WindowRounding);
    LOAD_YAML_STYLE_VAR(WindowBorderSize);
    LOAD_YAML_STYLE_VAR(WindowMinSize);
    LOAD_YAML_STYLE_VAR(WindowTitleAlign);
    LOAD_YAML_STYLE_VAR(ChildRounding);
    LOAD_YAML_STYLE_VAR(ChildBorderSize);
    LOAD_YAML_STYLE_VAR(PopupRounding);
    LOAD_YAML_STYLE_VAR(PopupBorderSize);
    LOAD_YAML_STYLE_VAR(FramePadding);
    LOAD_YAML_STYLE_VAR(FrameRounding);
    LOAD_YAML_STYLE_VAR(FrameBorderSize);
    LOAD_YAML_STYLE_VAR(ItemSpacing);
    LOAD_YAML_STYLE_VAR(ItemInnerSpacing);
    LOAD_YAML_STYLE_VAR(IndentSpacing);
    LOAD_YAML_STYLE_VAR(CellPadding);
    LOAD_YAML_STYLE_VAR(ScrollbarSize);
    LOAD_YAML_STYLE_VAR(ScrollbarRounding);
    LOAD_YAML_STYLE_VAR(GrabMinSize);
    LOAD_YAML_STYLE_VAR(GrabRounding);
    LOAD_YAML_STYLE_VAR(TabRounding);
    LOAD_YAML_STYLE_VAR(ButtonTextAlign);
    LOAD_YAML_STYLE_VAR(SelectableTextAlign);
    LOAD_YAML_STYLE_VAR(SeparatorTextBorderSize);
    LOAD_YAML_STYLE_VAR(SeparatorTextAlign);
    LOAD_YAML_STYLE_VAR(SeparatorTextPadding);
#ifdef IMGUI_HAS_DOCK
    LOAD_YAML_STYLE_VAR(DockingSeparatorSize);
#endif

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

void UImGui::Theme::save(const char* file, const SemanticColorData* semanticColorData) noexcept
{
    const auto& style = ImGui::GetStyle();

    YAML::Emitter out;
    out << YAML::BeginMap;
    // Colours
    for (size_t i = 0; i < ImGuiCol_COUNT; i++)
        out << YAML::Key << colourStrings[i] << YAML::Value << style.Colors[i];

    // Style
    OUTPUT_YAML_STYLE_VAR(Alpha);
    OUTPUT_YAML_STYLE_VAR(DisabledAlpha);
    OUTPUT_YAML_STYLE_VAR(WindowPadding);
    OUTPUT_YAML_STYLE_VAR(WindowRounding);
    OUTPUT_YAML_STYLE_VAR(WindowBorderSize);
    OUTPUT_YAML_STYLE_VAR(WindowMinSize);
    OUTPUT_YAML_STYLE_VAR(WindowTitleAlign);
    OUTPUT_YAML_STYLE_VAR(ChildRounding);
    OUTPUT_YAML_STYLE_VAR(ChildBorderSize);
    OUTPUT_YAML_STYLE_VAR(PopupRounding);
    OUTPUT_YAML_STYLE_VAR(PopupBorderSize);
    OUTPUT_YAML_STYLE_VAR(FramePadding);
    OUTPUT_YAML_STYLE_VAR(FrameRounding);
    OUTPUT_YAML_STYLE_VAR(FrameBorderSize);
    OUTPUT_YAML_STYLE_VAR(ItemSpacing);
    OUTPUT_YAML_STYLE_VAR(ItemInnerSpacing);
    OUTPUT_YAML_STYLE_VAR(IndentSpacing);
    OUTPUT_YAML_STYLE_VAR(CellPadding);
    OUTPUT_YAML_STYLE_VAR(ScrollbarSize);
    OUTPUT_YAML_STYLE_VAR(ScrollbarRounding);
    OUTPUT_YAML_STYLE_VAR(GrabMinSize);
    OUTPUT_YAML_STYLE_VAR(GrabRounding);
    OUTPUT_YAML_STYLE_VAR(TabRounding);
    OUTPUT_YAML_STYLE_VAR(ButtonTextAlign);
    OUTPUT_YAML_STYLE_VAR(SelectableTextAlign);
    OUTPUT_YAML_STYLE_VAR(SeparatorTextBorderSize);
    OUTPUT_YAML_STYLE_VAR(SeparatorTextAlign);
    OUTPUT_YAML_STYLE_VAR(SeparatorTextPadding);
#ifdef IMGUI_HAS_DOCK
    OUTPUT_YAML_STYLE_VAR(DockingSeparatorSize);
#endif
    if (semanticColorData != nullptr)
    {
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, DestructiveColor);
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, DestructiveColorActive);
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, SuccessColor);
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, SuccessColorActive);
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, WarningColor);
        OUTPUT_YAML_SEMANTIC_COLOUR(semanticColorData, WarningColorActive);
    }

    out << YAML::EndMap;

    std::ofstream o(file);
    o << out.c_str();
    o.close();
}

void renderStyleVar(const char* name, float& t) noexcept
{
    ImGui::DragFloat(name, &t);
}

void renderStyleVar(const char* name, ImVec2& t) noexcept
{
    ImGui::DragFloat2(name, (float*)&t);
}

void UImGui::Theme::showThemeEditor(void* bOpen) noexcept
{
    if (ImGui::Begin("UntitledImGuiTheme Theme Editor", static_cast<bool*>(bOpen)))
    {
        showThemeEditorInline();
        ImGui::End();
    }
}

void UImGui::Theme::showThemeEditorInline() noexcept
{
    auto& style = ImGui::GetStyle();
    for (size_t i = 0; i < ImGuiCol_COUNT; i++)
        ImGui::ColorEdit4(colourStrings[i], (float*)&style.Colors[i]);

    RENDER_STYLE_VAR_EDIT(Alpha);
    RENDER_STYLE_VAR_EDIT(DisabledAlpha);
    RENDER_STYLE_VAR_EDIT(WindowPadding);
    RENDER_STYLE_VAR_EDIT(WindowRounding);
    RENDER_STYLE_VAR_EDIT(WindowBorderSize);
    RENDER_STYLE_VAR_EDIT(WindowMinSize);
    RENDER_STYLE_VAR_EDIT(WindowTitleAlign);
    RENDER_STYLE_VAR_EDIT(ChildRounding);
    RENDER_STYLE_VAR_EDIT(ChildBorderSize);
    RENDER_STYLE_VAR_EDIT(PopupRounding);
    RENDER_STYLE_VAR_EDIT(PopupBorderSize);
    RENDER_STYLE_VAR_EDIT(FramePadding);
    RENDER_STYLE_VAR_EDIT(FrameRounding);
    RENDER_STYLE_VAR_EDIT(FrameBorderSize);
    RENDER_STYLE_VAR_EDIT(ItemSpacing);
    RENDER_STYLE_VAR_EDIT(ItemInnerSpacing);
    RENDER_STYLE_VAR_EDIT(IndentSpacing);
    RENDER_STYLE_VAR_EDIT(CellPadding);
    RENDER_STYLE_VAR_EDIT(ScrollbarSize);
    RENDER_STYLE_VAR_EDIT(ScrollbarRounding);
    RENDER_STYLE_VAR_EDIT(GrabMinSize);
    RENDER_STYLE_VAR_EDIT(GrabRounding);
    RENDER_STYLE_VAR_EDIT(TabRounding);
    RENDER_STYLE_VAR_EDIT(ButtonTextAlign);
    RENDER_STYLE_VAR_EDIT(SelectableTextAlign);
    RENDER_STYLE_VAR_EDIT(SeparatorTextBorderSize);
    RENDER_STYLE_VAR_EDIT(SeparatorTextAlign);
    RENDER_STYLE_VAR_EDIT(SeparatorTextPadding);
#ifdef IMGUI_HAS_DOCK
    RENDER_STYLE_VAR_EDIT(DockingSeparatorSize);
#endif
}
