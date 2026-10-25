// Instala el arreglo: copia obs-streamelements-core.dll/.pdb (junto a este
// .exe) en la carpeta de plugins de OBS, guardando antes el original.
#include <windows.h>
#include <tlhelp32.h>
#include <wchar.h>

#define TITLE L"SE.Live arreglado"

static void msg(const wchar_t *text, UINT icon)
{
	MessageBoxW(NULL, text, TITLE, MB_OK | icon);
}

static BOOL obs_running(void)
{
	PROCESSENTRY32W pe = {sizeof(pe)};
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL found = FALSE;
	if (snap == INVALID_HANDLE_VALUE)
		return FALSE;
	for (BOOL ok = Process32FirstW(snap, &pe); ok && !found;
	     ok = Process32NextW(snap, &pe))
		found = _wcsicmp(pe.szExeFile, L"obs64.exe") == 0;
	CloseHandle(snap);
	return found;
}

int WINAPI wWinMain(HINSTANCE h, HINSTANCE p, PWSTR cmd, int show)
{
	wchar_t obs[MAX_PATH] = L"C:\\Program Files\\obs-studio";
	wchar_t here[MAX_PATH], plugins[MAX_PATH], src[MAX_PATH],
		dst[MAX_PATH], bak[MAX_PATH];
	DWORD size = sizeof(obs);

	// La carpeta de OBS la guarda su instalador en el registro.
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\OBS Studio", NULL,
		     RRF_RT_REG_SZ, NULL, obs, &size);
	swprintf(plugins, MAX_PATH, L"%ls\\obs-plugins\\64bit", obs);
	swprintf(dst, MAX_PATH, L"%ls\\obs-streamelements-core.dll", plugins);

	if (GetFileAttributesW(dst) == INVALID_FILE_ATTRIBUTES) {
		msg(L"No encuentro SE.Live instalado en OBS.\n\n"
		    L"Instala primero SE.Live oficial y vuelve a abrir este programa.",
		    MB_ICONERROR);
		return 1;
	}

	while (obs_running())
		if (MessageBoxW(NULL,
				L"OBS está abierto.\n\nCiérralo y pulsa «Reintentar».",
				TITLE, MB_RETRYCANCEL | MB_ICONWARNING) !=
		    IDRETRY)
			return 1;

	GetModuleFileNameW(NULL, here, MAX_PATH);
	*wcsrchr(here, L'\\') = 0;

	// Copia de seguridad del original, solo si aún no hay una.
	swprintf(bak, MAX_PATH, L"%ls.bak", dst);
	CopyFileW(dst, bak, TRUE);

	const wchar_t *files[] = {L"obs-streamelements-core.dll",
				  L"obs-streamelements-core.pdb"};
	for (int i = 0; i < 2; i++) {
		swprintf(src, MAX_PATH, L"%ls\\%ls", here, files[i]);
		swprintf(dst, MAX_PATH, L"%ls\\%ls", plugins, files[i]);
		if (!CopyFileW(src, dst, FALSE)) {
			wchar_t text[MAX_PATH * 2];
			swprintf(text, MAX_PATH * 2,
				 L"No he podido copiar:\n%ls\n\n"
				 L"Comprueba que el zip está descomprimido y que "
				 L"este programa está en la misma carpeta que el .dll.",
				 src);
			msg(text, MB_ICONERROR);
			return 1;
		}
	}

	msg(L"¡Arreglo instalado!\n\nYa puedes abrir OBS.", MB_ICONINFORMATION);
	return 0;
}
