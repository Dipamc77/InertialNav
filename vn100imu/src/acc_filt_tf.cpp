#include <fstream>

using namespace std;
#include "ros/ros.h"
#include "vn100imu/xyz_data.h"
#include "std_msgs/Bool.h"
#include<cmath>
vn100imu::xyz_data accfmsg;
ros::Publisher pubaccf;
#define filtsize 6 // filter size is 11 window size is 10
float accfilt[3];
//MATLAB Function butter for LPF , [b,a] = butter(5,2*pi/10)
//Check help of MATLAB for b,a equation usage
float filta[filtsize] = {1.0000,1.2620,1.1928,0.5384,0.1508,0.0166};
float filtb[filtsize] = {0.1300,0.6501,1.3002,1.3002,0.6501,0.1300};
////MATLAB Function butter for LPF , [b,a] = butter(5,[0.01 2*pi/10],'bandpass')
//float filtb[filtsize] = {0.1292,0.0,-0.6459,0.0,1.2919,0.0,-1.2919,0.0,0.6459,0.0,-0.1292};
//float filta[filtsize] = {1.000,-3.7302,4.8653,-2.8015,1.7757,-1.9696,0.7399,0.1536,0.0499,-0.0667,-0.0164};
float win_inx[filtsize],win_iny[filtsize],win_inz[filtsize];
float win_outx[filtsize],win_outy[filtsize],win_outz[filtsize];
ofstream myfile;

int maxderiv = 10; // don't reduce this or else if it reaches by chance it won't get back easily

void applyfilter(float curr_accx,float curr_accy,float curr_accz)
{
  float deriv;                  // If derivative is too high, make it equal to the last value
  deriv = abs(win_inx[0] - curr_accx);
  curr_accx = (deriv>maxderiv) ? win_inx[0] : curr_accx;
  deriv = abs(win_iny[0] - curr_accy);
  curr_accy = (deriv>maxderiv) ? win_iny[0] : curr_accy;
  deriv = abs(win_inz[0] - curr_accz);
  curr_accz = (deriv>maxderiv) ? win_inz[0] : curr_accz;

  for(int i=filtsize-2;i>=0;i--)   // Input window shift update (Lastest in first position)
  {
    win_inx[i+1] = win_inx[i];
    win_iny[i+1] = win_iny[i];
    win_inz[i+1] = win_inz[i];
  }
  win_inx[0] = curr_accx;
  win_iny[0] = curr_accy;
  win_inz[0] = curr_accz;

  float filt_x=0.0,filt_y=0.0,filt_z=0.0;
  for(int i=1;i<filtsize;i++)         // Use IIR Filter
  {
    filt_x += win_inx[i]*filtb[i] - win_outx[i]*filta[i];
    filt_y += win_iny[i]*filtb[i] - win_outy[i]*filta[i];
    filt_z += win_inz[i]*filtb[i] - win_outz[i]*filta[i];
  }
  filt_x = (filt_x + win_inx[0]*filtb[0])/filta[0];
  filt_y = (filt_y + win_iny[0]*filtb[0])/filta[0];
  filt_z = (filt_z + win_inz[0]*filtb[0])/filta[0];
  accfilt[0] = filt_x;
  accfilt[1] = filt_y;
  accfilt[2] = filt_z;

  win_outx[0] = filt_x;
  win_outy[0] = filt_y;
  win_outz[0] = filt_z;
  for(int i=filtsize-2;i>=0;i--)     // Output window shift update (Lastest in first position)
  {
    win_outx[i+1] = win_outx[i];
    win_outz[i+1] = win_outz[i];
    win_outy[i+1] = win_outy[i];
  }
  myfile << curr_accx << endl;
}
void accelrecieved(const vn100imu::xyz_data::ConstPtr& accel)
{
  static int seq=0;
  seq++;
  accfmsg.header.seq = seq;
  accfmsg.header.stamp = accel->header.stamp;  // old timestamp, keep using same timestamp
  float period = accel->Period;
  applyfilter(accel->XYZ.x,accel->XYZ.y,accel->XYZ.z);
  accfmsg.XYZ.x = accfilt[0];
  accfmsg.XYZ.y = accfilt[1];
  accfmsg.XYZ.z = accfilt[2];
  accfmsg.Period=period;
  pubaccf.publish(accfmsg);
  ROS_INFO("Ax: %f  Ay: %f  Az: %f",accel->XYZ.x,accel->XYZ.y,accel->XYZ.z);
  ROS_INFO("Afx: %f  Afy: %f  Afz: %f",accfilt[0],accfilt[1],accfilt[2]);
}
void tareCallback(const std_msgs::Bool::ConstPtr& taresignal)
{
	if(taresignal->data)
  {
      accfilt[0] = 0.0; accfilt[1] = 0.0; accfilt[2] = 0.0;
      for(int i=0;i<filtsize;i++)
      { win_inx[i] = 0; win_iny[i] = 0; win_inz[i] = 0;
        win_outx[i] = 0; win_outy[i] = 0; win_outz[i] = 0;  }
	}
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "accfilt");

  myfile.open ("/home/r2d2/accelerationvals.txt");
  accfilt[0] = 0.0; accfilt[1] = 0.0; accfilt[2] = 0.0;
  for(int i=0;i<filtsize;i++)
  {  win_inx[i] = 0; win_iny[i] = 0; win_inz[i] = 0;
     win_outx[i] = 0; win_outy[i] = 0; win_outz[i] = 0;  }
  ros::NodeHandle n;
  ros::Subscriber subacc = n.subscribe<vn100imu::xyz_data>("/vn100imu/acceleration_data", 1, accelrecieved);
  ros::Subscriber subtare = n.subscribe<std_msgs::Bool>("/vn100imu/sendtarecommand", 1, tareCallback);
  pubaccf = n.advertise<vn100imu::xyz_data>("/vn100imu/acceleration_filtered", 1);
  ros::spin();
  myfile.close();
return 0;
}
