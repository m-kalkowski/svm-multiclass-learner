#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <cstdlib>
#include <ctime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_row(0),
    m_xAxisSize(0)
{
    ui->setupUi(this);

    ui->features->addItem("features-diff-channel");
    ui->features->addItem("features-all-channels");
    ui->features->addItem("features-signal-channels");

    qRegisterMetaType<QVector<int> >("QVector<int>");

    ui->tabWidget->addTab(&m_table, "Frames");

    connect(ui->features,
            SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this,
            SLOT(onListWidgetDoubleClicked(QListWidgetItem *)));

    connect(m_table.verticalHeader(),
            SIGNAL(sectionClicked(int)),
            this,
            SLOT(onTableWidgetDoubleClicked(int)));

    connect(ui->plotTestButton,
            SIGNAL(clicked()),
            this,
            SLOT(onPlotTestButtonClicked()));

    connect(ui->plotSignalButton,
            SIGNAL(clicked()),
            this,
            SLOT(onPlotSignalButtonClicked()));

    connect(this,
            SIGNAL(drawRandomPlot(int)),
            this,
            SLOT(onTableWidgetDoubleClicked(int)));

    connect(ui->generateResultsButton,
            SIGNAL(clicked()),
            this,
            SLOT(onGenerateResultsButtonClicked()));

    QCustomPlot *plot = (QCustomPlot *)ui->tabWidget->widget(0);
    connect(plot->xAxis,
            SIGNAL(rangeChanged(const QCPRange &)),
            this,
            SLOT(onRangeChanged(const QCPRange &)));

    connect(ui->rb1, SIGNAL(clicked()), this, SLOT(onRb1Clicked()));
    connect(ui->rb2, SIGNAL(clicked()), this, SLOT(onRb2Clicked()));
    connect(ui->rb3, SIGNAL(clicked()), this, SLOT(onRb3Clicked()));
    connect(ui->rb4, SIGNAL(clicked()), this, SLOT(onRb4Clicked()));
    connect(ui->rb5, SIGNAL(clicked()), this, SLOT(onRb5Clicked()));

    srand(time(NULL));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onListWidgetDoubleClicked(QListWidgetItem *item)
{
    m_fileName = "./data/" + item->text().toStdString() + ".txt";

    load();
}

void MainWindow::onTableWidgetDoubleClicked(int row)
{
    QCustomPlot *plot = (QCustomPlot *)ui->tabWidget->widget(0);
    plot->clearPlottables();

    QVector<double> x;
    double step = 0;
    for (auto i=0; i<diffChannelFeatures; ++i)
    {
        x.push_back(step);
        step += 2;
    }

    switch (ui->features->currentRow())
    {
        case eDIFF_CHANNEL:
        {
            QVector<double> y = QVector<double>::fromStdVector(m_featuresParser.getAllSamples().at(row));
            plot->addGraph();
            plot->graph(0)->setData(x, y);
            break;
        }
        case eALL_CHANNELS:
        {
            QVector<double> all = QVector<double>::fromStdVector(m_featuresParser.getAllSamples().at(row));
            QVector<double> diff, high, low;
            for (auto i=0; i<all.size(); i+=3)
            {
                high.push_back(all.at(i));
                low.push_back(all.at(i + 1));
                diff.push_back(all.at(i + 2));
            }

            plot->addGraph();
            plot->graph(0)->setPen(QPen(Qt::blue));
            plot->graph(0)->setData(x, high);
            plot->addGraph();
            plot->graph(1)->setPen(QPen(Qt::red));
            plot->graph(1)->setData(x, low);
            plot->addGraph();
            plot->graph(2)->setPen(QPen(Qt::green));
            plot->graph(2)->setData(x, diff);
            break;
        }
        case eSIGNAL_CHANNELS:
        {
            QVector<double> all = QVector<double>::fromStdVector(m_featuresParser.getAllSamples().at(row));
            QVector<double> high, low;
            for (auto i=0; i<all.size(); i+=2)
            {
                high.push_back(all.at(i));
                low.push_back(all.at(i + 1));
            }
            plot->addGraph();
            plot->graph(0)->setPen(QPen(Qt::blue));
            plot->graph(0)->setData(x, high);
            plot->addGraph();
            plot->graph(1)->setPen(QPen(Qt::red));
            plot->graph(1)->setData(x, low);
            break;
        }
    }

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
    sample_type allSamples = m_featuresParser.getAllSamples();
    label_type allLabels = m_featuresParser.getAllLabels();

    auto currentRow = 0;
    auto currentColumn = 1;

    for (auto label : allLabels)
        m_table.setItem(currentRow++, 0, new QTableWidgetItem(QString::fromStdString(label)));

    currentRow = 0;

    for (auto frame : allSamples)
    {
        for (auto sample : frame)
        {
            m_table.setItem(currentRow, currentColumn++, new QTableWidgetItem(QString::number(sample)));
        }
        currentRow++;
        currentColumn = 1;
    }
}

void MainWindow::load()
{
    m_featuresParser.load(m_fileName);

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

    int rowCount = m_featuresParser.getAllSamples().size();
    m_table.setRowCount(rowCount);

    populateTable();
    m_table.update();
}

void MainWindow::onRb1Clicked()
{
    m_eLabel = eRB1;
}

void MainWindow::onRb2Clicked()
{
    m_eLabel = eRB2;
}

void MainWindow::onRb3Clicked()
{
    m_eLabel = eRB3;
}

void MainWindow::onRb4Clicked()
{
    m_eLabel = eRB4;
}

void MainWindow::onRb5Clicked()
{
    m_eLabel = eRB5;
}

void MainWindow::onPlotTestButtonClicked()
{
    std::vector<range> ranges = m_featuresParser.getRanges();
    switch (m_eLabel)
    {
        case eRB1:
        {
            m_row = (ranges.at(eRB1).first + (rand() % (int)(ranges.at(eRB1).last - ranges.at(eRB1).first + 1)));
            emit drawRandomPlot(m_row + 1000 - 1);
            break;
        }
        case eRB2:
        {
            m_row = (ranges.at(eRB2).first + (rand() % (int)(ranges.at(eRB2).last - ranges.at(eRB2).first + 1)));
            emit drawRandomPlot(m_row + 1000 - 1);
            break;
        }
        case eRB3:
        {
            m_row = (ranges.at(eRB3).first + (rand() % (int)(ranges.at(eRB3).last - ranges.at(eRB3).first + 1)));
            emit drawRandomPlot(m_row + 1000 - 1);
            break;
        }
        case eRB4:
        {
            m_row = (ranges.at(eRB4).first + (rand() % (int)(ranges.at(eRB4).last - ranges.at(eRB4).first + 1)));
            emit drawRandomPlot(m_row + 1000 - 1);
            break;
        }
        case eRB5:
        {
            m_row = (ranges.at(eRB5).first + (rand() % (int)(ranges.at(eRB5).last - ranges.at(eRB5).first + 1)));
            emit drawRandomPlot(m_row + 1000 - 1);
            break;
        }
        default:
        break;
    }
}

void MainWindow::onRangeChanged(const QCPRange &newRange)
{
    ui->plot->xAxis->setRange(newRange.bounded(0, m_xAxisSize));
}
void MainWindow::onPlotSignalButtonClicked()
{

    sample_type signal;   
    label_type label;
    label_type testLabels = m_featuresParser.getTestLabels();
    sample_type testSamples =  m_featuresParser.getTestSamples();

    int numOfFrames = ui->numOfFrames->value();

    for (int i=0; i<numOfFrames; ++i)
    {
        int randomFrameNumber = 1 + (rand() % testLabels.size() - 1);
        signal.push_back(testSamples.at(randomFrameNumber));
        label.push_back(testLabels.at(randomFrameNumber));
    }

    /*std::vector<double> predictedLabels;
    
    m_svmMulticlass.predict(signal, predictedLabels, "./models/svm-all-df");

    for (auto i=0; i<label.size(); ++i)
        std::cout << "true label: " << label.at(i) << ", predicted label: " << predictedLabels.at(i) << std::endl;
    */

    QCustomPlot *plot = (QCustomPlot *)ui->tabWidget->widget(0);
    plot->clearPlottables();
    plot->clearItems();
    
    QVector<double> x;
    double step = 0;
    for (auto i=0; i<diffChannelFeatures * label.size(); ++i)
    {
        x.push_back(step);
        step += 2;
    }

    m_xAxisSize = x.back();

    switch (ui->features->currentRow())
    {
        case eDIFF_CHANNEL:
        {
            std::vector<double> a;
            for (auto s : signal)
            {
                a.insert(a.end(), s.begin(), s.end());
            }
            QVector<double> y = QVector<double>::fromStdVector(a);
            plot->addGraph();
            plot->graph(0)->setData(x, y);
            break;
        }
        case eALL_CHANNELS:
        {
            std::vector<double> a;
            for (auto s : signal)
            {
                a.insert(a.end(), s.begin(), s.end());
            }
            QVector<double> all = QVector<double>::fromStdVector(a);
            QVector<double> diff, high, low;
            for (auto i=0; i<all.size(); i+=3)
            {
                high.push_back(all.at(i));
                low.push_back(all.at(i + 1));
                diff.push_back(all.at(i + 2));
            }

            plot->addGraph();
            plot->graph(0)->setPen(QPen(Qt::blue));
            plot->graph(0)->setData(x, high);
            plot->addGraph();
            plot->graph(1)->setPen(QPen(Qt::red));
            plot->graph(1)->setData(x, low);
            plot->addGraph();
            plot->graph(2)->setPen(QPen(Qt::green));
            plot->graph(2)->setData(x, diff);
            break;
        }
        case eSIGNAL_CHANNELS:
        {
            std::vector<double> a;
            for (auto s : signal)
            {
                a.insert(a.end(), s.begin(), s.end());
            }
            QVector<double> all = QVector<double>::fromStdVector(a);
            QVector<double> high, low;
            for (auto i=0; i<all.size(); i+=2)
            {
                high.push_back(all.at(i));
                low.push_back(all.at(i + 1));
            }
            plot->addGraph();
            plot->graph(0)->setPen(QPen(Qt::blue));
            plot->graph(0)->setData(x, high);
            plot->addGraph();
            plot->graph(1)->setPen(QPen(Qt::red));
            plot->graph(1)->setData(x, low);
            break;
        }
    }
            
    plot->axisRect()->setRangeZoom(Qt::Horizontal);
    plot->axisRect()->setRangeDrag(Qt::Horizontal);

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    


    std::vector<QCPItemRect*> section;
    for (int i=0; i<label.size(); ++i)
    {
        section.push_back(new QCPItemRect(plot));
        section.back()->topLeft->setType(QCPItemPosition::ptPlotCoords);
        section.back()->topLeft->setAxes(plot->xAxis, plot->yAxis);
        section.back()->bottomRight->setType(QCPItemPosition::ptPlotCoords);
        section.back()->bottomRight->setAxes(plot->xAxis, plot->yAxis);
        section.back()->topLeft->setCoords(i*319*2 + 7, 3300);
        section.back()->bottomRight->setCoords(319*2 * (i + 1) - 7, -3300);
        section.back()->setBrush(QBrush(QColor(0,200,0,100)));
        section.back()->setPen(Qt::NoPen);
        section.back()->setLayer("sectionBackground");
    }
    
    plot->addLayer("sectionBackground", plot->layer("grid"), QCustomPlot::limBelow);
    
    // give the axes some labels:
    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    plot->xAxis->setRange(0, x.back());
    plot->yAxis->setRange(-3300, 3300);
    plot->replot();

}

void MainWindow::onGenerateResultsButtonClicked()
{
    std::vector<double> predictedLabels;
    sample_type testSamples = m_featuresParser.getTestSamples();
    label_type testLabels = m_featuresParser.getTestLabels();

    m_svmMulticlass.predict(testSamples, predictedLabels, "./models/svm-all-df");

    for (auto i=0; i<testLabels.size(); ++i)
        std::cout << "true label: " << testLabels.at(i) << ", predicted label: " << predictedLabels.at(i) << std::endl;
}
