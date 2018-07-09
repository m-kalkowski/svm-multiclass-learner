#include "mainwindow.h"
#include "svmc-ova-learner.h"
#include "svmc-ovo-learner.h"
#include "svmnu-ova-learner.h"
#include "svmnu-ovo-learner.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

#include <QFont>
#include <iostream>
#include <cstdlib>
#include <ctime>


namespace {
    constexpr auto timeStepMs = 2;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_fileName(""),
    m_row(0),
    m_xAxisSize(0)
{
    ui->setupUi(this);

    m_featuresModel.setRootPath(QDir::currentPath() + "/data");
    ui->features->setModel(&m_featuresModel);
    ui->features->setRootIndex(m_featuresModel.index(QDir::currentPath() + "/data"));

    m_modelsModel.setRootPath(QDir::currentPath() + "/models");
    ui->models->setModel(&m_modelsModel);
    ui->models->setRootIndex(m_modelsModel.index(QDir::currentPath() + "/models"));

    qRegisterMetaType<QVector<int> >("QVector<int>");

    ui->tabWidget->addTab(&m_table, "Frames");

    connect(ui->features,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this,
            SLOT(onFeaturesFileDoubleClicked(const QModelIndex &)));

    connect(ui->models,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this,
            SLOT(onModelsFileDoubleClicked(const QModelIndex &)));

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
    plot->setBackground(QColor(35, 38, 41));
    plot->xAxis->setTickLabelColor(QColor(255, 255, 255));
    plot->xAxis->setBasePen(QColor(255, 255, 255));
    plot->xAxis->setTickPen(QColor(255, 255, 255));
    plot->xAxis->setSubTickPen(QColor(255, 255, 255));
    plot->xAxis->setTickLabelFont(QFont("Helvetica [Cronyx]", 7));
    plot->yAxis->setTickLabelColor(QColor(255, 255, 255));
    plot->yAxis->setBasePen(QColor(255, 255, 255));
    plot->yAxis->setTickPen(QColor(255, 255, 255));
    plot->yAxis->setSubTickPen(QColor(255, 255, 255));
    plot->yAxis->setTickLabelFont(QFont("Helvetica [Cronyx]", 7));
    plot->xAxis->setLabelColor(QColor(255, 255, 255));
    plot->xAxis->setLabelFont(QFont("Helvetica [Cronyx]", 7));
    plot->yAxis->setLabelColor(QColor(255, 255, 255));
    plot->yAxis->setLabelFont(QFont("Helvetica [Cronyx]", 7));

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

void MainWindow::onFeaturesFileDoubleClicked(const QModelIndex & index)
{
    m_fileName = m_featuresModel.filePath(index).toStdString();
    std::cout << m_fileName << std::endl;
    if (!m_featuresParser.load(m_fileName)) {
        std::cout << "Unable to open file!" << std::endl;
        return;
    }

    int channels = m_featuresParser.channels();
    m_table.setColumnCount(FeaturesParser::numOfFeatures*channels + 1);

    int rowCount = m_featuresParser.getAllSamples().size();
    m_table.setRowCount(rowCount);

    populateTable();
    m_table.update();
}

void MainWindow::onModelsFileDoubleClicked(const QModelIndex & index)
{
    m_modelPath = m_modelsModel.filePath(index).toStdString();
    m_modelName = m_modelsModel.fileName(index).toStdString();
    if (m_modelName == "svm_c-ova-all.dat" || m_modelName == "svm_c-ova-signal.dat")
        m_machineLearnersManager.registerMachineLearner(std::make_shared<SvmcOvaLearner>(), m_modelName);
    if (m_modelName == "svm_c-ovo-all.dat" || m_modelName == "svm_c-ovo-signal.dat")
        m_machineLearnersManager.registerMachineLearner(std::make_shared<SvmcOvoLearner>(), m_modelName);
    if (m_modelName == "svm_nu-ova-all.dat" || m_modelName == "svm_nu-ova-signal.dat")
        m_machineLearnersManager.registerMachineLearner(std::make_shared<SvmnuOvaLearner>(), m_modelName);
    if (m_modelName == "svm_nu-ovo-all.dat" || m_modelName == "svm_nu-ovo-signal.dat")
        m_machineLearnersManager.registerMachineLearner(std::make_shared<SvmnuOvoLearner>(), m_modelName);
}

void MainWindow::onTableWidgetDoubleClicked(int row)
{
    QVector<double> x;
    double time = 0;
    for (auto i=0; i<FeaturesParser::numOfFeatures; ++i) {
        x.push_back(time);
        time += timeStepMs;
    }

    QVector<double> y = QVector<double>::fromStdVector(m_featuresParser.getAllSamples().at(row));

    plot(x, y);
}

void MainWindow::plot(QVector<double> &x, QVector<double> &y)
{
    QCustomPlot *plot = (QCustomPlot *)ui->tabWidget->widget(0);
    plot->clearPlottables();

    plot->axisRect()->setRangeZoom(Qt::Horizontal);
    plot->axisRect()->setRangeDrag(Qt::Horizontal);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    QColor colors[] = {Qt::blue, Qt::red, Qt::green};
    m_xAxisSize = x.back();

    QVector<double> frameOneChannel;

    int channels = m_featuresParser.channels();
    for (auto i=0; i<channels; ++i) {
        for (auto j=0; j<y.size(); j+=channels)
            frameOneChannel.push_back(y.at(j + i));

        plot->addGraph();
        plot->graph(i)->setPen(QPen(colors[i]));
        plot->graph(i)->setData(x, frameOneChannel);

        frameOneChannel.clear();
    }

    // give the axes some labels:
    plot->xAxis->setLabel("t[ms]");
    plot->yAxis->setLabel("V[mV]");
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

    for (auto frame : allSamples) {
        for (auto sample : frame)
            m_table.setItem(currentRow, currentColumn++, new QTableWidgetItem(QString::number(sample)));

        currentRow++;
        currentColumn = 1;
    }
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
    int row = 0;
    std::vector<range> ranges = m_featuresParser.getRanges();
    switch (m_eLabel) {
        case eRB1:
            row = (ranges.at(eRB1).first + (rand() % (int)(ranges.at(eRB1).last - ranges.at(eRB1).first + 1)));
            break;
        case eRB2:
            row = (ranges.at(eRB2).first + (rand() % (int)(ranges.at(eRB2).last - ranges.at(eRB2).first + 1)));
            break;
        case eRB3:
            row = (ranges.at(eRB3).first + (rand() % (int)(ranges.at(eRB3).last - ranges.at(eRB3).first + 1)));
            break;
        case eRB4:
            row = (ranges.at(eRB4).first + (rand() % (int)(ranges.at(eRB4).last - ranges.at(eRB4).first + 1)));
            break;
        case eRB5:
            row = (ranges.at(eRB5).first + (rand() % (int)(ranges.at(eRB5).last - ranges.at(eRB5).first + 1)));
            break;
        default:
        break;
    }

    QVector<double> x;
    double time = 0;
    for (auto i=0; i<FeaturesParser::numOfFeatures; ++i) {
        x.push_back(time);
        time += timeStepMs;
    }

    QVector<double> y = QVector<double>::fromStdVector(m_featuresParser.getTestSamples().at(row));

    plot(x, y);
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

    size_t numOfFrames = ui->numOfFrames->value();

    for (size_t i=0; i<numOfFrames; ++i) {
        size_t randomFrameNumber = 1 + (rand() % testLabels.size() - 1);
        signal.push_back(testSamples.at(randomFrameNumber));
        label.push_back(testLabels.at(randomFrameNumber));
    }

    QVector<double> x;
    double time = 0;
    for (size_t i=0; i<FeaturesParser::numOfFeatures * label.size(); ++i) {
        x.push_back(time);
        time += timeStepMs;
    }

    std::vector<double> a;
    for (auto s : signal)
        a.insert(a.end(), s.begin(), s.end());

    QVector<double> y = QVector<double>::fromStdVector(a);

    std::cout << y.size() << std::endl;
    plot(x, y);
}

void MainWindow::onGenerateResultsButtonClicked()
{
    std::vector<double> predictedLabels;
    sample_type testSamples = m_featuresParser.getTestSamples();
    label_type testLabels = m_featuresParser.getTestLabels();

    if (testSamples.size() == 0 || testLabels.size() == 0 || m_fileName == "") {
        std::cout << "No features file provided or features file empty." << std::endl;
        return;
    }

    std::shared_ptr<IMachineLearner> machineLearner =
        m_machineLearnersManager.getMachineLearner(m_modelName);

    if (machineLearner == nullptr) {
        std::cout << "No model selected." << std::endl;
        return;
    }

    machineLearner->predict(testSamples, predictedLabels, m_modelPath);

    for (size_t i=0; i<testLabels.size(); ++i)
        std::cout << "true label: " << testLabels.at(i)
                  << ", predicted label: " << predictedLabels.at(i)
                  << std::endl;
}
