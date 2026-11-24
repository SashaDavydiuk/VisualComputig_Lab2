#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>

class GLWidget;
class QPushButton;
class QLineEdit;
class QLabel;
class QCheckBox;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);

private slots:
    void colorBtnPressed();
    void toggleVisibility(bool);
    void durationLeChanged();

private:
    void startAnim();
    void stopAnim();

    int timerID;
    std::chrono::time_point<std::chrono::steady_clock> lastTime;

    QPushButton *btnChangeCol;
    QLineEdit *leDuration;
    QLabel *lblDuration;
    QCheckBox *cbVisib;

    GLWidget *glWidget;
};

#endif
