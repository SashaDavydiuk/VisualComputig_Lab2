#ifndef CANVAS_H
#define CANVAS_H
#include <QFrame>

class Canvas : public QFrame
{
	Q_OBJECT

public:
	Canvas(QWidget *parent = 0);
	~Canvas();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void setWorld(const QPointF &min, const QPointF &max);
    void setFunction(int f);

protected:
	void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

	QPoint  WC_to_DC(const QPointF &wc);
	QPointF DC_to_WC(const QPoint  &dc);

private:
	QPointF wMin, wMax;
    int function;
signals:
void worldPosClicked(const QString &text);
};

#endif // CANVAS_H
