#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "vn100imu/coord.h"
#include<time.h>
#include <deque>

using namespace cv;
using namespace std;

Mat outImg;
image_transport::Publisher plotPub;
deque< Point > points(1000);
sensor_msgs::ImagePtr img_msg;

void timercallback(const ros::TimerEvent&)
{
  for(int i=0;i<100;i++)      // Change all this crap to combiner code
  {
    points[i].x = rand() % 640;
    points[i].y = rand() % 480;
  }
  Scalar color;
  outImg = Mat::zeros(480,640,CV_8UC3);
  for(int i = 0;i<100;i++)
  {
    color[0] = rand() % 256;
    color[1] = rand() % 256;
    color[2] = rand() % 256;
    circle(outImg,points[i],2,color,-1);
  }

  // publish the image
  img_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", outImg).toImageMsg();
  plotPub.publish(img_msg);

  //imshow("Dance",outImg);
  //waitKey(10);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "Plot2dNode");
  ros::NodeHandle nh;
  srand(time(NULL));
  image_transport::ImageTransport it(nh);
  plotPub = it.advertise("dataplot2d", 1);
  int publish_rate =  10;
  ros::Timer pub_timer;
  pub_timer=nh.createTimer(ros::Duration(1.0/(double)publish_rate),timercallback);
  ros::spin();
}
