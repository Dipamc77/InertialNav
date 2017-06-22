#include "tuning_ui.h"
#include <QApplication>
#include <signal.h> //  our new library

volatile sig_atomic_t keyboadinterrupt = 0;
bool windowclosed;

void interrupthandler(int sig){ // can be called asynchronously
  keyboadinterrupt = 1; // set flag
}


int main(int argc, char *argv[])
{
    ros::init(argc, argv, "tuning_uinode");
    ros::NodeHandle nh;
    QApplication a(argc, argv);
    tuning_ui w(nh);
    w.show();
    ros::Rate rate(20);
    windowclosed = false;
    signal(SIGINT, interrupthandler);
    while(ros::ok() && !keyboadinterrupt && !windowclosed)
    {
        ros::spinOnce();
        rate.sleep();
        a.processEvents(); // TODO check how to put KeyboardInterrupt
    }
    return 0;//a.exec();
}
