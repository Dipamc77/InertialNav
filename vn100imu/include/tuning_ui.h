#ifndef TUNING_UI_H
#define TUNING_UI_H

#include <QMainWindow>
#include "setparameters.h"
#include <ros/ros.h>
#include "std_msgs/String.h"
#include <QGraphicsScene>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

namespace Ui {
class tuning_ui;
}

class tuning_ui : public QMainWindow
{
    Q_OBJECT

public:
    explicit tuning_ui(ros::NodeHandle _nh,QWidget *parent = 0);
    ~tuning_ui();
    void ui_update();
    void imagehandler(const sensor_msgs::ImageConstPtr& msg);

private slots:
    void on_chk_camopen_toggled(bool checked);

    void on_chk_encoder_toggled(bool checked);

    void on_chk_vn100_toggled(bool checked);

    void on_chk_imagetuning_toggled(bool checked);

    void on_btn_train_clicked();

    void on_btn_adjustvals_clicked();

    void on_sel_color_activated(int index);

    void on_btn_graph_clicked();

    void on_btn_savetraining_clicked();

    void on_btn_loadtraining_clicked();

    void on_edit_trainwinsize_textEdited(const QString &arg1);

    void on_btn_savecolor_clicked();

    void on_btn_loadcolor_clicked();

    void on_overl_cam_toggled(bool checked);

    void on_overl_encoder_toggled(bool checked);

    void on_overl_imuraw_toggled(bool checked);

    void on_overl_imutrained_toggled(bool checked);

    void on_chk_showcam_toggled(bool checked);

    void on_chk_iirlpf_toggled(bool checked);

    void on_tuning_ui_destroyed();

public:
  QImage Q_temp;
  QGraphicsScene *scene1;
  QImage::Format imgformat;
  Mat camImg;

private:
    Ui::tuning_ui *ui;
    setparameters *paramWindow;

    //ros::Subscriber uisub;
    ros::Publisher  uipub;
    std_msgs::String uimsg;
    image_transport::Subscriber ui_imagesub;
};

#endif // TUNING_UI_H
