#ifndef GPIOPINSWIDGET_H
#define GPIOPINSWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTimer>

class GpioPinsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GpioPinsWidget(QWidget *parent = nullptr);
    void SetGpioName(const QString &name);
private slots:
    void CheckBoxClicked(bool checked);
    void IoComboxBoxChanged(const QString &text);
    void TimerTimeOut();
private:
    QCheckBox * checkBox;
    QLabel * label;
    QVBoxLayout * layout;
    QComboBox * ioComboBox;
    QTimer * timer;
};

#endif // GPIOPINSWIDGET_H
