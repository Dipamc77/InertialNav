#include "setparameters.h"
#include "ui_setparameters.h"
#define junk -1001

extern int paramlist[6]; // This stores parameter value
extern bool paramWindowOpen;

setparameters::setparameters(QWidget *parent) : QDialog(parent), paramui(new Ui::setparameters)
{
    paramui->setupUi(this);
}

setparameters::~setparameters()
{
    delete paramui;
}

void setparameters::on_setparameters_rejected()
{
    paramWindowOpen = false;
}

void setparameters::on_slider1_sliderMoved(int value)
{
    paramlist[0] = value;
    paramui->valueset1->setText(QString::number(value));
}

void setparameters::on_slider2_sliderMoved(int value)
{
    paramlist[1] = value;
    paramui->valueset2->setText(QString::number(value));
}

void setparameters::on_slider3_sliderMoved(int value)
{
    paramlist[2] = value;
    paramui->valueset3->setText(QString::number(value));
}

void setparameters::on_slider4_sliderMoved(int value)
{
    paramlist[3] = value;
    paramui->valueset4->setText(QString::number(value));
}

void setparameters::on_slider5_sliderMoved(int value)
{
    paramlist[4] = value;
    paramui->valueset5->setText(QString::number(value));
}

void setparameters::on_slider6_sliderMoved(int value)
{
    paramlist[5] = value;
    paramui->valueset6->setText(QString::number(value));
}

void setparameters::on_valueset1_textEdited(const QString &arg1)
{
   int value = arg1.toInt();
   paramlist[0] = value;
   paramui->slider1->setValue(value);
}

void setparameters::on_valueset2_textEdited(const QString &arg1)
{
    int value = arg1.toInt();
    paramlist[1] = value;
    paramui->slider2->setValue(value);
}

void setparameters::on_valueset3_textEdited(const QString &arg1)
{
    int value = arg1.toInt();
    paramlist[2] = value;
    paramui->slider3->setValue(value);
}

void setparameters::on_valueset4_textEdited(const QString &arg1)
{
    int value = arg1.toInt();
    paramlist[3] = value;
    paramui->slider4->setValue(value);
}

void setparameters::on_valueset5_textEdited(const QString &arg1)
{
    int value = arg1.toInt();
    paramlist[4] = value;
    paramui->slider5->setValue(value);
}

void setparameters::on_valueset6_textEdited(const QString &arg1)
{
    int value = arg1.toInt();
    paramlist[5] = value;
    paramui->slider6->setValue(value);
}
