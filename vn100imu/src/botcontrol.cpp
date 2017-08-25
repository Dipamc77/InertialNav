#include "botcontrol.h"
#include "ui_botcontrol.h"

botcontrol::botcontrol(ros::NodeHandle _nh,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::botcontrol)
{
    uipub = _nh.advertise<std_msgs::String>("/botcontrol/uimessages", 1);
    ui->setupUi(this);
}

botcontrol::~botcontrol()
{
    delete ui;
}

void botcontrol::on_rbtn_dist_toggled(bool checked)
{
    ui->edit_dist->setEnabled(checked);
}

void botcontrol::on_rbtn_time_toggled(bool checked)
{
    ui->edit_time->setEnabled(checked);
}

void botcontrol::on_edit_dist_textEdited(const QString &arg1)
{
  // TODO: wrong data entry can be fixed here

}
void botcontrol::on_edit_time_textEdited(const QString &arg1)
{

}
void botcontrol::keyPressEvent(QKeyEvent *event)
{
   int keyascii = event->key();
   QString str =  ui->edit_pwm->text();
   int pval = str.toInt();
   if(keyascii == 'Q')
   {
       pval -= 10;
       if(pval <0)
           pval = 0;
       ui->edit_pwm->setText(QString::number(pval));
   }
   else if(keyascii == 'E')
   {
       pval +=10;
       if(pval >255)
           pval = 255;
       ui->edit_pwm->setText(QString::number(pval));
   }
   else if(keyascii == 'F')
       on_btn_pwm_clicked();
   else if(keyascii == 'W')
       on_btn_forward_pressed();
   else if(keyascii == 'S')
       on_btn_back_pressed();
   else if(keyascii == 'D')
       on_btn_right_pressed();
   else if(keyascii == 'A')
       on_btn_left_pressed();
   else
       on_btn_stop_clicked();
}
void botcontrol::on_edit_pwm_returnPressed()
{
  std::string msg;
  QString qs;
      qs = "PWM " + ui->edit_pwm->text();
      msg = qs.toStdString();
    uimsg.data = msg;
    uipub.publish(uimsg);
}

void botcontrol::on_btn_pwm_clicked()
{
  std::string msg;
  QString qs;
      qs = "PWM " + ui->edit_pwm->text();
      msg = qs.toStdString();
    uimsg.data = msg;
    uipub.publish(uimsg);
}

void botcontrol::on_btn_go_clicked()
{
  std::string msg;
  QString qs;
  bool distchecked = ui->edit_dist->isEnabled(); // TODO : only works because dist and time are the only 2 radiobuttons
  if(distchecked)
  {
    qs = "Go "+ui->edit_dist->text() + " cm";
  }
  else
  {
    qs = "Go "+ui->edit_time->text() + " sec";
  }
  msg = qs.toStdString();
  uimsg.data = msg;
  uipub.publish(uimsg);
}

void botcontrol::on_btn_stop_clicked()
{
    std::string msg;
    msg = "Stop";
    uimsg.data = msg;
    uipub.publish(uimsg);
}

void botcontrol::on_btn_forward_pressed()
{
  std::string msg;
    msg = "Forward";
  uimsg.data = msg;
  uipub.publish(uimsg);
}

void botcontrol::on_btn_left_pressed()
{
  std::string msg;
    msg = "Left";
  uimsg.data = msg;
  uipub.publish(uimsg);
}

void botcontrol::on_btn_right_pressed()
{
  std::string msg;
    msg = "Right";
  uimsg.data = msg;
  uipub.publish(uimsg);
}

void botcontrol::on_btn_back_pressed()
{
  std::string msg;
    msg = "Backward";
  uimsg.data = msg;
  uipub.publish(uimsg);
}

void botcontrol::on_btn_forward_released()
{
}

void botcontrol::on_btn_left_released()
{

}

void botcontrol::on_btn_right_released()
{

}

void botcontrol::on_btn_back_released()
{

}
void botcontrol::on_btn_tare_clicked()
{
  std::string msg;
    msg = "TARE";
  uimsg.data = msg;
  uipub.publish(uimsg);
}
