#pragma once

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

#include <EGL/egl.h>
#include <gles2/gl2.h>

class CELLWinApp
{
public:
    CELLWinApp(HINSTANCE inst) {
        m_hInstance = inst;
        m_hWnd = nullptr;

        m_config = nullptr;
        m_display = EGL_NO_DISPLAY;
        m_context = EGL_NO_CONTEXT;
        m_surface = EGL_NO_SURFACE;

        m_width = m_height = 0;

        WNDCLASSEX  winClass;
        winClass.lpszClassName = _T("CELLWinApp");
        winClass.cbSize = sizeof(winClass);
        winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        winClass.lpfnWndProc = lpfnWndProc;
        winClass.hInstance = m_hInstance;
        winClass.hIcon = 0;
        winClass.hIconSm = 0;
        winClass.hCursor = LoadCursor(m_hInstance, IDC_ARROW);
        winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        winClass.lpszMenuName = NULL;
        winClass.cbClsExtra = 0;
        winClass.cbWndExtra = 0;
        RegisterClassEx(&winClass);
    }
    virtual ~CELLWinApp() {
        UnregisterClass(_T("CELLWinApp"), m_hInstance);
    }

    int main(int width, int height);

private:
    bool initGLES();
    void destroyGLES();

protected:

    static LRESULT CALLBACK lpfnWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    virtual void render();
    virtual LRESULT onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    // for gles 2.0
    EGLConfig  m_config;
    EGLSurface m_surface;
    EGLContext m_context;
    EGLDisplay m_display;

    int m_width;
    int m_height;
};
