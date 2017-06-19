#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

using namespace cv;

Mat camImg,plotImg,overlayedImg;

image_transport::Publisher overlPub;


bool overheadReceived = false, plotReceived = false;

void overheadCallback(const sensor_msgs::ImageConstPtr& msg)
{
   try
   {
     camImg = (cv_bridge::toCvShare(msg, "bgr8")->image).clone(); // Don't remove this clone call, else it will not finish copying the whole image and overwrite it prematurely
     //imshow("Img",cv_bridge::toCvShare(msg, "bgr8")->image );
   }
   catch (cv_bridge::Exception& e)
   {
     ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
   }
   overheadReceived=true;

}
void plotCallback(const sensor_msgs::ImageConstPtr& msg)
{
   try
   {
     plotImg = (cv_bridge::toCvShare(msg, "bgr8")->image).clone(); // Don't remove this clone call, else it will not finish copying the whole image and overwrite it prematurely
     //imshow("Img",cv_bridge::toCvShare(msg, "bgr8")->image );
   }
   catch (cv_bridge::Exception& e)
   {
     ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
   }
   plotReceived=true;

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "OverlayNode");
  ros::NodeHandle nh;
  srand(time(NULL));
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber overheadSub = it.subscribe("overheadCamImage",1,overheadCallback);
  image_transport::Subscriber plotSub = it.subscribe("dataplot2d",1,plotCallback);
  overlPub = it.advertise("overlayImage", 1);

  sensor_msgs::ImagePtr img_msg;
  while(1)
  {
    if(overheadReceived && plotReceived)
    {
      overlayedImg = camImg.clone();
      for(int i=0;i<overlayedImg.rows;i++)
      {
        for(int j=0;j<overlayedImg.cols;j++)
        {
          Vec3b color = plotImg.at<Vec3b>(i,j);
          if(!(color[0] == 0 && color[1] == 0 && color[2] == 0))
          {
            overlayedImg.at<Vec3b>(i,j) = color;
          }
        }
      }
      imshow("Overlay",overlayedImg);
      overheadReceived = false; plotReceived = false;
    }
    if((char)waitKey(1)==27)
    {
      break;
    }
    ros::spinOnce();
  }
}
