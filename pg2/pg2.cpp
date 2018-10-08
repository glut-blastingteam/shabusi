#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string.h>
#define APP_NAME "pg2"
#define PROTECT_APP_NAME "pg1"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HANDLE mutex;
    mutex = CreateMutex(NULL, TRUE, PROTECT_APP_NAME);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(mutex);
    }
    else {
        char exec[20] = { '\0' };
        strcat(exec, PROTECT_APP_NAME);
        strcat(exec, ".exe");
        CloseHandle(mutex);
        WinExec(exec, SW_HIDE);
    }
    switch (message) {
    case WM_CREATE:
        SetTimer(hwnd, 1, 10, NULL);
        return 0;

    case WM_TIMER:
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    HANDLE mutex;
    mutex = CreateMutex(NULL, TRUE, APP_NAME);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBox(NULL, "The same program was already existed", "Startup Error", MB_OK);
        CloseHandle(mutex);
        return 0;
    }

    WNDCLASS wndclass;
    wndclass.style = CS_NOCLOSE;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = APP_NAME;
    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, "Failure to regist class", "Startup Error", MB_OK);
        return 0;
    }

    CreateWindow(APP_NAME, "TEST", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}


