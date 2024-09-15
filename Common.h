#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32
    #ifdef MLS_EXPORT_LIBRARY
        #ifdef MLS_LIB_COMPILE
            #define MLS_PUBLIC_API __declspec(dllexport)
        #else
            #define MLS_PUBLIC_API __declspec(dllimport)
        #endif
    #else
        #define MLS_PUBLIC_API
    #endif
#else
    #define MLS_PUBLIC_API
#endif

    typedef struct MLS_PUBLIC_API UImGui_ThemeVec4
    {
        float x;
        float y;
        float z;
        float w;
    } UImGui_ThemeVec4;

    typedef struct MLS_PUBLIC_API UImGui_SemanticColourData
    {
        UImGui_ThemeVec4 DestructiveColor;
        UImGui_ThemeVec4 DestructiveColorActive;
        UImGui_ThemeVec4 SuccessColor;
        UImGui_ThemeVec4 SuccessColorActive;
        UImGui_ThemeVec4 WarningColor;
        UImGui_ThemeVec4 WarningColorActive;
    } UImGui_SemanticColourData;
#ifdef __cplusplus
}
#endif