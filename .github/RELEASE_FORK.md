Versión arreglada de SE.Live: **OBS ya no se queda congelado** al usar transiciones en el lienzo vertical, ni al abrir OBS con esa opción activa, ni al cambiar de escena muy rápido.

### Cómo instalarlo (fácil)
1. Ten instalado SE.Live oficial en OBS 33 (Windows).
2. **Cierra OBS.**
3. Descarga **SE.Live-arreglado.zip** y descomprímelo.
4. Abre **Instalar arreglo.exe** y acepta el permiso de administrador. Guarda una copia del original (`.bak`) y pone el arreglo.
5. Abre OBS.

### A mano
Copia `obs-streamelements-core.dll` (y `.pdb`) en `C:\Program Files\obs-studio\obs-plugins\64bit\`, eligiendo **Reemplazar**. Guarda antes una copia del original.

Si actualizas SE.Live con su instalador oficial, vuelve a hacer estos pasos.

---
**English:** fixes OBS freezing with SE.Live when the vertical (custom) canvas uses OBS transitions. Close OBS and run `Instalar arreglo.exe` from the zip (or copy `obs-streamelements-core.dll` over the one in `C:\Program Files\obs-studio\obs-plugins\64bit\` by hand), then start OBS. Based on SE.Live 26.10.6.1039, built for OBS 33 (x64). This is an unofficial fork; not affiliated with StreamElements.
