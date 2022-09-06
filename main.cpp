#include <windows.h>
#include <GL/GL.h>
// From: https://registry.khronos.org/OpenGL/api/GL/wglext.h
//#define WGL_WGLEXT_PROTOTYPES
#include "wglext.h"
 
#include <stdexcept>
#include <cstdlib>
#include <cassert>


HGLRC gOpenGLRenderingContext;
HDC gWindowHandleToDeviceContext;

PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_CREATE:
        {
            PIXELFORMATDESCRIPTOR pfd =
            {
                sizeof(PIXELFORMATDESCRIPTOR),
                1,
                PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
                PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
                32,                   // Colordepth of the framebuffer.
                0, 0, 0, 0, 0, 0,
                0,
                0,
                0,
                0, 0, 0, 0,
                24,                   // Number of bits for the depthbuffer
                8,                    // Number of bits for the stencilbuffer
                0,                    // Number of Aux buffers in the framebuffer.
                PFD_MAIN_PLANE,
                0,
                0, 0, 0
            };

            gWindowHandleToDeviceContext = GetDC(hWnd);

            int letWindowsChooseThisPixelFormat;
            letWindowsChooseThisPixelFormat = ChoosePixelFormat(gWindowHandleToDeviceContext, &pfd); 
            SetPixelFormat(gWindowHandleToDeviceContext,letWindowsChooseThisPixelFormat, &pfd);

            gOpenGLRenderingContext = wglCreateContext(gWindowHandleToDeviceContext);
            wglMakeCurrent (gWindowHandleToDeviceContext, gOpenGLRenderingContext);

            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
            assert(wglSwapIntervalEXT);
        }
        break;
    case WM_DESTROY:
        //wglMakeCurrent(gWindowHandleToDeviceContext, NULL); Unnecessary; wglDeleteContext will make the context not current
        wglDeleteContext(gOpenGLRenderingContext);
        PostQuitMessage(0);
        return DefWindowProc(hWnd, message, wParam, lParam);
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


HWND makeWindow(HINSTANCE hInstance)
{
    WNDCLASS wc      = {0}; 
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    wc.lpszClassName = "oglversionchecksample";
    wc.style = CS_OWNDC; // Important
    if( !RegisterClass(&wc) )
    {
        throw std::logic_error{"Window registration failed."};
    }
    return CreateWindowA(
        wc.lpszClassName,
        "stutter-test native win32",
        WS_OVERLAPPEDWINDOW|WS_VISIBLE,
        0,0,640,480,
        0,
        0,
        hInstance,
        0);
}


void swapBuffers()
{
	SwapBuffers(gWindowHandleToDeviceContext);
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    HWND window = makeWindow(hInstance);
    wglSwapIntervalEXT(1);

    MSG msg = {0};

    const float gStepColorIncrement = 0.01f;
    float red = 0.f;

    bool running = true;
    while(running)
    {
        while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage( &msg );
            if (msg.message == WM_QUIT)
            {
                running = false;
            }
        }

        red += gStepColorIncrement;
        red = red > 1.f ? red - 1.f : red;

        glClearColor(red, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        swapBuffers();
    }

    return EXIT_SUCCESS;
}
