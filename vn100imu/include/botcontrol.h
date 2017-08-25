#ifndef BOTCONTROL_H
#define BOTCONTROL_H

#include <QMainWindow>
#include "setparameters.h"
#include <ros/ros.h>
#include "std_msgs/String.h"
#include <QKeyEvent>

namespace Ui {
class botcontrol;
}

class botcontrol : public QMainWindow
{
    Q_OBJECT

public:
    explicit botcontrol(ros::NodeHandle _nh,QWidget *parent = 0);
    ~botcontrol();

private slots:

    void on_rbtn_dist_toggled(bool checked);

    void on_rbtn_time_toggled(bool checked);

    void on_edit_dist_textEdited(const QString &arg1);

    void on_edit_time_textEdited(const QString &arg1);

    void on_btn_go_clicked();

    void on_btn_stop_clicked();

    void on_btn_forward_pressed();

    void on_btn_left_pressed();

    void on_btn_right_pressed();

    void on_btn_back_pressed();

    void on_btn_forward_released();

    void on_btn_left_released();

    void on_btn_right_released();

    void on_btn_back_released();

    void on_btn_pwm_clicked();

    void on_edit_pwm_returnPressed();

    void keyPressEvent(QKeyEvent *event);

   void on_btn_tare_clicked();

private:
    Ui::botcontrol *ui;

    ros::Publisher uipub;
    std_msgs::String uimsg;
};

#endif // BOTCONTROL_H
