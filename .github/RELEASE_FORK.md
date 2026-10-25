Versión arreglada de SE.Live: **OBS ya no se queda congelado** al usar transiciones en el lienzo vertical, ni al abrir OBS con esa opción activa, ni al cambiar de escena muy rápido.

### Cómo instalarlo
1. Ten instalado SE.Live oficial en OBS 33 (Windows).
2. **Cierra OBS.**
3. Descomprime este zip y copia `obs-streamelements-core.dll` (y `.pdb`) en `C:\Program Files\obs-studio\obs-plugins\64bit\`, eligiendo **Reemplazar**. Guarda antes una copia del original.
4. Abre OBS.

Si actualizas SE.Live con su instalador oficial, vuelve a hacer estos pasos.

---
**English:** fixes OBS freezing with SE.Live when the vertical (custom) canvas uses OBS transitions. Close OBS, copy `obs-streamelements-core.dll` over the one in `C:\Program Files\obs-studio\obs-plugins\64bit\`, start OBS. Based on SE.Live 26.10.6.1039, built for OBS 33 (x64). This is an unofficial fork; not affiliated with StreamElements.
