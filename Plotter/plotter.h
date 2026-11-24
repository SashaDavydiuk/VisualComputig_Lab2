#ifndef PLOTTER_H
#define PLOTTER_H
#include <QWidget>

class QPushButton;
class QLabel;
class QLineEdit;
class Canvas;
class QComboBox;

class Plotter : public QWidget
{
	Q_OBJECT

public:
	Plotter(QWidget *parent = 0);
	~Plotter();

private slots:
	void drawBtnPressed();

private:
	Canvas *viewport;

    QPushButton *btnDrawFunc;
    QComboBox *cmbFunction;
	QLabel *lblMinX;
	QLabel *lblMaxX;
	QLabel *lblMinY;
	QLabel *lblMaxY;
    QLabel *lblMouse;
	QLineEdit *ledMinX;
	QLineEdit *ledMaxX;
	QLineEdit *ledMinY;
	QLineEdit *ledMaxY;

};

#endif // PLOTTER_H
