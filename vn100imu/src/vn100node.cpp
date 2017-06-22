#include<iostream>
//#include<unistd.h>
//#include<math.h>
//#include<stdlib.h>
//vectornav source header
#include "vectornav.h"
#include "vn100.h"
#include<ros/ros.h>
//adding message type headers
#include<vn100imu/xyz_data.h>
#include<vn100imu/ypr_data.h>
#include<vn100imu/YPR.h>
#include "std_msgs/Bool.h"
#include "std_msgs/String.h"
//#include<cmath>

//Publishers
	ros::Publisher pubypr_data;
	ros::Publisher pubacc_data;
	ros::Publisher pubdelv_data;

//Device
	Vn100 vnsens;
	std::string imu_frame_id;
	void vnerror_msg(VN_ERROR_CODE vn_error,std::string &msg);
//printing the asynchronous data;
VnVector3 TrueAccel;
VnVector3 Gyro;
VnYpr YPR;
float sampletime,period;
ros::Time prevtime;
ros::Duration sampletimediff,nextsampletimediff;
bool readSuccess = false;
bool usevn100flag_fromui = false;

void publish_device()
{
	if(!usevn100flag_fromui)
		return;
	static int seq=0;
	seq++;
	VN_ERROR_CODE vn_error;
	std::string vn_err_msg;
	ros::Time timestamp;
	timestamp=ros::Time::now();
	vn_error=vn100_getYawPitchRollTrueBodyAccelerationAngularRate(&vnsens,&YPR,&TrueAccel,&Gyro);
	//vn_error=vn100_getYawPitchRollTrueInertialAccelerationAngularRate(&vnsens,&YPR,&TrueAccel,&Gyro);
	sampletimediff = ros::Time::now()-timestamp;
	sampletime = (sampletimediff.toSec())*1e3;  // print this
  nextsampletimediff = timestamp - prevtime;
	period = (nextsampletimediff.toSec())*1e3; // print this
	prevtime = timestamp;
	if(vn_error!=VNERR_NO_ERROR)
	{
		vnerror_msg(vn_error,vn_err_msg);
		ROS_INFO("error in reading the sensor data:%s",vn_err_msg.c_str());
		readSuccess = false;
		return;
	}
	else
	{
		readSuccess = true;
		ROS_INFO("Sampletime(ms): %f\tPeriod(ms): %f",sampletime,period);
	}

	if(pubacc_data.getNumSubscribers()>0)
	{
		//ROS_INFO("subscribed");
		vn100imu::xyz_data msg_acc_data;
		msg_acc_data.header.seq=seq;
		msg_acc_data.header.stamp=timestamp;
		msg_acc_data.header.frame_id=imu_frame_id;
		//msg_acc_data.XYZ.x=(abs(TrueAccel.c0)<6.0)?TrueAccel.c0:0.0;  // TODO find better way of handling spikes
		//msg_acc_data.XYZ.y=(abs(TrueAccel.c1)<6.0)?TrueAccel.c1:0.0;	// TODO try low pass filter instead
		//msg_acc_data.XYZ.z=(abs(TrueAccel.c2)<6.0)?TrueAccel.c2:0.0;
		msg_acc_data.XYZ.x = TrueAccel.c0;
		msg_acc_data.XYZ.y = TrueAccel.c1;
		msg_acc_data.XYZ.z = TrueAccel.c2;
		msg_acc_data.Period=period*1e-3;
		pubacc_data.publish(msg_acc_data);
	}
	if(pubypr_data.getNumSubscribers()>0)
	{
		//ROS_INFO("subscribed");
		vn100imu::ypr_data msg_ypr_data;
		msg_ypr_data.header.seq=seq;
		msg_ypr_data.header.stamp=timestamp;
		msg_ypr_data.header.frame_id=imu_frame_id;
		msg_ypr_data.YPR.x=YPR.yaw;
		msg_ypr_data.YPR.y=YPR.pitch;
		msg_ypr_data.YPR.z=YPR.roll;
		pubypr_data.publish(msg_ypr_data);
	}
	if(pubdelv_data.getNumSubscribers()>0)
	{
		VnVector3 delta_Velocity,delta_Theta;
		float delta_Time;
		vn_error = vn100_getdeltaVelocity(&vnsens,&delta_Time,&delta_Theta,&delta_Velocity);
		if(vn_error!=VNERR_NO_ERROR)
		{
			vnerror_msg(vn_error,vn_err_msg);
			ROS_INFO("error in reading the velocity data:%s",vn_err_msg.c_str());
		}
		else
		{
			vn100imu::xyz_data velocity_data;
			velocity_data.header.seq = seq;
			velocity_data.header.stamp=timestamp;
			velocity_data.header.frame_id=imu_frame_id;
			//velocity_data.delta_Time=delta_Time;
			//velocity_data.delta_Theta.x=delta_Theta.c0;
			//velocity_data.delta_Theta.y=delta_Theta.c1;
			//velocity_data.delta_Theta.y=delta_Theta.c1;
			velocity_data.XYZ.x=delta_Velocity.c0;
			velocity_data.XYZ.y=delta_Velocity.c1;
			velocity_data.XYZ.z=delta_Velocity.c2;
			velocity_data.Period=period*1e-3;
			pubdelv_data.publish(velocity_data);
			ROS_INFO("deltatime %f",delta_Time);
			ROS_INFO( "dvx: %f  dvy: %f  dvz: %f",delta_Velocity.c0,delta_Velocity.c1,delta_Velocity.c2);
		}
	}
	if(readSuccess)
		ROS_INFO("Y: %f  P: %f  R: %f  Ax: %f  Ay: %f  Az: %f",YPR.yaw,YPR.pitch,YPR.roll,TrueAccel.c0,TrueAccel.c1,TrueAccel.c2);
}
//defining timer publication
void publish_timer(const ros::TimerEvent&)
{
	publish_device();
}
void uimessagehandler(const std_msgs::String::ConstPtr& uimessage)
{
	std::string message = uimessage->data;
	if(message.compare("VN100 Publish On") == 0)
		usevn100flag_fromui = true;
	else if (message.compare("VN100 Publsih Off") == 0)
		usevn100flag_fromui = false;
}
//Function defining possible errors in using vectornav ins
void vnerror_msg(VN_ERROR_CODE vn_error,std::string &msg)
{
	switch(vn_error)
	{
		case VNERR_NO_ERROR:
		  msg="No Error";
		  break;
		case VNERR_UNKNOWN_ERROR:
		  msg="UnKnownError";
		  break;
		case VNERR_NOT_IMPLEMENTED:
		  msg="Not implemented";
		  break;
		case VNERR_TIMEOUT:
		  msg="TimeOut";
		  break;
		case VNERR_INVALID_VALUE:
		  msg="Invalid value";
		  break;
		case VNERR_FILE_NOT_FOUND:
		  msg="File not Found";
		  break;
		case VNERR_NOT_CONNECTED:
		  msg="Not Connected";
 		 break;
		default:
		  msg="Undefined error";
	}
}
void tareCallback(const std_msgs::Bool::ConstPtr& taresignal)
{
	if(taresignal->data)
	{
		VN_ERROR_CODE vn_error;
		std::string vn_err_msg;
		vn_error=vn100_tare(&vnsens,1);
	}
}
int main(int argc,char** argv)
{
	ros::init(argc,argv,"rawvn100Data"); ////initializing ros node
	ros::NodeHandle n;
	ros::NodeHandle np("~");        //creating public and private nodehandlers to handle ros publish services and private parameters
	std::string port;
	int baudrate,publish_rate,async_output_rate,async_output_type;
	std::string enable,headingMode,filteringMode,tuningMode;
	np.param<std::string>("serial_port",port,"/dev/ttyUSB0");
	np.param<int>("serial_baud",baudrate,115200);
	np.param<int>("publish_rate",publish_rate,10);
	//np.param<int>("async_output_type",async_output_type,0);
	//np.param<int>("async_output_rate",async_output_rate,100);//assigning params to variables
  np.param<std::string>("VPE_enable",enable,"1");
	np.param<std::string>("VPE_headingMode",headingMode,"1"); // TODO : check this headingmode in vn100 manual
  np.param<std::string>("VPE_filteringMode",filteringMode,"1");
	np.param<std::string>("VPE_tuningMode", tuningMode,"1");

	pubypr_data  =np.advertise<vn100imu::ypr_data> ("/vn100imu/orientation_data",1);
	pubacc_data  =np.advertise<vn100imu::xyz_data> ("/vn100imu/acceleration_data",1);//Initializing
  pubdelv_data =np.advertise<vn100imu::xyz_data> ("/vn100imu/deltavelocity",1);
	ros::Subscriber sub = n.subscribe<std_msgs::Bool>("/vn100imu/sendtarecommand", 1, tareCallback);
	ros::Subscriber subui = n.subscribe<std_msgs::String>("/tuning_ui/uimessages",1,uimessagehandler);
	VN_ERROR_CODE vn_err;         //dealing with vectornav errors
	std::string vn_error_msg;
	ROS_INFO("connecting to vnsens. port: %s at a baudrate:%d\n",
	                        port.c_str(),
	                        baudrate);
	vn_err=vn100_connect(&vnsens,port.c_str(),baudrate);//connecting to vectornav
	if(vn_err!=VNERR_NO_ERROR)    //in case of any error
	{
		vnerror_msg(vn_err,vn_error_msg);
		ROS_FATAL("Could not connect to the sensor on this %s port error:%s\n did you add the user to the dialout group???",
       	        port.c_str(),
	          vn_error_msg.c_str()
		);
		exit(EXIT_FAILURE);
	}

	//enable vpe funtion

	int trialCount = 10;
	vn_err = vn100_getVpeControl(&vnsens,(unsigned char*) enable.c_str(),(unsigned char*) headingMode.c_str(),(unsigned char*)filteringMode.c_str(),(unsigned char*) tuningMode.c_str());
	if(vn_err!=VNERR_NO_ERROR)
	{
		while(trialCount-- && vn_err!=VNERR_NO_ERROR)
		{
			vnerror_msg(vn_err,vn_error_msg);
			ROS_FATAL("Unable to enable the VPE");
			vn_err = vn100_getVpeControl(&vnsens,(unsigned char*) enable.c_str(),(unsigned char*) headingMode.c_str(),(unsigned char*)filteringMode.c_str(),(unsigned char*) tuningMode.c_str());
		}
		if(trialCount==-1)
		{
			ROS_FATAL("VPE Cannot Initialize");
			exit(EXIT_FAILURE);
		}
	}
	vn100_tare(&vnsens,1);
	prevtime = ros::Time::now();
//enabling ros timer to publish the data at the particular intervals
	ros::Timer pub_timer;
	ROS_INFO("publishing at %d Hz\n",publish_rate);
	pub_timer=np.createTimer(ros::Duration(1.0/(double)publish_rate),publish_timer);
	ros::spin();
	vn100_disconnect(&vnsens);
	return 0;
}
