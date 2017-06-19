#include "ros/ros.h"
#include "vn100imu/xyz_data.h"
#include "std_msgs/Bool.h"
#include<cmath>
vn100imu::xyz_data velmsg;
ros::Publisher pubvel;
float vel[3];

void newaccelrecieved(const vn100imu::xyz_data::ConstPtr& accel)
{
  static int seq=0;
  seq++;
  velmsg.header.seq = seq;
  velmsg.header.stamp = accel->header.stamp;  // old timestamp, keep using same timestamp
  float period = accel->Period;
  float minaccel = 1;
  vel[0] += ((abs(accel->XYZ.x)>minaccel) ? accel->XYZ.x : 0)*period; // TODO Change the threshold or validate the use
  vel[1] += ((abs(accel->XYZ.y)>minaccel) ? accel->XYZ.y : 0)*period;
  vel[2] += ((abs(accel->XYZ.z)>minaccel) ? accel->XYZ.z : 0)*period;
  velmsg.XYZ.x = vel[0];
  velmsg.XYZ.y = vel[1];
  velmsg.XYZ.z = vel[2];
  velmsg.Period=period;
  pubvel.publish(velmsg);
  ROS_INFO("Vx: %f  Vy: %f  Vz: %f",vel[0],vel[1],vel[2]);
}
void tareCallback(const std_msgs::Bool::ConstPtr& taresignal)
{
	if(taresignal->data)
  {
      vel[0] = 0.0; vel[1] = 0.0; vel[2] = 0.0;
	}
}
int main(int argc, char **argv)
{
  vel[0] = 0.0; vel[1] = 0.0; vel[2] = 0.0;
  ros::init(argc, argv, "veldata");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe<vn100imu::xyz_data>("/vn100imu/acceleration_filtered", 1, newaccelrecieved);
  ros::Subscriber taresub = n.subscribe<std_msgs::Bool>("/vn100imu/sendtarecommand", 1, tareCallback);
  pubvel = n.advertise<vn100imu::xyz_data>("/vn100imu/velocity_data", 1);
  ros::spin();
return 0;
}
