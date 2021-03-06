#include "mainwindow.h"
#include "svmc-ova-learner.h"
#include "svmc-ovo-learner.h"
#include "svmnu-ova-learner.h"
#include "svmnu-ovo-learner.h"
#include "bayesclassifier.h"
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
    m_xAxisSize(0),
    m_currentSamples(0),
    m_currentLabels(0)
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

    ui->generatedResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

    if (std::find(m_featureNames.begin(), m_featureNames.end(), m_fileName) == m_featureNames.end()) {
        m_featureNames.push_back(m_fileName);
        ui->selectedFeatures->addItem(QFileInfo(QString::fromStdString(m_fileName)).fileName());
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
    std::string modelPath = m_modelsModel.filePath(index).toStdString();
    std::string modelName = QFileInfo(m_modelsModel.filePath(index)).fileName().toStdString();
    if (modelName == "svm_c-ova-all.dat" || modelName == "svm_c-ova-signal.dat")
        m_machineLearnersManager.registerMachineLearner(std::make_shared<SvmcOvaLearner>(), modelName);
    if (modelName == "svm_c-ovo-all.dat" || modelName == "svm_c-ovo-signal.dat")
        m_machineLearnersManager.registerMachineLearner(std::make_shared<SvmcOvoLearner>(), modelName);
    if (modelName == "svm_nu-ova-all.dat" || modelName == "svm_nu-ova-signal.dat")
        m_machineLearnersManager.registerMachineLearner(std::make_shared<SvmnuOvaLearner>(), modelName);
    if (modelName == "svm_nu-ovo-all.dat" || modelName == "svm_nu-ovo-signal.dat")
        m_machineLearnersManager.registerMachineLearner(std::make_shared<SvmnuOvoLearner>(), modelName);
    if (modelName == "BayesClassifier")
        m_machineLearnersManager.registerMachineLearner(std::make_shared<BayesClassifier>(modelPath, m_fileName), modelName);

    std::map<std::string, std::string>::iterator it = m_models.find(modelName);
    if (it == m_models.end()) {
        m_models[modelName] = modelPath;
        ui->selectedModels->addItem(QString::fromStdString(modelName));
    }
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
    for(size_t i=0; i<plot->graphCount(); ++i)
        plot->graph(i)->data().data()->clear();

    for (auto i : m_items)
        delete i;

    for (auto t : m_textLabels)
        delete t;

    m_items.clear();
    m_textLabels.clear();

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
    plot->yAxis->setRange(-3300, 3500);
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
    m_signalNumbers.clear();
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

    std::vector<double> selectedTestSamples = m_featuresParser.getTestSamples().at(row);
    QVector<double> y = QVector<double>::fromStdVector(selectedTestSamples);

    m_currentLabels.push_back(m_featuresParser.getTestLabels().at(row));
    m_currentSamples.push_back(m_featuresParser.getTestSamples().at(row));
    m_signalNumbers.push_back(row);

    plot(x, y);
}

void MainWindow::onRangeChanged(const QCPRange &newRange)
{
    ui->plot->xAxis->setRange(newRange.bounded(0, m_xAxisSize));
}

