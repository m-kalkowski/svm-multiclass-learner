#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "features-parser.h"
#include "machine-learners-manager.h"
#include "svm-multiclass.h"
#include "dnn-multiclass.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFileSystemModel>
#include <QSplitter>


class QCPRange;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum EState { eDIFF_CHANNEL, eALL_CHANNELS, eSIGNAL_CHANNELS };
    enum ELabel { eRB1, eRB2, eRB3, eRB4, eRB5};

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void drawRandomPlot(int row);

private slots:
    void onTableWidgetDoubleClicked(int row);
    void onFeaturesFileDoubleClicked(const QModelIndex & index);
    void onModelsFileDoubleClicked(const QModelIndex & index);
    void onPlotTestButtonClicked();
    void onPlotSignalButtonClicked();
    void onGenerateResultsButtonClicked();
    void onRangeChanged(const QCPRange & newRange);
    void onRb1Clicked();
    void onRb2Clicked();
    void onRb3Clicked();
    void onRb4Clicked();
    void onRb5Clicked();

private:
    void populateTable();
    void plot(QVector<double> &x, QVector<double> &y);

    ELabel m_eLabel;
    EState m_state;

    Ui::MainWindow *ui;
    QTableWidget m_table;
    FeaturesParser m_featuresParser;

    MachineLearnersManager m_machineLearnersManager;

    SvmMulticlass m_svmMulticlass;
    DnnMulticlass m_dnnMulticlass;
    QFileSystemModel m_featuresModel;
    QFileSystemModel m_modelsModel;
    QSplitter m_splitter;
    std::string m_fileName;
    std::string m_modelName;
    std::string m_modelPath;
    int m_row;
    int m_xAxisSize;
};

#endif // MAINWINDOW_H
