#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>
#include <cmath>
#include <limits>
#include "plotter.h"
#include "canvas.h"

#define MAX_WIDTH 180

/** c'tor */
Plotter::Plotter(QWidget *parent)
	: QWidget(parent)
{
	// instantiate Canvas and button
	viewport = new Canvas();
    btnDrawFunc = new QPushButton("&Draw Function");
    cmbFunction = new QComboBox();


    // instantiate Canvas and button
    viewport = new Canvas();
    btnDrawFunc = new QPushButton("&Draw Function");
    cmbFunction = new QComboBox();
    cmbFunction->addItem("Sinus");
    cmbFunction->addItem("Kreis");
    cmbFunction->addItem("Herz");
    lblMouse = new QLabel("Klick");

	// instantiate elements for setting world window
    ledMinX = new QLineEdit("-20");
    ledMinX->setMaximumWidth(MAX_WIDTH);
	lblMinX = new QLabel("World MinX");
	lblMinX->setBuddy(ledMinX);

    ledMinY = new QLineEdit("-20");
    ledMinY->setMaximumWidth(MAX_WIDTH);
	lblMinY = new QLabel("World MinY");
	lblMinY->setBuddy(ledMinY);

    ledMaxX = new QLineEdit("20");
    ledMaxX->setMaximumWidth(MAX_WIDTH);
	lblMaxX = new QLabel("World MaxX");
	lblMaxX->setBuddy(ledMaxX);

    ledMaxY = new QLineEdit("20");
    ledMaxY->setMaximumWidth(MAX_WIDTH);
	lblMaxY = new QLabel("World MaxY");
	lblMaxY->setBuddy(ledMaxY);

	// create layout and add gui elements to it
	QGridLayout *mainLayout = new QGridLayout;

	mainLayout->addWidget(viewport, 0, 0, 1, 3);
    mainLayout->addWidget(btnDrawFunc, 1, 0);
    mainLayout->addWidget(cmbFunction, 2, 0);
    mainLayout->addWidget(lblMouse, 3, 0);
    mainLayout->addWidget(lblMinX, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(ledMinX, 1, 2);
    mainLayout->addWidget(lblMinY, 2, 1, Qt::AlignRight);
    mainLayout->addWidget(ledMinY, 2, 2);
    mainLayout->addWidget(lblMaxX, 3, 1, Qt::AlignRight);
    mainLayout->addWidget(ledMaxX, 3, 2);
    mainLayout->addWidget(lblMaxY, 4, 1, Qt::AlignRight);
    mainLayout->addWidget(ledMaxY, 4, 2);

	// add layout to this widget instance
	setLayout(mainLayout);

    // update world dimensions as initialized in textfields
    drawBtnPressed();
	
	// connect button click event to draw handler
    connect(btnDrawFunc, SIGNAL(clicked()), this, SLOT(drawBtnPressed()));

     connect(viewport, SIGNAL(worldPosClicked(QString)), lblMouse, SLOT(setText(QString)));
}

/** d'tor */
Plotter::~Plotter()
{
}

/** method for handling button clicked event */
void Plotter::drawBtnPressed()
{
	QPointF min, max;

    bool okMinX, okMinY, okMaxX, okMaxY;
    min.setX(ledMinX->text().toFloat(&okMinX));
    min.setY(ledMinY->text().toFloat(&okMinY));
    max.setX(ledMaxX->text().toFloat(&okMaxX));
    max.setY(ledMaxY->text().toFloat(&okMaxY));

    // The coordinate transforms require finite, nonzero world extents.
    if (!okMinX || !okMinY || !okMaxX || !okMaxY ||
        !std::isfinite(min.x()) || !std::isfinite(min.y()) ||
        !std::isfinite(max.x()) || !std::isfinite(max.y()) ||
        min.x() >= max.x() || min.y() >= max.y() ||
        max.x() - min.x() > std::numeric_limits<float>::max() ||
        max.y() - min.y() > std::numeric_limits<float>::max()) {
        QMessageBox::warning(this, tr("Invalid world window"),
                             tr("Enter finite numbers with MinX < MaxX and MinY < MaxY."));
        return;
    }

    viewport->setFunction(cmbFunction->currentIndex());
	// update world window in canvas instance
	viewport->setWorld(min, max);

	// force redraw
	update();
}
