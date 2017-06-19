//#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "vn100imu/coord.h"

using namespace cv;
//using namespace std;

Mat inpImg;
ros::Publisher pubcoord;
bool imageReceived = false;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
   try
   {
     inpImg = (cv_bridge::toCvShare(msg, "bgr8")->image).clone(); // Don't remove this clone call, else it will not finish copying the whole image and overwrite it prematurely
     //imshow("Img",cv_bridge::toCvShare(msg, "bgr8")->image );
   }
   catch (cv_bridge::Exception& e)
   {
     ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
   }
   imageReceived=true;

}
int main(int argc, char** argv)
{
  ros::init(argc, argv, "CamTrackingNode");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber imageSub = it.subscribe("overheadCamImage",1,imageCallback);
  pubcoord = nh.advertise<vn100imu::coord>("trackedCoordinates", 1);

  sensor_msgs::ImagePtr msg;
  while(1)
  {
    if(imageReceived)
    {

      //trackOverhead() // TODO : Add this  function
      // publish tracked coordinate

      //  waitKey(2);
      imshow("Input",inpImg);
      imageReceived = false;
    }
    if((char)waitKey(1)==27)
    {
      break;
    }
    ros::spinOnce();
  }
}
