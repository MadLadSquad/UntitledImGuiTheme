#pragma once
#include "../Common.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// Last argument defaults to `nullptr` in the C++ API
 	MLS_PUBLIC_API int UImGui_Theme_load(const char* file, UImGui_SemanticColourData* semanticColourData);

	// Last argument defaults to `nullptr` in the C++ API
	MLS_PUBLIC_API void UImGui_Theme_save(const char* save, UImGui_SemanticColourData* semanticColourData);

	MLS_PUBLIC_API void UImGui_Theme_showThemeEditor(void* bOpen);
	MLS_PUBLIC_API void UImGui_Theme_showThemeEditorInline();
#ifdef __cplusplus
}
#endif