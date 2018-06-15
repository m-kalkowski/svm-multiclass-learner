#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	// add two new graphs and set their look:
	/*ui->tabWidget->plot->addGraph();
	ui->tabWidget->plot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
	ui->tabWidget->plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
	ui->tabWidget->plot->addGraph();
	ui->tabWidget->plot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
	// generate some points of data (y0 for first, y1 for second graph):
	QVector<double> x(251), y0(251), y1(251);
	for (int i=0; i<251; ++i)
	{
	  x[i] = i;
	  y0[i] = qExp(-i/150.0)*qCos(i/10.0); // exponentially decaying cosine
	  y1[i] = qExp(-i/150.0);              // exponential envelope
	}
	// configure right and top axis to show ticks but no labels:
	// (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
	ui->tabWidget->plot->xAxis2->setVisible(true);
	ui->tabWidget->plot->xAxis2->setTickLabels(false);
	ui->tabWidget->plot->yAxis2->setVisible(true);
	ui->tabWidget->plot->yAxis2->setTickLabels(false);
	// make left and bottom axes always transfer their ranges to right and top axes:
	connect(ui->tabWidget->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->tabWidget->plot->xAxis2, SLOT(setRange(QCPRange)));
	connect(ui->tabWidget->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->tabWidget->plot->yAxis2, SLOT(setRange(QCPRange)));
	// pass data points to graphs:
	ui->tabWidget->plot->graph(0)->setData(x, y0);
	ui->tabWidget->plot->graph(1)->setData(x, y1);
	// let the ranges scale themselves so graph 0 fits perfectly in the visible area:
	ui->tabWidget->plot->graph(0)->rescaleAxes();
	// same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
	ui->tabWidget->plot->graph(1)->rescaleAxes(true);
	// Note: we could have also just called tabWidget->plot->rescaleAxes(); instead
	// Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
	ui->tabWidget->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);*/
}

MainWindow::~MainWindow()
{
    delete ui;
}
