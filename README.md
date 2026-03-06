# UMGAutoBuilder

Config-driven UMG (`WidgetBlueprint`) scaffold generation for Unreal Engine (Editor-only).

This plugin is designed to be **portable**: it does not depend on any game-specific module. You can extract `Plugins/UMGAutoBuilder/` into another UE project and rebuild.

## Positioning

`UMGAutoBuilder` is best used as a **structured UMG scaffold generator**:

- generate or refresh panel-style widget skeletons from JSON
- export an existing widget into a machine-editable JSON starting point
- apply conservative patch operations to named widgets

It is **not** intended to be a perfect round-trip replacement for every complex `WidgetBlueprint`. The recommended workflow is: generate the structure first, then let UI or Blueprint authors continue refining the asset in the editor.

## Features

- **Build mode**: create or rebuild a `WidgetBlueprint` from a JSON spec (widget tree + properties + slots).
- **Patch mode**: update only specific widgets / append children into an existing widget tree (no full rebuild).
- **Export mode**: export an existing widget tree into a JSON spec (useful as a starting point or for AI-assisted iteration).
- **Assertions**: optional `assertWidgets` list to verify that critical widget names exist after generation.
- **Editor workbench**: run Build / Patch / Export from a lightweight editor tab.

## Requirements

- Unreal Engine **5.3+** (tested with 5.3)
- Editor build (this is an **Editor module**, not runtime)

## Commandlets

### 1) Build / Patch

Run:

```powershell
UnrealEditor-Cmd.exe "<YourProject>.uproject" `
  -run=UMGAutoBuild `
  -Config="<path-to-json>" `
  -NoAssetRegistryCache -unattended -nop4 -nosplash
```

Optional params:

- `-Mode=build|patch` (default: `build`)

### 2) Export

Run:

```powershell
UnrealEditor-Cmd.exe "<YourProject>.uproject" `
  -run=UMGAutoExport `
  -Widget="/Game/UI/WB_Example.WB_Example" `
  -Out="C:/tmp/WB_Example.json" `
  -Pretty `
  -NoAssetRegistryCache -unattended -nop4 -nosplash
```

Params:

- `-Widget=`: widget blueprint asset path
- `-Out=`: output JSON file path
- `-Pretty` (optional): pretty-printed JSON

## Editor Workbench

After enabling the plugin, open:

- `Tools -> UMG Auto Builder`

The workbench provides:

- a `Config Path` field for `Build` / `Patch`
- a `Widget Path` and `Export Output Path` for `Export`
- an inline result panel for warnings and errors

## JSON Schema (high level)

Top-level:

```json
{
  "schemaVersion": 1,
  "targetWidget": "/Game/UI/WB_Example.WB_Example",
  "createIfMissing": true,
  "mode": "build",
  "assertWidgets": ["RootCanvas", "TitleText"],
  "root": { "type": "CanvasPanel", "name": "RootCanvas", "children": [] },
  "patch": {
    "setWidgetProps": [
      { "name": "TitleText", "props": { "text": "Hello" } }
    ],
    "ensureChildren": [
      {
        "parent": "SomeWrapBox",
        "children": [
          { "type": "TextBlock", "name": "NewChild", "props": { "text": "X" } }
        ]
      }
    ]
  }
}
```

Node fields:

- `type`: `CanvasPanel | Overlay | VerticalBox | HorizontalBox | WrapBox | SizeBox | Border | Image | TextBlock | ProgressBar | Spacer | UserWidget`
- `name`: widget name (`FName`)
- `props`: widget properties (depends on type)
- `slot`: parent slot properties (depends on parent panel)
- `children`: array of child nodes (for panel widgets)

Top-level fields:

- `schemaVersion`: JSON schema version (current: `1`)
- `targetWidget`: target widget blueprint asset path
- `createIfMissing`: create the target asset if it does not exist
- `mode`: `build` or `patch`
- `assertWidgets`: optional widget names that must exist after apply
- `root`: root node used by `build`
- `patch`: patch operations used by `patch`

`UserWidget` props:

- `class`: soft class path, e.g. `"/Game/UI/WB_Chip.WB_Chip_C"`

## Notes

- Build mode uses a **fresh WidgetTree** each run to avoid UE name/class collisions when regenerating.
- Patch mode intentionally stays conservative: it only updates named widgets and/or appends specified children.
- Export/Build preserve the common slot alignment and size fields used by `HorizontalBoxSlot`, `VerticalBoxSlot`, `OverlaySlot`, and `WrapBoxSlot`.
- Unknown top-level, node, and patch fields are reported as warnings to make malformed specs easier to diagnose.