void MainWindow::onPlotSignalButtonClicked()
{
    m_signalNumbers.clear();
    sample_type signal;
    label_type label;
    label_type testLabels = m_featuresParser.getTestLabels();
    sample_type testSamples =  m_featuresParser.getTestSamples();

    size_t numOfFrames = ui->numOfFrames->value();

    for (size_t i=0; i<numOfFrames; ++i) {
        size_t randomFrameNumber = 1 + (rand() % testLabels.size() - 1);
        m_signalNumbers.push_back(randomFrameNumber);
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

    m_currentLabels = label;
    m_currentSamples = signal;

    plot(x, y);
}

void MainWindow::onGenerateResultsButtonClicked()
{
    for (auto i : m_items)
        delete i;

    for (auto t : m_textLabels)
        delete t;

    m_items.clear();
    m_textLabels.clear();

    std::vector<double> predictedLabels;

    int currentColumn = 0;

    QStringList* labels = new QStringList;

    ui->generatedResults->setRowCount(m_currentLabels.size());
    ui->generatedResults->setColumnCount(m_models.size() + 1);
    labels->push_back(QString("True Label"));

    for (size_t i=0; i<m_currentLabels.size(); ++i)
        ui->generatedResults->setItem(i, currentColumn, new QTableWidgetItem(QString::fromStdString(m_currentLabels.at(i))));

    currentColumn++;

    for (auto model : m_models) {
        std::shared_ptr<IMachineLearner> machineLearner =
        m_machineLearnersManager.getMachineLearner(model.first);
        if (model.first == "BayesClassifier")
            machineLearner->predictVector(m_signalNumbers, predictedLabels);
        else
            machineLearner->predict(m_currentSamples, predictedLabels, model.second);

        labels->push_back(QString::fromStdString(model.first));

        for (size_t i=0; i<m_currentLabels.size(); ++i)
            ui->generatedResults->setItem(i, currentColumn, new QTableWidgetItem(labelToString(predictedLabels.at(i))));

        currentColumn++;
    }

    ui->generatedResults->setHorizontalHeaderLabels(*labels);
    currentColumn = 0;

    QCustomPlot *plot = (QCustomPlot *)ui->tabWidget->widget(0);
    int l1 = 20;
    int l2 = 2;

    for (size_t i=0; i<m_currentSamples.size(); ++i) {
        m_items.push_back(new QCPItemLine(plot));
        m_items.back()->setHead(QCPLineEnding(QCPLineEnding::EndingStyle::esNone));
        m_items.back()->setTail(QCPLineEnding(QCPLineEnding::EndingStyle::esNone));
        m_items.back()->setPen(QPen(QColor(51, 180, 233), 20));
        m_items.back()->pen().setWidthF(20.0);
        m_items.back()->start->setCoords((319*i + l2)*timeStepMs, 3500);
        m_items.back()->end->setCoords((319*i + 319 - l1)*timeStepMs, 3500);

        m_items.push_back(new QCPItemLine(plot));
        m_items.back()->setPen(QPen(QColor(51, 180, 233), 2));
        m_items.back()->start->setCoords((319*i + l2)*timeStepMs, 3500);
        m_items.back()->end->setCoords((319*i + l2)*timeStepMs, -3300);

        m_items.push_back(new QCPItemLine(plot));
        m_items.back()->setPen(QPen(QColor(51, 180, 233), 2));
        m_items.back()->start->setCoords((319*i + 319 - l1)*timeStepMs, 3500);
        m_items.back()->end->setCoords((319*i + 319 - l1)*timeStepMs, -3300);

        m_textLabels.push_back(new QCPItemText(plot));
        m_textLabels.back()->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
        double label = stringToLabel(m_currentLabels.at(i));
        m_textLabels.back()->position->setCoords(((2*319*i + 319 + l2 - l1) / 2)*timeStepMs, 3495); // place position at center/top of axis rect
        m_textLabels.back()->setText(QString("%1: class %2").arg(i).arg(label));
        m_textLabels.back()->setFont(QFont(font().family(), 8, QFont::Weight::Bold)); // make font a bit larger
        m_textLabels.back()->setColor(QColor(255, 255, 255));
    }
    plot->replot();
}

double MainWindow::stringToLabel(std::string label)
{
    double result;

    if (label == "jeden_rezystor_120")
        result = 0;
    else if (label == "rezystancja_60")
        result = 1;
    else if (label == "rezystor_10k")
        result = 2;
    else if (label == "zwarcie_lini_canL_do_masy_przez_110")
        result = 3;
    else if (label == "oba_zwarte_caly_czas_przez_rezystor_110")
        result = 4;

    return result;
}

QString MainWindow::labelToString(double label)
{
    QString result;

    if (label == 0)
        result = "jeden_rezystor_120";
    else if (label == 1)
        result = "rezystancja_60";
    else if (label == 2)
        result = "rezystor_10k";
    else if (label == 3)
        result = "zwarcie_lini_canL_do_masy_przez_110";
    else if (label == 4)
        result = "oba_zwarte_caly_czas_przez_rezystor_110";

    return result;
}
