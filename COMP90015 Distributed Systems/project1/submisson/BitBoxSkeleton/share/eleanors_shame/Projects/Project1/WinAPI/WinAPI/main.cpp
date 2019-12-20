#include <windows.h>
#include <cstdio>

#define ID_FILE_EXIT	100
#define ID_FILE_SAVE	101
#define ID_FILE_OPEN	102
#define IDC_MAIN_EDIT	201

#define ZeroMem(var) memset(&var, 0, sizeof(var))

HFONT hfDefault;
CONST CHAR gClassName[] = "WinClass";

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL LoadTextToEdit(HWND hEdit, LPCSTR pFileName);

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLIne, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hWnd;
	MSG msg;

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(0, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= gClassName;
	wc.hIconSm			= LoadIcon(0, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)) {
		MessageBox(0, "An error occurred.", "ERROR 1", MB_ICONERROR | MB_OK);
		return 1;
	}

	hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, gClassName, "NotePad--", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
							  0, 0, hInstance, 0);
	if(!hWnd) {
		MessageBox(0, "An error occurred.", "ERROR 2", MB_ICONERROR | MB_OK);
		return 2;
	}

	{
		HDC hdc;
		LONG lfHeight;

		hdc = GetDC(0);
		lfHeight = -MulDiv(10 , GetDeviceCaps(hdc, LOGPIXELSY), 72);
		ReleaseDC(0, hdc);
		DeleteObject((HGDIOBJ)hfDefault);
		hfDefault = CreateFont(lfHeight, 0, 0, 0, FW_DONTCARE, 0, 0, 0, 0, 0, 0, ANTIALIASED_QUALITY, FF_MODERN, "Courier New");

		if(!hfDefault) {
			MessageBox(hWnd, "An error occurred.", "ERROR 6", MB_ICONERROR | MB_OK);
		}

		SendDlgItemMessage(hWnd, IDC_MAIN_EDIT, WM_SETFONT, (WPARAM)hfDefault, 1);
	}
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&msg, hWnd, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_CREATE: {
				HMENU hMenu, hSubMenu;
				HICON hIcon, hIconSm;
				HWND hEdit;

				hMenu = CreateMenu();
				
				hSubMenu = CreatePopupMenu();
				AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, "&Open");
				AppendMenu(hSubMenu, MF_STRING, ID_FILE_SAVE, "&Save");
				AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0);
				AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
				AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, "&File");

				SetMenu(hWnd, hMenu);

				hIcon = (HICON)LoadImage(0, "C:\\C\\icon1.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
				if(hIcon) {
					SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
				}
				else {
					MessageBox(0, "An error occurred.", "ERROR 3", MB_ICONERROR | MB_OK);
				}

				hIconSm = (HICON)LoadImage(0, "C:\\C\\icon2.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
				if(hIconSm) {
					SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
				}
				else {
					MessageBox(0, "An error occurred.", "ERROR 4", MB_ICONERROR | MB_OK);
				}

				hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", 
						WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
						0, 0, 640, 480, hWnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(0), 0);
				if(!hEdit) {
					MessageBox(0, "An error occurred.", "ERROR 5", MB_ICONERROR | MB_OK);
				}

				SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(0, 0));
			}
			break;
		case WM_SIZE: {
				HWND hEdit;
				RECT rcClient;

				GetClientRect(hWnd, &rcClient);

				hEdit = GetDlgItem(hWnd, IDC_MAIN_EDIT);
				SetWindowPos(hEdit, 0, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
			}
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case ID_FILE_EXIT:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				case ID_FILE_SAVE: {
						int length = GetWindowTextLength(GetDlgItem(hWnd, IDC_MAIN_EDIT));
						if(length) {
							char *buf;

							buf = (char*)GlobalAlloc(GPTR, length + 1);
							GetDlgItemText(hWnd, IDC_MAIN_EDIT, buf, length + 1);

							OPENFILENAME ofn;
							char fileName[MAX_PATH] = "";

							ZeroMem(ofn);
							ofn.lStructSize = sizeof(ofn);
							ofn.hwndOwner	= hWnd;
							ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
							ofn.lpstrFile	= fileName;
							ofn.nMaxFile	= MAX_PATH;
							ofn.Flags		= OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
							ofn.lpstrDefExt = "txt";

							if(GetSaveFileName(&ofn)) {
								FILE *fp;
								fopen_s(&fp, fileName, "w+");
								fprintf_s(fp, "%s", buf);
								fclose(fp);
							}

							GlobalFree((HGLOBAL)buf);
						}
					}
					break;
				case ID_FILE_OPEN: {
						OPENFILENAME ofn;
						char fileName[MAX_PATH] = "";
						ZeroMem(ofn);
						ofn.lStructSize = sizeof(ofn);
						ofn.hwndOwner	= hWnd;
						ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
						ofn.lpstrFile	= fileName;
						ofn.nMaxFile	= MAX_PATH;
						ofn.Flags		= OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
						ofn.lpstrDefExt = "txt";

						if(GetOpenFileName(&ofn)) {
							LoadTextToEdit(GetDlgItem(hWnd, IDC_MAIN_EDIT), fileName);
						}
					}
					break;

			}
			break;
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
BOOL LoadTextToEdit(HWND hEdit, LPCSTR pFileName) {
	HANDLE hFile;
	BOOL success = FALSE;

	hFile = CreateFile(pFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(hFile != INVALID_HANDLE_VALUE) {
		DWORD dwFileSize;
		dwFileSize = GetFileSize(hFile, 0);
		if(dwFileSize != 0xFFFFFFFF) {
			LPSTR pFileText;

			pFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if(pFileText) {
				DWORD dwRead;

				if(ReadFile(hFile, pFileText, dwFileSize, &dwRead, 0)) {
					pFileText[dwFileSize] = '\0';
					if(SetWindowText(hEdit, pFileText)) {
						success = TRUE;
					}
				}
				GlobalFree((HGLOBAL)pFileText);
			}
		}
		CloseHandle(hFile);
	}
	return success;
}