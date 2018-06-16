#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->features->addItem("features-diff-channel");
    ui->features->addItem("features-all-channels");
    ui->features->addItem("features-signal-channels");

    ui->tabWidget->addTab(&m_table, "Frames");
    
    connect(ui->features,
            SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this,
            SLOT(onListWidgetDoubleClicked(QListWidgetItem *)));

    connect(m_table.verticalHeader(),
            SIGNAL(sectionClicked(int)),
            this,
            SLOT(onTableWidgetDoubleClicked(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onListWidgetDoubleClicked(QListWidgetItem *item) 
{   
    m_featuresParser.load("./data/" + item->text().toStdString() + ".txt");

    switch (ui->features->currentRow())
    {
        case eDIFF_CHANNEL:
            m_table.setColumnCount(diffChannelFeatures + 1);
            break;
        case eALL_CHANNELS:
            m_table.setColumnCount(allChannelsFeatures + 1);
            break;
        case eSIGNAL_CHANNELS:
            m_table.setColumnCount(signalChannelsFeatures + 1);
            break;
    }

    m_table.setRowCount(1000);

    populateTable();
}    

void MainWindow::onTableWidgetDoubleClicked(int row)
{
    QVector<double> y = QVector<double>::fromStdVector(m_featuresParser.getTrainSamples().at(row));
    QVector<double> x(y.size());

    for (auto i=0; i<x.size(); ++i)
    {
        x[i] = i;
    }

    QCustomPlot *plot = (QCustomPlot *)ui->tabWidget->widget(0);
    // create graph and assign data to it:
    plot->addGraph();
    plot->graph(0)->setData(x, y);
    // give the axes some labels:
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    plot->xAxis->setRange(0, x.back());
    plot->yAxis->setRange(-3300, 3300);
    plot->replot();
}

void MainWindow::populateTable()
{
    sample_type trainSamples = m_featuresParser.getTrainSamples();
    label_type trainLabels = m_featuresParser.getTrainLabels();

    auto currentRow = 0;
    auto currentColumn = 1;

    for (auto label : trainLabels)
        m_table.setItem(currentRow++, 0, new QTableWidgetItem(QString::fromStdString(label)));
    
    currentRow = 0;

    for (auto frame : trainSamples)
    {
        for (auto sample : frame)
        {
            m_table.setItem(currentRow, currentColumn++, new QTableWidgetItem(QString::number(sample)));
        }
        currentRow++;
        currentColumn = 1;
    }
}

