#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QPainter>
#include "GLWidget.h"

//#define INTERLEAVED 1

GLWidget::GLWidget()
    : program(nullptr)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    dT = 0;
    numSecs = 60;

    // Identifiers for simple, plain GL example
    gVboGeo = gVboGeoC = gEboGeo = gVaoGeo = 0;
}

GLWidget::~GLWidget()
{
    makeCurrent();

    delete program;

    glDeleteBuffers(1, &gVboGeo);
    glDeleteBuffers(1, &gVboGeoC);
    glDeleteBuffers(1, &gEboGeo);
    glDeleteVertexArrays(1, &gVaoGeo);

    doneCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(128, 128);
}

QSize GLWidget::sizeHint() const
{
    return QSize(768, 512);
}

void GLWidget::animate(float dT)
{
    this->dT = dT;  // NYI
    update();
}

void GLWidget::setDuration(float secs)
{
    numSecs = secs; // NYI
    update();
}

void GLWidget::toggleVisib()
{
    // NYI
    update();
}

void GLWidget::resetView()
{
    dT = 0; // NYI
    update();
}

void GLWidget::setClearColor(const QColor &color)
{
    bgCol = color;
    update();
}

void GLWidget::paintEvent(QPaintEvent* event)
{
    QOpenGLWidget::paintEvent(event);

    // draw fps string with Qt's 2d API on top
    QPainter painter(this);
    QString str("Press Space...");
    if (dT > 0) {
        float fps = std::clamp(dT > 1E-6f ? 1.f / dT : 0.f, 0.f, 99.99f);
        str = QString("%1 fps").arg(fps, 5, 'f', 2, '0');
    }
    painter.setPen(Qt::red);
    painter.setFont(QFont("Courier New"));
    painter.drawText(10, 15, str);
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    initShaders();

#ifdef INTERLEAVED
    const float posArr[] = { -0.5, -0.5, 1.0,  0.0, 0.0, 1.0,
                              0.5, -0.5, 1.0,  0.0, 0.0, 1.0,
                              0.5,  0.5, 1.0,  0.0, 1.0, 0.0,
                              0.0,  1.0, 1.0,  1.0, 0.0, 0.0,
                             -0.5,  0.5, 1.0,  0.0, 1.0, 0.0 };
#else
    const float posArr[] = { -0.5, -0.5,  1.0,
                              0.5, -0.5,  1.0,
                              0.5,  0.5,  1.0,
                              0.0,  1.0,  1.0,
                             -0.5,  0.5,  1.0 };
    const float colArr[] = {  0.0,  0.0,  1.0,
                              0.0,  0.0,  1.0,
                              0.0,  1.0,  0.0,
                              1.0,  0.0,  0.0,
                              0.0,  1.0,  0.0 };
#endif
    const unsigned indArr[] = { 0, 1, 2,  0, 2, 4,  4, 2, 3 };

    // TODO; generate/bind vertex array identifier - use gVaoGeo
    glGenVertexArrays(1, &gVaoGeo);
    glBindVertexArray(gVaoGeo);

    // TODO; generate/bind vertex buffer[s] - use gVboGeo [, gVboGeoC]
    // (also, copy data and enable attribute pointers)
    // BTW, you can either use interleaved or standard vertex arrays

    glGenBuffers(1, &gVboGeo);
    glBindBuffer(GL_ARRAY_BUFFER, gVboGeo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posArr), posArr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &gVboGeo);
    glBindBuffer(GL_ARRAY_BUFFER, gVboGeo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colArr), colArr, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // TODO; generate/bind buffer for indices - use gEboGeo
    // (and don't forget to disable VAO)

    glGenBuffers(1, &gEboGeo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEboGeo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indArr), indArr, GL_STATIC_DRAW);

    glBindVertexArray(0);



}

void GLWidget::paintGL()
{
    glClearColor(bgCol.redF(), bgCol.greenF(), bgCol.blueF(), bgCol.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    program->bind();
    program->setUniformValue("hasVertCols", 1.0f);

    // TODO; draw object
    glBindVertexArray(gVaoGeo);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    program->release();
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    qDebug() << width << ", " << height;
    float aspect = width / (float)height;   // NYI
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint d(event->pos() - lastPos);   // NYI
    if (event->buttons() & Qt::LeftButton) {
        ;
    }
    else if (event->buttons() & Qt::RightButton) {
        ;
    }
    lastPos = event->pos();
    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta(); // NYI
    update();
}

void GLWidget::initShaders()
{
    program = new QOpenGLShaderProgram();

    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/shaders/vert_shader.glsl"))
        close();
    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/shaders/frag_shader.glsl"))
        close();

    program->bindAttributeLocation("position", POSITIONS);
    program->bindAttributeLocation("color", COLORS);
    program->bindAttributeLocation("normal", NORMALS);
    program->bindAttributeLocation("texcoord", TEX_COORDS);

    if (!program->link())
        close();
}
