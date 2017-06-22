#include "tuning_ui.h"
#include "ui_tuning_ui.h"
#include "setparameters.h"
#include "ui_setparameters.h"

bool paramWindowOpen;
int paramlist[6]; // This stores parameter value
extern bool windowclosed;

tuning_ui::tuning_ui(ros::NodeHandle _nh,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tuning_ui)
{
    uipub = _nh.advertise<std_msgs::String>("/tuning_ui/uimessages", 1);
    ui->setupUi(this);
    paramWindow = new setparameters();
    paramWindowOpen = false;
    image_transport::ImageTransport it(_nh);
    scene1 = new QGraphicsScene(this);
    ui_imagesub = it.subscribe("overheadCamImage",1,&tuning_ui::imagehandler,this);
}

tuning_ui::~tuning_ui()
{
    delete ui;
}

void tuning_ui::on_tuning_ui_destroyed()
{
      windowclosed = true;
}

void tuning_ui::ui_update()
{
    bool trainingallowed = ui->chk_vn100->isChecked() &&
                     (ui->chk_camopen->isChecked() || ui->chk_encoder->isChecked());

    ui->btn_loadtraining->setEnabled(trainingallowed);
    ui->btn_savetraining->setEnabled(trainingallowed);
    ui->btn_train->setEnabled(trainingallowed);
    ui->edit_trainwinsize->setEnabled(trainingallowed);

    bool graphallowed = ui->chk_vn100->isChecked() ||
            (ui->chk_camopen->isChecked() || ui->chk_encoder->isChecked());

    ui->btn_graph->setEnabled(graphallowed);
}
void tuning_ui::imagehandler(const sensor_msgs::ImageConstPtr& msg)
{
   try
   {
     camImg = (cv_bridge::toCvShare(msg, "bgr8")->image).clone();
     // Don't remove this clone call, else it will not finish copying the whole image and overwrite it prematurely
   }
   catch (cv_bridge::Exception& e)
   {
     ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
   }

   Mat tempImage;
   if(camImg.channels() == 3)
   {
     cvtColor(camImg,tempImage,CV_BGR2RGB);
     imgformat = QImage::Format_RGB888;
   }
   else if(camImg.channels() == 1)
   {
      tempImage = camImg.clone();
      imgformat = QImage::Format_Indexed8;
   }
   if(!(ui->ImageBox->width()==camImg.cols && ui->ImageBox->height()==camImg.rows))
      cv::resize(tempImage,tempImage,Size(ui->ImageBox->width()-2,ui->ImageBox->height()-2)); // size formatting
   Q_temp = QImage((uchar*)tempImage.data,tempImage.cols,tempImage.rows,tempImage.step,imgformat); // convert to Qimage

   QPixmap pixmap = QPixmap::fromImage(Q_temp);
   scene1->addPixmap(pixmap);
   ui->ImageBox->setScene(scene1);


}
void tuning_ui::on_chk_camopen_toggled(bool checked)
{
    ui->chk_showcam->setEnabled(checked);
    ui->overl_cam->setEnabled(checked);
    ui->chk_imagetuning->setEnabled(checked);
    if(!checked)
    {
       ui->overl_cam->setChecked(false);
       ui->chk_imagetuning->setChecked(false);
    }
    std::string msg;
    if(checked)
      msg = "Camera Publish On";
    else
      msg = "Camera Publish Off";
    uimsg.data = msg;
    uipub.publish(uimsg);
    ui_update();
}

void tuning_ui::on_chk_encoder_toggled(bool checked)
{
    ui->overl_encoder->setEnabled(checked);
    if(!checked)
        ui->overl_encoder->setChecked(false);
    ui_update();
}

void tuning_ui::on_chk_vn100_toggled(bool checked)
{
    ui->overl_imuraw->setEnabled(checked);
    ui->overl_imutrained->setEnabled(checked);
    ui->chk_iirlpf->setEnabled(checked);
    std::string msg;
    if(checked)
      msg = "VN100 Publish On";
    else
      msg = "VN100 Publsih Off";
    uimsg.data = msg;
    uipub.publish(uimsg);
    ui_update();
}

void tuning_ui::on_chk_imagetuning_toggled(bool checked)
{
    ui->btn_savecolor->setEnabled(checked);
    ui->btn_loadcolor->setEnabled(checked);
    ui->btn_adjustvals->setEnabled(checked);
    ui->sel_color->setEnabled(checked);
    ui_update();
}

void tuning_ui::on_btn_train_clicked()
{

}

void tuning_ui::on_btn_adjustvals_clicked()
{
    if(paramWindowOpen)
        return;
    paramWindowOpen = true;
    // TODO : use the old file to set the limits
    paramWindow->show();
}

void tuning_ui::on_sel_color_activated(int index)
{
    // TODO : use this for the limit setting file
}

void tuning_ui::on_btn_graph_clicked()
{

}

void tuning_ui::on_btn_savetraining_clicked()
{

}

void tuning_ui::on_btn_loadtraining_clicked()
{

}

void tuning_ui::on_edit_trainwinsize_textEdited(const QString &arg1)
{

}

void tuning_ui::on_btn_savecolor_clicked()
{

}

void tuning_ui::on_btn_loadcolor_clicked()
{

}

void tuning_ui::on_overl_cam_toggled(bool checked)
{
  std::string msg;
  if(checked)
    msg = "Camera Overlay On";
  else
    msg = "Camera Overlay Off";
  uimsg.data = msg;
  uipub.publish(uimsg);
}

void tuning_ui::on_overl_encoder_toggled(bool checked)
{
    std::string msg;
    if(checked)
      msg = "Encoder Overlay On";
    else
      msg = "Encoder Overlay Off";
    uimsg.data = msg;
    uipub.publish(uimsg);
}

void tuning_ui::on_overl_imuraw_toggled(bool checked)
{
    std::string msg;
    if(checked)
      msg = "IMU Overlay On";
    else
      msg = "IMU Overlay Off";
    uimsg.data = msg;
    uipub.publish(uimsg);
}

void tuning_ui::on_overl_imutrained_toggled(bool checked)
{
    std::string msg;
    if(checked)
      msg = "INS Overlay On";
    else
      msg = "INS Overlay Off";
    uimsg.data = msg;
    uipub.publish(uimsg);
}

void tuning_ui::on_chk_showcam_toggled(bool checked)
{
    std::string msg;
    if(checked)
      msg = "Camera Display On";
    else
      msg = "Camera Display Off";
    uimsg.data = msg;
    uipub.publish(uimsg);
}

void tuning_ui::on_chk_iirlpf_toggled(bool checked)
{
    std::string msg;
    if(checked)
      msg = "IIR LPF On";
    else
      msg = "IIR LPF Off";
    uimsg.data = msg;
    uipub.publish(uimsg);
}
