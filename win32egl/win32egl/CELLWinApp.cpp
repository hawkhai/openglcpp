#include "CELLWinApp.h"
#include <tchar.h>
#include <stdio.h>

#include <EGL/egl.h>
#include <gles2/gl2.h>

int CELLWinApp::main(int width, int height) {
    m_hWnd = CreateWindowEx(NULL, // dwExStyle
        _T("CELLWinApp"), // lpClassName
        _T("CELLWinApp"), // lpWindowName
        WS_OVERLAPPEDWINDOW, // dwStyle
        CW_USEDEFAULT, // X
        CW_USEDEFAULT, // Y
        width, // nWidth
        height, // nHeight
        NULL, // hWndParent
        NULL, // hMenu
        m_hInstance, // hInstance
        this // lpParam
    );
    if (m_hWnd == 0) {
        return -1;
    }

    ShowWindow(m_hWnd, SW_SHOW);
    if (!initGLES()) {
        return  -2;
    }

    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (msg.message == WM_DESTROY ||
            msg.message == WM_CLOSE)
        {
            break;
        }
        /**
        * 有消息，处理消息，无消息，则进行渲染绘制。
        */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            render();
            eglSwapBuffers(m_display, m_surface);
            Sleep(1);
        }
    }

    destroyGLES();
    return  0;
}

void CELLWinApp::render()
{
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_width, m_height);
}

bool CELLWinApp::initGLES() {
    const EGLint attribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_OPENGL_ES2_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE,24,
        EGL_NONE
    };

    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    EGLint major;
    EGLint minor;
    eglInitialize(m_display, &major, &minor);

    EGLint numConfigs(0);
    eglChooseConfig(m_display, attribs, &m_config, 1, &numConfigs);
    
    EGLint format(0);
    eglGetConfigAttrib(m_display, m_config, EGL_NATIVE_VISUAL_ID, &format);

    m_surface = eglCreateWindowSurface(m_display, m_config, m_hWnd, NULL);
    EGLint attr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    m_context = eglCreateContext(m_display, m_config, 0, attr);

    if (eglMakeCurrent(m_display, m_surface, m_surface, m_context) == EGL_FALSE) {
        return false;
    }
    eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_width);
    eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_height);
    return true;
}

void CELLWinApp::destroyGLES() {
    if (m_display != EGL_NO_DISPLAY) {
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (m_context != EGL_NO_CONTEXT) {
            eglDestroyContext(m_display, m_context);
        }
        if (m_surface != EGL_NO_SURFACE) {
            eglDestroySurface(m_display, m_surface);
        }
        eglTerminate(m_display);
    }
    m_display = EGL_NO_DISPLAY;
    m_context = EGL_NO_CONTEXT;
    m_surface = EGL_NO_SURFACE;
}

LRESULT CELLWinApp::onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CHAR:
        char szChar[20];
        sprintf_s(szChar, 20, "char code is %d", wParam);
        MessageBoxA(hWnd, szChar, "char", 0);
        break;
    case WM_LBUTTONDOWN:
        MessageBoxA(hWnd, "mouse clicked", "message", 0);
        break;
    case WM_CLOSE:
    case WM_DESTROY:
    {
        ::PostQuitMessage(0);
    }
    break;
    case WM_MOUSEMOVE:
        break;
    default:
        return  DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return  S_OK;

}

LRESULT CALLBACK CELLWinApp::lpfnWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    CELLWinApp* pThis = (CELLWinApp*)GetWindowLong(hWnd, GWL_USERDATA);
    if (pThis) {
        return  pThis->onEvent(hWnd, msg, wParam, lParam);
    }
    if (WM_CREATE == msg)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        SetWindowLong(hWnd, GWL_USERDATA, (DWORD_PTR)pCreate->lpCreateParams);
    }
    return  DefWindowProc(hWnd, msg, wParam, lParam);
}
