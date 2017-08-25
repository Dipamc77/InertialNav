#include "botcontrol.h"
#include <QApplication>
#include <signal.h>

volatile sig_atomic_t keyboadinterrupt = 0;

void interrupthandler(int sigc){ // can be called asynchronously
  keyboadinterrupt = 1; // set flag
}

int main(int argc, char *argv[])
{

  ros::init(argc, argv, "botcontrolnode");
  ros::NodeHandle nh;
  QApplication a(argc, argv);
  botcontrol w(nh);
  w.show();
  ros::Rate rate(20);
  signal(SIGINT, interrupthandler);
  while(ros::ok() && !keyboadinterrupt)
  {
      ros::spinOnce();
      rate.sleep();
      a.processEvents(); // TODO check how to put KeyboardInterrupt
  }

    return 0;
}
