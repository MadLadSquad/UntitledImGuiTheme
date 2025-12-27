#include "CUTheme.h"
#include "../UTheme.hpp"

int UImGui_Theme_load(const char* file, UImGui_SemanticColourData* semanticColourData)
{
    return UImGui::Theme::load(file, semanticColourData);
}

void UImGui_Theme_save(const char* save, UImGui_SemanticColourData* semanticColourData)
{
    UImGui::Theme::save(save, semanticColourData);
}

void UImGui_Theme_showThemeEditor(void* bOpen)
{
    UImGui::Theme::showThemeEditor(bOpen);
}

void UImGui_Theme_showThemeEditorInline()
{
    UImGui::Theme::showThemeEditorInline();
}