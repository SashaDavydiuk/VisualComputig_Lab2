#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

class QOpenGLShaderProgram;
class QOpenGLTexture;

class GLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    enum { POSITIONS, COLORS, NORMALS, TEX_COORDS, INDICES, N };

    GLWidget();
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void animate(float dT);
    void setDuration(float secs);
    void toggleVisib();
    void resetView();
    void setClearColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void initShaders();

    QOpenGLShaderProgram *program;

    QPoint lastPos;
    float dT, numSecs;
    QColor bgCol;

    unsigned int gVboGeo, gVboGeoC, gEboGeo, gVaoGeo;
};

#endif
