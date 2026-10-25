# obs-streamelements-core — versión arreglada (fork)

> **Esto no es el repositorio oficial de StreamElements.** Es una copia (fork) de
> [StreamElements/obs-streamelements-core](https://github.com/StreamElements/obs-streamelements-core)
> con **un único arreglo**. Todo lo demás es igual que el original.

## 🇪🇸 Qué arregla

Con SE.Live instalado, **OBS se quedaba congelado** (sin responder, había que cerrarlo a la fuerza), a veces en mitad de un directo, cuando un lienzo extra, como el **vertical**, usaba transiciones de verdad:

- al activar «usar las transiciones de OBS» en el lienzo vertical;
- al abrir OBS con esa opción ya activada;
- al cambiar de escena muy rápido varias veces seguidas.

**Por qué pasaba:** al cambiar de escena, SE.Live se quedaba «agarrando» la escena actual mientras le pedía a OBS que empezara la transición. Al mismo tiempo, OBS terminaba la transición anterior y necesitaba esa misma escena. Cada uno esperaba al otro y ninguno avanzaba.

**El arreglo:** SE.Live suelta la escena antes de pedirle nada a OBS. El orden de las cosas y el comportamiento son los mismos; solo desaparece la espera infinita. Está en [`streamelements/SEVideoComposition.cpp`](streamelements/SEVideoComposition.cpp).

## 🇪🇸 Cómo instalarlo

1. Ten instalado SE.Live normal (la versión oficial) en OBS 33.
2. Descarga **SE.Live-arreglado.zip** de [Releases](../../releases/latest).
3. **Cierra OBS.**
4. Copia `obs-streamelements-core.dll` (y `.pdb`) del zip en la carpeta de plugins de OBS, normalmente
   `C:\Program Files\obs-studio\obs-plugins\64bit\`, y elige **Reemplazar**. Guarda antes una copia del archivo original por si quieres volver atrás.
5. Abre OBS.

Si actualizas SE.Live con su instalador oficial, se volverá a poner el archivo original: repite los pasos.

## 🇬🇧 In English

**Fixes:** OBS freezing (not crashing) with SE.Live when a custom canvas, such as the vertical one, uses a real transition: when enabling OBS transitions on the vertical canvas, on start-up with that option saved, and when switching scenes quickly. The cause was a lock-order deadlock between the UI thread (`SetCurrentScene` holding the current-scene lock across `obs_transition_start`) and the graphics thread (`transition_stop` → `GetCurrentSceneRef`). The fix releases the lock before calling into libobs.

**Install:** close OBS, copy `obs-streamelements-core.dll` from **SE.Live-arreglado.zip** ([Releases](../../releases/latest)) over the one in `C:\Program Files\obs-studio\obs-plugins\64bit\`, and start OBS. Keep a backup of the original. Built for OBS 33 (Windows x64), with crash reporting disabled.

---

*Original README below.*

# obs-streamelements-core

Core SE.Live OBS plugin.

# JavaScript OBS API

The host API this plugin exposes to page JavaScript — 217 `window.host` calls,
104 `window` events and methods, and 70 data structures — is documented in
**[`docs/api/`](docs/api/)**, accessible at <a href="https://streamelements.github.io/obs-streamelements-core/api/" target="_blank">https://streamelements.github.io/obs-streamelements-core/api/</a> currently at API version 6.6.

That tree is the source of truth. It replaces `JavaScript OBS API Version
6.6.docx`, which is not in version control; see
[`tools/docx-to-markdown/`](tools/docx-to-markdown/) for how it was converted
and verified.

# Building locally

This plugin does not build on its own. It is built as part of an obs-studio
tree, and it has a hard dependency on the `obs-browser` plugin being present
as a sibling — `StreamElementsBrowserWidget.cpp` includes
`../../obs-browser/panel/browser-panel.hpp` by relative path.

## Layout

```
obs-studio/
├── CMakeUserPresets.json          <- copy of this repo's CMakeUserPresets.json (Windows)
└── plugins/
    ├── CMakeLists.txt             <- must contain: add_obs_plugin(obs-streamelements-core)
    ├── obs-browser/               <- required
    └── obs-streamelements-core/   <- this repo
```

Clone obs-studio with `--recursive` so `obs-browser` comes along. CI pins
obs-studio to tag `31.1.2`; newer trees also work.

## Windows

CMake reads `CMakeUserPresets.json` only from the top-level source directory,
so the preset has to be copied up to the obs-studio root before configuring:

```cmd
copy plugins\obs-streamelements-core\CMakeUserPresets.json CMakeUserPresets.json
cmake --preset=windows-x64-streamelements
msbuild build_x64\obs-studio.sln -p:Configuration=RelWithDebInfo
```

The `windows-x64-streamelements` preset just inherits obs-studio's own
`windows-x64` preset, so configuring with `--preset=windows-x64` works too.

## macOS

`deps/BugSplat.xcframework.zip` must be unpacked first or CMake stops with a
fatal error:

```bash
(cd plugins/obs-streamelements-core/deps && unzip -o BugSplat.xcframework.zip)
cmake --preset=macos -DCMAKE_OSX_ARCHITECTURES=arm64 -B build_macos_arm64
cd build_macos_arm64 && xcodebuild -configuration RelWithDebInfo \
    -scheme obs-streamelements-core \
    -destination "generic/platform=macOS,name=Any Mac"
```

## Notes

- `streamelements/Version.generated.hpp` must define
  `STREAMELEMENTS_PLUGIN_VERSION`. A placeholder is committed so a fresh
  checkout compiles; CI overwrites it with the real build number. Don't edit
  it by hand.
- Don't copy the `-DENABLE_*=OFF` flags out of `.github/workflows/build.yml`.
  CI turns off nearly every OBS feature — including `-DENABLE_UI=OFF` —
  because it only needs the plugin binary as an artifact. For local work you
  want the OBS UI built so there is something to run the plugin in.
- There are no tests. Compiling is the only automated check, alongside
  `CI/check-format.sh`.
- Format only what you changed:
  `clang-format -i -style=file -fallback-style=none <file>`. Do not run
  `formatcode.sh` — it has no exclusions and rewrites all of `deps/` and
  `streamelements/deps/`.

# Release channels status

<a href="https://streamelements.github.io/obs-streamelements-core/status.html" target="_blank">Extended Status Page</a> — every channel on both platforms, with build dates, promotion drift and release notes, read live from the CDN manifests. Source: <a href="docs/status.html">docs/status.html</a>.

# Deployment

1. Make sure RELEASE_NOTES.md reflects the release content of the plug-in since previous release to the public (`latest` release group).

2. Once this repository is built, run the <a href="https://github.com/StreamElements/obs-streamelements-core/actions/workflows/release.yml" target="_blank">Release Signed Binaries</a> action in github.

# Proper deployment order

| From Environment | To Environment | Predicate |
|--- |--- |---
| `signed` | `qa` | none |
| `qa` | `beta` | passes internal and closed beta testing |
| `beta` | `latest` | 2 weeks without serious issues on `beta` |
| `latest` | `stable` | 2 weeks without stability issues on `latest` |

