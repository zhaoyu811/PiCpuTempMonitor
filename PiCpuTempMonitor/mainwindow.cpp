#include "mainwindow.h"
#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <QDateTime>

const int sampleCount = 100;
const int timeOut = 500;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    chart = new QChart;
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing, true);
    series = new QLineSeries(chart);

    axisX = new QDateTimeAxis(chart);
    axisX->setTitleText("Time");
    axisX->setFormat("hh:mm:ss");
    axisY = new QValueAxis(chart);
    axisY->setTitleText("Cpu temperature");
    axisY->setRange(0, 100);
    chart->addSeries(series);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    chart->legend()->hide();
    chart->setTitle("Cpu Temperature Monitor");
    chartView->setMinimumSize(QSize(800, 600));
    setCentralWidget(chartView);

    timer = new QTimer(this);
    timer->setInterval(timeOut);
    timer->setSingleShot(false);
    timer->start();
    connect(timer, &QTimer::timeout, this, &MainWindow::TimerTimeOut);
}

MainWindow::~MainWindow()
{

}

void MainWindow::TimerTimeOut()
{
    //1. initial data
    if(buffer.isEmpty())
    {
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        buffer.reserve(sampleCount);
        for(int i=0; i<sampleCount; i++)
        {
            buffer.append(QPointF(currentTime-((100-i)*timeOut), 0));
        }
    }
    //2. read cpu temperature
    int fd;
    ssize_t ret;
    char buf[20];
    fd = open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
    if(fd == 0)
    {
        qDebug() << "open failed";
        return;
    }
    memset(buf, 0, 20);
    ret = read(fd, buf, 5);
    if(ret == 0)
    {
        qDebug() << "read failed";
        ::close(fd);
        return;
    }
    else
    {
        //qDebug()<< ret;
        ::close(fd);
    }

    double temp;
    temp = atoi(buf)/1000.0;
    //3. update data
    buffer.remove(0);
    buffer.append(QPointF(QDateTime::currentDateTime().toMSecsSinceEpoch(), temp));
    series->replace(buffer);
    //4. update axis
    axisX->setRange(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(buffer[0].x())), QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(buffer[sampleCount-1].x())));
    if(temp>75)
        axisY->setRange(50, 150);
    else if(temp<25)
        axisY->setRange(-50, 50);
}
