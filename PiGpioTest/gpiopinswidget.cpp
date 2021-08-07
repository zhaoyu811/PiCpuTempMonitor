#include "gpiopinswidget.h"
#include <QWidget>
#include <wiringPi.h>

GpioPinsWidget::GpioPinsWidget(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    checkBox = new QCheckBox(this);
    label = new QLabel(this);
    layout->addWidget(checkBox);
    layout->addWidget(label);
    setLayout(layout);
    timer = new QTimer(this);
    timer->setInterval(500);
    timer->setSingleShot(false);
    connect(timer, &QTimer::timeout, this, &GpioPinsWidget::TimerTimeOut);
}

void GpioPinsWidget::SetGpioName(const QString &name)
{
    label->setText(name);
    if(name.contains("0v"))
    {
        checkBox->setStyleSheet(QString("QCheckBox::indicator:unchecked {image: url(:/img/img/off.png);}QCheckBox::indicator:checked {image: url(:/img/img/off.png);}"));
    }
    else if(name=="3.3v" || name=="5v")
    {
        checkBox->setStyleSheet(QString("QCheckBox::indicator:unchecked {image: url(:/img/img/on.png);}QCheckBox::indicator:checked {image: url(:/img/img/on.png);}"));
    }
    else        //GPIO
    {
        ioComboBox = new QComboBox(this);
        ioComboBox->addItem(QString("in"));
        ioComboBox->addItem(QString("out"));
        layout->addWidget(ioComboBox);
        connect(ioComboBox, &QComboBox::currentTextChanged, this, &GpioPinsWidget::IoComboxBoxChanged);
        checkBox->setStyleSheet(QString("QCheckBox::indicator:unchecked {image: url(:/img/img/off.png);}QCheckBox::indicator:checked {image: url(:/img/img/on.png);}"));
        int alt;
        alt = getAlt(name.split(".").at(1).toInt());
        if(alt == INPUT)
        {
            ioComboBox->setCurrentText(QString("in"));
            timer->start();
        }
        else if(alt == OUTPUT)
        {
            ioComboBox->setCurrentText(QString("out"));
            connect(checkBox, &QCheckBox::clicked, this, &GpioPinsWidget::CheckBoxClicked);
        }
    }
}

void GpioPinsWidget::CheckBoxClicked(bool checked)
{
    if(checked)
    {
        digitalWrite(label->text().split(".").at(1).toInt(), 1);
    }
    else
    {
        digitalWrite(label->text().split(".").at(1).toInt(), 0);
    }
}

void GpioPinsWidget::IoComboxBoxChanged(const QString &text)
{
    if(text == "in")
    {
        pinMode(label->text().split(".").at(1).toInt(), INPUT);
        disconnect(checkBox, &QCheckBox::clicked, this, &GpioPinsWidget::CheckBoxClicked);
        timer->start();
    }
    else if(text == "out")
    {
        pinMode(label->text().split(".").at(1).toInt(), OUTPUT);
        if(timer->isActive())
            timer->stop();
        connect(checkBox, &QCheckBox::clicked, this, &GpioPinsWidget::CheckBoxClicked);
    }
}

void GpioPinsWidget::TimerTimeOut()
{
    int pinVal;
    pinVal = digitalRead(label->text().split(".").at(1).toInt());
    if(pinVal==0)
    {
        checkBox->setCheckState(Qt::Unchecked);
    }
    else
    {
        checkBox->setCheckState(Qt::Checked);
    }
}
