#include <assert.h>
#include <stdlib.h>
#include <cstring>
#include <direct.h>
#include <windows.h>
#include "graphics.h"
#include "image.h"
#include "macro.h"
#include "platform.h"
#include "private.h"

struct Window {
    HWND handle;
    HDC memoryDC;
    image_t *surface;
    HWND textHandle;
    float textWidth;
    float textHeight;
    /* Common Data */
    int shouldClose;
    char keys[KEY_NUM];
    char buttons[BUTTON_NUM];
    Callbacks callbacks;
    void* userdata;
};

/* Platform Initialization */

static int initialized = 0;

#ifdef UNICODE
static const wchar_t* const WINDOW_CLASS_NAME = L"Class";
static const wchar_t* const WINDOW_ENTRY_NAME = L"Entry";
#else
static const char* const WINDOW_CLASS_NAME = "Class";
static const char* const WINDOW_ENTRY_NAME = "Entry";
#endif

/*
 * For virtual-key codes, see
 * https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
 */
static void handleKeyMessage(Window *window, WPARAM virtualKey, char pressed) {
    Keycode key;
    switch (virtualKey) {
        case 'A':      key = KEY_A;     break;
        case 'D':      key = KEY_D;     break;
        case 'S':      key = KEY_S;     break;
        case 'W':      key = KEY_W;     break;
        case 'Q':      key = KEY_Q;     break;
        case 'E':      key = KEY_E;     break;
        case VK_SPACE: key = KEY_SPACE; break;
        default:       key = KEY_NUM;   break;
    }
    if (key < KEY_NUM) {
        window->keys[key] = pressed;
        if (window->callbacks.keyCallback) {
            window->callbacks.keyCallback(window, key, pressed);
        }
    }
}

static void handleButtonMessage(Window *window, Button button, char pressed) {
    window->buttons[button] = pressed;
    if (window->callbacks.buttonCallback) {
        window->callbacks.buttonCallback(window, button, pressed);
    }
}

static void handleScrollMessage(Window *window, float offset) {
    if (window->callbacks.scrollCallback) {
        window->callbacks.scrollCallback(window, offset);
    }
}

