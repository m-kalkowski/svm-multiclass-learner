#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "features-parser.h"
#include "svm-multiclass.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum { eDIFF_CHANNEL, eALL_CHANNELS, eSIGNAL_CHANNELS };
    enum ELabel { eRB1, eRB2, eRB3, eRB4, eRB5};

    static constexpr auto diffChannelFeatures    = 319;
    static constexpr auto allChannelsFeatures    = 319 * 3;
    static constexpr auto signalChannelsFeatures = 319 * 2;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void drawRandomPlot(int row);

private slots:
    void onListWidgetDoubleClicked(QListWidgetItem *item);
    void onTableWidgetDoubleClicked(int row);
    void onPlotTestButtonClicked();
    void onGenerateResultsButtonClicked();
    void onRb1Clicked();
    void onRb2Clicked();
    void onRb3Clicked();
    void onRb4Clicked();
    void onRb5Clicked();

private:
    void populateTable();
    void load();

    ELabel m_eLabel;

    Ui::MainWindow *ui;
    QTableWidget m_table;
    FeaturesParser m_featuresParser;
    SvmMulticlass m_svmMulticlass;
    std::string m_fileName;
    int m_row;
};

#endif // MAINWINDOW_H
