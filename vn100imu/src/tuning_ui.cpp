#include "tuning_ui.h"
#include "ui_tuning_ui.h"
#include "setparameters.h"
#include "ui_setparameters.h"

bool paramWindowOpen;
int paramlist[6]; // This stores parameter value

tuning_ui::tuning_ui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tuning_ui)
{
    ui->setupUi(this);
    paramWindow = new setparameters();
    paramWindowOpen = false;
}

tuning_ui::~tuning_ui()
{
    delete ui;
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

}

void tuning_ui::on_overl_encoder_toggled(bool checked)
{

}

void tuning_ui::on_overl_imuraw_toggled(bool checked)
{

}

void tuning_ui::on_overl_imutrained_toggled(bool checked)
{

}

void tuning_ui::on_chk_showcam_toggled(bool checked)
{

}

void tuning_ui::on_chk_iirlpf_toggled(bool checked)
{

}