static LRESULT CALLBACK processMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                                       LPARAM lParam) {
    Window *window = (Window*)GetProp(hWnd, WINDOW_ENTRY_NAME);
    if (window == NULL) {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    } else if (uMsg == WM_CLOSE) {
        window->shouldClose = 1;
        return 0;
    } else if (uMsg == WM_KEYDOWN) {
        handleKeyMessage(window, wParam, 1);
        return 0;
    } else if (uMsg == WM_KEYUP) {
        handleKeyMessage(window, wParam, 0);
        return 0;
    } else if (uMsg == WM_LBUTTONDOWN) {
        handleButtonMessage(window, BUTTON_L, 1);
        return 0;
    } else if (uMsg == WM_RBUTTONDOWN) {
        handleButtonMessage(window, BUTTON_R, 1);
        return 0;
    } else if (uMsg == WM_LBUTTONUP) {
        handleButtonMessage(window, BUTTON_L, 0);
        return 0;
    } else if (uMsg == WM_RBUTTONUP) {
        handleButtonMessage(window, BUTTON_R, 0);
        return 0;
    } else if (uMsg == WM_MOUSEWHEEL) {
        float offset = GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
        handleScrollMessage(window, offset);
        return 0;
    } else {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

static void registerClass(void) {
    ATOM classAtom;
    WNDCLASS windowClass;
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = processMessage;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandle(NULL);
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = WINDOW_CLASS_NAME;
    classAtom = RegisterClass(&windowClass);
    assert(classAtom != 0);
    UNUSED_VAR(classAtom);
}

static void unregisterClass(void) {
    UnregisterClass(WINDOW_CLASS_NAME, GetModuleHandle(NULL));
}

static void initializePath(void) {
#ifdef UNICODE
    wchar_t path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    *wcsrchr(path, L'\\') = L'\0';
    _wchdir(path);
    _wchdir(L"assets");
#else
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    *strrchr(path, '\\') = '\0';
    _chdir(path);
    _chdir("assets");
#endif
}

void platformInitialize(void) {
    assert(initialized == 0);
    registerClass();
    initializePath();
    initialized = 1;
}

void platformTerminate(void) {
    assert(initialized == 1);
    unregisterClass();
    initialized = 0;
}

/* Window Related Functions */

static HWND createWindow(const char *title_, int width, int height) {
    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT rect;
    HWND handle;

#ifdef UNICODE
    wchar_t title[LINE_SIZE];
    mbstowcs(title, title_, LINE_SIZE);
#else
    const char *title = title_;
#endif

    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    AdjustWindowRect(&rect, style, 0);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    handle = CreateWindow(WINDOW_CLASS_NAME, title, style,
                          CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                          NULL, NULL, GetModuleHandle(NULL), NULL);
    assert(handle != NULL);
    return handle;
}

/*
 * For memory device context, see
 * https://docs.microsoft.com/en-us/windows/desktop/gdi/memory-device-contexts
 */
static void createSurface(HWND handle, int width, int height,
                          image_t **outSurface, HDC *outMemoryDC) {
    BITMAPINFOHEADER biHeader;
    HBITMAP dibBitmap;
    HBITMAP oldBitmap;
    HDC windowDC;
    HDC memoryDC;
    image_t *surface;

    surface = imageCreate(width, height, 4, FORMAT_LDR);
    free(surface->ldrBuffer);
    surface->ldrBuffer = NULL;

    windowDC = GetDC(handle);
    memoryDC = CreateCompatibleDC(windowDC);
    ReleaseDC(handle, windowDC);

    memset(&biHeader, 0, sizeof(BITMAPINFOHEADER));
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biWidth = width;
    biHeader.biHeight = -height;  /* top-down */
    biHeader.biPlanes = 1;
    biHeader.biBitCount = 32;
    biHeader.biCompression = BI_RGB;
    dibBitmap = CreateDIBSection(memoryDC, (BITMAPINFO*)&biHeader,
                                  DIB_RGB_COLORS, (void**)&surface->ldrBuffer,
                                  NULL, 0);
    assert(dibBitmap != NULL);
    oldBitmap = (HBITMAP)SelectObject(memoryDC, dibBitmap);
    DeleteObject(oldBitmap);

    *outSurface = surface;
    *outMemoryDC = memoryDC;
}

Window* windowCreate(const char *title, int width, int height, int textWidth, int textHeight) {
    Window *window;
    HWND handle;
    image_t *surface;
    HDC memoryDC;
    HWND textHandle;

    assert(initialized && width > 0 && height > 0);

    handle = createWindow(title, width, height);
    textHandle = CreateWindow(TEXT("static"), TEXT("static"), WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        0, 0, textWidth, textHeight, handle, HMENU(21), GetModuleHandle(NULL), NULL);
    SetBkColor(GetDC(textHandle), RGB(0, 0, 0));
    SetTextColor(GetDC(textHandle), RGB(255, 0, 0));
    createSurface(handle, width, height, &surface, &memoryDC);

    window = (Window*)malloc(sizeof(Window));
    memset(window, 0, sizeof(Window));
    window->handle = handle;
    window->memoryDC = memoryDC;
    window->surface = surface;
    window->textWidth = textWidth;
    window->textHeight = textHeight;
    window->textHandle = textHandle;

    SetProp(handle, WINDOW_ENTRY_NAME, window);
    ShowWindow(handle, SW_SHOW);
    return window;
}

void windowDestroy(Window *window) {
    ShowWindow(window->handle, SW_HIDE);
    RemoveProp(window->handle, WINDOW_ENTRY_NAME);

    DeleteDC(window->memoryDC);
    DestroyWindow(window->handle);

    window->surface->ldrBuffer = NULL;
    imageRelease(window->surface);
    free(window);
}

int windowShouldClose(Window *window) {
    return window->shouldClose;
}

void windowSetUserdata(Window *window, void *userdata) {
    window->userdata = userdata;
}

void *windowGetUserdata(Window *window) {
    return window->userdata;
}

static void presentSurface(Window *window) {
    HDC windowDC = GetDC(window->handle);
    HDC memoryDC = window->memoryDC;
    image_t *surface = window->surface;
    int width = surface->width;
    int height = surface->height;
    BitBlt(windowDC, 0, 0, width, height, memoryDC, 0, 0, SRCCOPY);
    ReleaseDC(window->handle, windowDC);
}

void windowDrawBuffer(Window *window, Framebuffer *buffer) {
    privateBlitBgr(buffer, window->surface);
    presentSurface(window);
}

void windowDrawText(Window* window, char* text) {
#ifdef UNICODE
    wchar_t* wc;
    int len = MultiByteToWideChar(CP_ACP, 0, text, strlen(text), NULL, 0);
    wc = new wchar_t[len + 1];
    MultiByteToWideChar(CP_ACP, 0, text, strlen(text), wc, len);
    wc[len] = '\0';
    SetWindowText(window->textHandle, wc);
    delete[] wc;
#else
    SetWindowText(window->textHandle, text);
#endif // !UNICODE
}

/* Input Related Functions */

void inputPollEvents(void) {
    MSG message;
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

int inputKeyPressed(Window *window, Keycode key) {
    assert(key >= 0 && key < KEY_NUM);
    return window->keys[key];
}

int inputButtonPressed(Window *window, Button button) {
    assert(button >= 0 && button < BUTTON_NUM);
    return window->buttons[button];
}

void inputQueryCursor(Window *window, double *xpos, double *ypos) {
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(window->handle, &point);
    *xpos = (float)point.x;
    *ypos = (float)point.y;
}

void inputSetCallbacks(Window *window, Callbacks callbacks) {
    window->callbacks = callbacks;
}

/* Misc Platform Functions */

static double getNativeTime(void) {
    static double period = -1;
    LARGE_INTEGER counter;
    if (period < 0) {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        period = 1 / (double)frequency.QuadPart;
    }
    QueryPerformanceCounter(&counter);
    return counter.QuadPart * period;
}

double platformGetTime(void) {
    static double initial = -1;
    if (initial < 0) {
        initial = getNativeTime();
    }
    return getNativeTime() - initial;
}