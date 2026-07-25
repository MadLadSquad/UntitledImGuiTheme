# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

UntitledImGuiTheme is a standalone MIT-licensed library by [MadLadSquad](https://madladsquad.com) that serialises a full `ImGuiStyle` (colours + style vars) to/from YAML and ships a built-in theme editor widget. End-user docs live on the [wiki](https://github.com/MadLadSquad/UntitledImGuiTheme/wiki/Home).

The whole library is four files: `UTheme.hpp` / `UTheme.cpp` (C++ API), `C/CUTheme.h` / `C/CUTheme.cpp` (thin C wrapper), plus `Common.h` (C-compatible POD types and the `MLS_PUBLIC_API` export macro shared by both). `rapidyaml/` is a git submodule pointing at the MadLadSquad rapidyaml fork.

This checkout is usually a submodule of [UntitledImGuiFramework](https://github.com/MadLadSquad/UntitledImGuiFramework) at `Framework/Modules/Theming/ThirdParty/UntitledImGuiTheme`, but the library has no framework dependency — only dear imgui and ryml.

## Building

There is **no build system in this repository** — `CMakeLists.txt` and `main.cpp` are in `.gitignore` (scratch files for local testing). Consumers compile the sources directly:

- Compile `UTheme.cpp` (and `C/CUTheme.cpp` if the C API is wanted) into the consuming target.
- `<imgui.h>` **must** be resolvable on the include path — `UTheme.hpp` `#error`s at preprocess time otherwise.
- `<ryml.hpp>` must be resolvable; link the `ryml` target from the `rapidyaml/` submodule (or a system ryml of a compatible version).
- Define `MLS_EXPORT_LIBRARY` (+ `MLS_LIB_COMPILE` while compiling the library itself) only when building a Windows shared library; otherwise `MLS_PUBLIC_API` expands to nothing.

When developing inside the framework, build a project that has `theming: true` in its `uvproj.yaml`; the framework's `Framework/cmake/SetupSources.cmake` globs `UTheme.cpp` + `C/CUTheme.cpp` into the framework library under `USE_THEME_MODULE` and defines `UIMGUI_THEME_MODULE_ENABLED`. `ModulesManager::applyCustomisations()` then calls `Theme::load("<configDir>/Theme/<Modules::data().themeLocation>.theme.yaml")`.

There is no test suite and CI runs no build. `.github/workflows/update.yml` bumps submodules on a schedule and opens an auto-issue; `release.yaml` tarballs the tree on `v*` tags.

## Architecture

### The three parallel tables

`ImGuiStyle` is walked three separate times, once per operation, each by a different macro over the same list of member names in the same order:

| Operation | Macro | Backing helper |
|---|---|---|
| `Theme::load` | `LOAD_YAML_STYLE_VAR(x)` | `parse_style_var<T>` (+ `ImVec2` specialisation) |
| `Theme::save` | `OUTPUT_YAML_STYLE_VAR(x)` | `emit_style_var<T>` (+ `ImVec2` specialisation) |
| `Theme::showThemeEditorInline` | `RENDER_STYLE_VAR_EDIT(x)` | `renderStyleVar` overloads (`float`/`ImVec2`/`ImGuiDir`/`bool`/`int`) |

**Adding or renaming a style var means editing all three lists**, keeping them in the same order. Nothing enforces the correspondence — a member missing from one list silently stops round-tripping. `#ifdef IMGUI_HAS_DOCK` guards appear in all three for the docking-only vars.

Type dispatch is by `if constexpr (std::is_enum_v<T>)` (round-tripped through the underlying integer) with explicit `ImVec2` template specialisations; enum members reaching `renderStyleVar` need an overload that matches, which is why `ImGuiDir` has its own combo box.

### colourStrings

`Theme::colourStrings` (private, in `UTheme.hpp`) is the name table for `ImGuiStyle::Colors`. All three operations iterate `for (i = 0; i < ImGuiCol_COUNT; i++)` and index straight into it, so:

- The first `ImGuiCol_COUNT` entries must match the `ImGuiCol_` enum **exactly in order** — it is a positional mapping, not a lookup. As of the pinned imgui fork that is 63 entries (with `IMGUI_HAS_DOCK`).
- Entries are `#ifdef IMGUI_HAS_DOCK`-guarded at the docking positions so the indices stay aligned in non-docking builds.
- Do **not** count the obsolete aliases declared after `ImGuiCol_COUNT` in `imgui.h` (`ImGuiCol_TabActive`, `ImGuiCol_NavHighlight`, …) when checking the list against a new imgui version.
- The six trailing semantic names (`DestructiveColor` … `WarningColorActive`) sit past `ImGuiCol_COUNT` and are never indexed — they document the YAML keys and act as a buffer. Too *few* imgui entries would make the loops silently read these instead of erroring.

### Semantic colours

`SemanticColourData` is application-level colour data with no `ImGuiStyle` slot (destructive/success/warning + their active variants). It is passed in/out by pointer through the optional last parameter of `load`/`save`, handled by its own `LOAD_YAML_SEMANTIC_COLOUR` / `OUTPUT_YAML_SEMANTIC_COLOUR` macros keyed by name rather than index, and is skipped entirely when the pointer is null. Its type lives in `Common.h` (as `UImGui_SemanticColourData`, aliased to `UImGui::SemanticColourData`) so the C and C++ APIs share one layout.

### YAML layer

Uses the MadLadSquad rapidyaml fork's node API, **not** the legacy `>>` / `<<` operators (those are `RYML_LEGACY_OPERATOR`-deprecated; several commits here exist purely to track ryml API changes). Writing uses `node.save(v)`. Reading uses `node.deserialize(&v)` rather than `node.load(&v)`: `load()` routes a scalar it cannot convert to the error callback, whereas `deserialize()` reports it in the returned `ReadResult`, which is what lets a hand-broken value leave the style member alone. Reading goes through `ryml::parse_in_place` on a `std::string` read from the file, so the buffer must outlive the tree. Vectors are emitted with `ryml::FLOW_SL` so they serialise as `[x, y, z, w]` on one line.

Every lookup goes through `root.find_child(key)` and every read checks `readable()` (plus `has_val()` for scalars, `is_seq()`/`num_children()` for vectors) before touching the node. This is load-bearing rather than defensive: `operator[]` and the type queries only *assert* on a missing or ill-typed node when `RYML_USE_ASSERT` is on, so the same access reads out of bounds in a release build. The checks are what make a partial theme file leave the remaining style members at their current values.

Both entry points are `noexcept` and report failure by returning `-1`: `load()` for a file that cannot be read, is empty, or whose root is not a map, and `save()` for a file that cannot be written.

**This library must build without exceptions** — no `throw`, no `try`, and nothing that requires unwinding, since upstream forbids them. Every error is therefore a check on a value: `is_open()` on the stream, `tree.empty()` and `is_map()` on the parse result, `readable()`/`has_val()`/`is_seq()` before each read, the `ReadResult` returned by `deserialize()`, and `good()` on the output stream.

The one failure that cannot be checked this way is a YAML **syntax** error, which ryml reports mid-parse by calling a global error handler that is required never to return; its default aborts the process, so a theme file with broken syntax takes the application down. Interrupting it needs either an exception or `longjmp`, and `longjmp` out of ryml's frames is undefined behaviour (non-trivial destructors) as well as leaking the parse arena — so neither is used here, matching what the rest of the framework does with its own ryml configs. The library never installs or resets ryml's callbacks, so an application that wants softer handling can install its own global handler and keep it.

### C API

`C/CUTheme.cpp` is a 1:1 forwarding wrapper (`UImGui_Theme_<method>` → `UImGui::Theme::<method>`) with no defaulted arguments. Any new public C++ method should get a matching `MLS_PUBLIC_API` C entry point, and any new shared struct belongs in `Common.h` as a `UImGui_`-prefixed POD with a `typedef` alias added in the `UImGui` namespace in `UTheme.hpp`.
