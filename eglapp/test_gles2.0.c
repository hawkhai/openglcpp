#include "cube.h"
#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include "fakedriverInterface.h"

const char* VERTEX_SHADER =
"attribute vec4 vPosition;"
"void main() {"
"    gl_Position = vPosition;"
"}";
// 设置 float 类型默认精度，顶点着色器默认 highp，片元着色器需要用户声明
const char* FRAGMENT_SHADER =
"precision mediump float;"
"uniform vec4 vColor;"
"void main() {"
"    gl_FragColor = vColor;"
"}";

int loadShader(const char* strSource, const int iType) {
    int iShader = glCreateShader(iType);
    glShaderSource(iShader, 1, &strSource, NULL);
    glCompileShader(iShader);
    return iShader;
}

int loadProgram(const char* vertSource, const char* fragSource) {
    int iVertShader = loadShader(vertSource, GL_VERTEX_SHADER);
    int iFragShader = loadShader(fragSource, GL_FRAGMENT_SHADER);
    int iProgId = glCreateProgram();
    glAttachShader(iProgId, iVertShader);
    glAttachShader(iProgId, iFragShader);
    glLinkProgram(iProgId);
    glDeleteShader(iVertShader);
    glDeleteShader(iFragShader);
    return iProgId;
}

int mProgramId = -1;
int mColorId = -1; // vec4
int mPositionId = -1; // vec4
// 默认情况下，在 OpenGL 中，沿逆时针方向绘制的面为正面。
const GLfloat TRIANGLE_COORDS[] = {
     0.5f,  0.5f, -1.0f, // 右上角
     0.5f, -0.5f, -1.0f, // 右下角
    -0.5f, -0.5f, -1.0f, // 左下角
    -0.5f,  0.5f, -1.0f, // 左上角
};
const GLuint TRIANGLE_INDICES[] = {
    0, 1, 3, // 第一个三角形
    1, 2, 3, // 第二个三角形
};
const GLfloat COLOR[] = {
    1.0f, 0.0f, 0.0f, 1.0f, // 红色不透明
};

void onSurfaceCreated() {
    mProgramId = loadProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    mPositionId = glGetAttribLocation(mProgramId, "vPosition");
    mColorId = glGetUniformLocation(mProgramId, "vColor");
    // 直接把 mPositionId 和 CPU 内存联系起来。
    // OpenGL ES 2.0 和 3.0 存在差异，和 OpenGL 也不一样。非常蛋疼。
    glVertexAttribPointer(mPositionId, 3/*每组数据三个float*/, GL_FLOAT, GL_FALSE, 0, TRIANGLE_COORDS);
}

void onSurfaceDestroy() {
    glUseProgram(0);
    glDeleteProgram(mProgramId);
}

void onDrawFrame() {

    glClearColor(.5f, .5f, .5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgramId);

    glEnableVertexAttribArray(mPositionId);
    // GLint location, GLsizei count, const GLfloat* value
    glUniform4fv(mColorId, 1, COLOR);

    // https://arm-software.github.io/opengl-es-sdk-for-android/simple_triangle.html
    // https://community.arm.com/developer/tools-software/graphics/b/blog/posts/stride-argument-in-opengl-es-2-0
    // GLenum mode, GLint first, GLsizei count
    glDrawArrays(GL_TRIANGLES, 0, 4/*数组长度4*/); // 只会绘制1个三角形
    glDisableVertexAttribArray(mPositionId);
}

int main_gles20(int argc, char **argv) {

    fda_loadfakedriver(FDA_ID_ANGLE);

    const unsigned int width = 640;
    const unsigned int height = 480;

    EGLDisplay sEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(sEGLDisplay, NULL, NULL);

    /* EGL Configuration */
    EGLint aEGLAttributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLConfig	aEGLConfigs[1];
    EGLint		cEGLConfigs;
    eglChooseConfig(sEGLDisplay, aEGLAttributes, aEGLConfigs, 1, &cEGLConfigs);

    HWND hWindow = create_window(width, height);
    EGLSurface sEGLSurface = eglCreateWindowSurface(sEGLDisplay,
        aEGLConfigs[0], (EGLNativeWindowType)hWindow, NULL);

    EGLint aEGLContextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    EGLContext sEGLContext = eglCreateContext(sEGLDisplay,
        aEGLConfigs[0], EGL_NO_CONTEXT, aEGLContextAttributes);

    eglMakeCurrent(sEGLDisplay, sEGLSurface, sEGLSurface, sEGLContext);

    glViewport(0, 0, width, height);
    onSurfaceCreated();

    /* Enter event loop */
    while (TRUE) {
        MSG sMessage;
        if (PeekMessage(&sMessage, NULL, 0, 0, PM_REMOVE)) {
            if (sMessage.message == WM_QUIT) {
                break;
            }
            else {
                TranslateMessage(&sMessage);
                DispatchMessage(&sMessage);
            }
        }

        onDrawFrame();
        eglSwapBuffers(sEGLDisplay, sEGLSurface);
        Sleep(20);
    }

    /* EGL clean up */
    eglMakeCurrent(sEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    onSurfaceDestroy();
    eglDestroySurface(sEGLDisplay, sEGLSurface);
    eglDestroyContext(sEGLDisplay, sEGLContext);
    eglTerminate(sEGLDisplay);
    return 0;
}
