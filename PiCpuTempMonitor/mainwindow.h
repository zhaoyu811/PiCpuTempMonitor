#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QTimer>
#include <QVector>
#include <QDateTimeAxis>
#include <QValueAxis>

using namespace QT_CHARTS_NAMESPACE;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void TimerTimeOut();

private:
    QChart * chart;
    QChartView * chartView;
    QLineSeries * series;
    QDateTimeAxis * axisX;
    QValueAxis * axisY;
    QTimer * timer;
    QVector<QPointF> buffer;
};

#endif // MAINWINDOW_H
