#ifndef GLWIDGET277_H
#define GLWIDGET277_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <QTimer>


class GLWidget277
    : public QOpenGLWidget,
      public QOpenGLFunctions_3_2_Core
{
    Q_OBJECT


protected:
    /*** AUTOMATIC TESTING: DO NOT MODIFY ***/
    /*** If true, save a test image and exit */
    /***/ bool autotesting;

public:
    GLWidget277(QWidget *parent);
    ~GLWidget277();

    void debugContextVersion();
    void printGLErrorLog();
    void printLinkInfoLog(int prog);
    void printShaderInfoLog(int shader);

private slots:
    /*** AUTOMATIC TESTING: DO NOT MODIFY ***/
    /***/ void saveImageAndQuit();
    virtual void timerUpdate();


};


#endif // GLWIDGET277_H
