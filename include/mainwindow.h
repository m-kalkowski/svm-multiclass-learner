#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "features-parser.h"

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
    
    static constexpr auto diffChannelFeatures    = 319;
    static constexpr auto allChannelsFeatures    = 319 * 3;
    static constexpr auto signalChannelsFeatures = 319 * 2;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onListWidgetDoubleClicked(QListWidgetItem *item);
    void onTableWidgetDoubleClicked(int row);

private:
    void populateTable();
    
    Ui::MainWindow *ui;
    QTableWidget m_table;
    FeaturesParser m_featuresParser;
};

#endif // MAINWINDOW_H
