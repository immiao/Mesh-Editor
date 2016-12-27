#include "glwidget277.h"
#include <utils.h>

#include <iostream>
#include <QApplication>
#include <QProcessEnvironment>
#include <QOpenGLContext>
#include <QDebug>


GLWidget277::GLWidget277(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

GLWidget277::~GLWidget277()
{
}

inline const char *glGS(GLenum e)
{
    return reinterpret_cast<const char *>(glGetString(e));
}

void GLWidget277::debugContextVersion()
{
    QOpenGLContext *ctx = context();
    QSurfaceFormat form = format();
    QSurfaceFormat ctxform = ctx->format();
    QSurfaceFormat::OpenGLContextProfile prof = ctxform.profile();

    const char *profile =
        prof == QSurfaceFormat::CoreProfile ? "Core" :
        prof == QSurfaceFormat::CompatibilityProfile ? "Compatibility" :
        "None";
    int ctxmajor = ctxform.majorVersion();
    int ctxminor = ctxform.minorVersion();
    bool valid = ctx->isValid();
    int formmajor = form.majorVersion();
    int formminor = form.minorVersion();
    const char *vendor = glGS(GL_VENDOR);
    const char *renderer = glGS(GL_RENDERER);
    const char *version = glGS(GL_VERSION);
    const char* s_glsl = glGS(GL_SHADING_LANGUAGE_VERSION);

    printf("Widget version: %d.%d\n", ctxmajor, ctxminor);
    printf("Context valid: %s\n", valid ? "yes" : "NO");
    printf("Format version: %d.%d\n", formmajor, formminor);
    printf("Profile: %s\n", profile);
    printf("  Vendor:   %s\n", vendor);
    printf("  Renderer: %s\n", renderer);
    printf("  Version:  %s\n", version);
    printf("  GLSL:     %s\n", s_glsl);

    QString glsl = s_glsl;
    if (ctxmajor < 3 || glsl.startsWith("1.10") || glsl.startsWith("1.20")) {
        printf("ERROR: "
               "Unable to get an OpenGL 3.x context with GLSL 1.30 or newer. "
               "If your hardware should support it, update your drivers. "
               "If you have switchable graphics, make sure that you are using the discrete GPU.\n");
        QApplication::exit();
    } else if ((ctxmajor == 3 && ctxminor < 2) || glsl.startsWith("1.30") || glsl.startsWith("1.40")) {
        printf("WARNING: "
               "Enable to get an OpenGL 3.2 context with GLSL 1.50. "
               "If your hardware should support it, update your drivers. "
               "If you have switchable graphics, make sure that you are using the discrete GPU. "
               "If you cannot get 3.2 support, it is possible to port this project....");

        // Note: doing this requires at least the following actions:
        // * Change the header and base class in glwidget277.h to 3.0/3.1 instead of 3.2 Core.
        // * Change the shaders to require GLSL 1.30 or 1.40.
    }
}

void GLWidget277::printGLErrorLog()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error " << error << ": ";
        const char *e =
            error == GL_INVALID_OPERATION             ? "GL_INVALID_OPERATION" :
            error == GL_INVALID_ENUM                  ? "GL_INVALID_ENUM" :
            error == GL_INVALID_VALUE                 ? "GL_INVALID_VALUE" :
            error == GL_INVALID_INDEX                 ? "GL_INVALID_INDEX" :
            error == GL_INVALID_OPERATION             ? "GL_INVALID_OPERATION" :
            QString::number(error).toUtf8().constData();
        std::cerr << e << std::endl;
        // Throwing here allows us to use the debugger to track down the error.
#ifndef __APPLE__
        // Don't do this on OS X.
        // http://lists.apple.com/archives/mac-opengl/2012/Jul/msg00038.html
        throw;
#endif
    }
}

void GLWidget277::printLinkInfoLog(int prog)
{
    GLint linked;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    if (linked == GL_TRUE) {
        return;
    }
    std::cerr << "GLSL LINK ERROR" << std::endl;

    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glGetProgramInfoLog(prog, infoLogLen, &charsWritten, infoLog);
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete[] infoLog;
    }
    // Throwing here allows us to use the debugger to track down the error.
    throw;
}

void GLWidget277::printShaderInfoLog(int shader)
{
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE) {
        return;
    }
    std::cerr << "GLSL COMPILE ERROR" << std::endl;

    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        // error check for fail to allocate memory omitted
        glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete[] infoLog;
    }
    // Throwing here allows us to use the debugger to track down the error.
    throw;
}

/*** AUTOMATIC TESTING: DO NOT MODIFY ***/
/***/ void GLWidget277::saveImageAndQuit() {
/***/     glFlush();
/***/     QImage image = grabFramebuffer();
/***/     image.save("image.png");
/***/     QApplication::quit();
/***/ }

void GLWidget277::timerUpdate()
{

}
