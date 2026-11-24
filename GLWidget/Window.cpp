#include <QApplication>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QColorDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QKeyEvent>
#include <QDebug>
#include "Window.h"
#include "GLWidget.h"

#define FRAME_TIME_IN_MS 16

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Visual Computing Praktikum");

    timerID = -1;

    glWidget = new GLWidget();
    glWidget->setClearColor(Qt::black);

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(glWidget, 0, 0, 1, 4);

    // Example of how to integrate some GUI stuff here
    btnChangeCol = new QPushButton("Set background color");
    btnChangeCol->setFocusPolicy(Qt::NoFocus);
    leDuration = new QLineEdit("60");
    lblDuration = new QLabel("Set animation speed: ");
    cbVisib = new QCheckBox("Toggle Visibility");
    cbVisib->setChecked(true);

    mainLayout->addWidget(btnChangeCol, 1, 0, Qt::AlignLeft);
    mainLayout->addWidget(cbVisib,      1, 1, Qt::AlignCenter);
    mainLayout->addWidget(lblDuration,  1, 2, Qt::AlignRight);
    mainLayout->addWidget(leDuration,   1, 3, Qt::AlignRight);

    setLayout(mainLayout);

    // connect events to handler methods
    connect(btnChangeCol, SIGNAL(clicked()),
            this, SLOT(colorBtnPressed()));
    connect(cbVisib, SIGNAL(toggled(bool)),
            this, SLOT(toggleVisibility(bool)));
    connect(leDuration, SIGNAL(textEdited(QString)),
            this, SLOT(durationLeChanged()));

    // Finally, some simple example menu (though file open NYI)
    QAction *quit = new QAction("&Quit");
    QAction *open = new QAction("&Open");

    QMenu *fileMenu = new QMenu("File");
    fileMenu->addAction(open);
    fileMenu->addAction(quit);

    QMenuBar *menuBar = new QMenuBar();
    menuBar->addMenu(fileMenu);
    mainLayout->setMenuBar(menuBar);

    connect(quit, &QAction::triggered, qApp, &QApplication::quit);
    connect(open, &QAction::triggered, [=](){
        qDebug() << "File open NYI!";
    });
}

void Window::keyPressEvent(QKeyEvent *)
{
    //int key = event->key();
}

void Window::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();

    switch(key) {
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_Space:
        if (timerID < 0)
            startAnim();
        else
            stopAnim();
        break;
    case Qt::Key_R:
        glWidget->resetView();
        break;
    default:
        break;
    }

    update();
}

void Window::timerEvent(QTimerEvent *)
{
    // calculation of time step dT for smooth animation
    auto currTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> diffTime = currTime - lastTime;
    float dT = static_cast<float>(diffTime.count());
    lastTime = currTime;

    // animate scene (by using time step dT)
    glWidget->animate(dT);
}

void Window::startAnim()
{
    lastTime = std::chrono::steady_clock::now();
    if (timerID < 0)
        timerID = startTimer(FRAME_TIME_IN_MS);
}

void Window::stopAnim()
{
    if (timerID >= 0) {
        killTimer(timerID);
        timerID = -1;
    }
}

void Window::durationLeChanged()
{
    float s = leDuration->text().toFloat();
    if (s >= 1) {
        glWidget->setDuration(s);
        if (timerID < 0)
            startAnim();
    }
}

void Window::toggleVisibility(bool)
{
    glWidget->toggleVisib();
}

void Window::colorBtnPressed()
{
    QColor color = QColorDialog::getColor(Qt::black, this);
    if (color.isValid()) {
        qDebug() << "Background color: " << color.name();
        glWidget->setClearColor(color);
    }
}
