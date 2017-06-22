#include "ros/ros.h"
#include "vn100imu/xyz_data.h"
#include "vn100imu/ypr_data.h"

float acc[3],ypr[3];
bool accrec,yprrec;
ros::Publisher pubtrueacc;

void newaccelrecieved(const vn100imu::xyz_data::ConstPtr& accel)
{
  acc[0] = accel->XYZ.x;
  acc[1] = accel->XYZ.y;
  acc[2] = accel->XYZ.z;
  accrec = true;
}
void newyprrecieved(const vn100imu::ypr_data::ConstPtr& yprdata)
{
  ypr[0] = yprdata->YPR.x;
  ypr[1] = yprdata->YPR.y;
  ypr[2] = yprdata->YPR.z;
  yprrec = true;
}
void matrix_mul(float A[3][3],float B[3][3],float C[3][3])
{
    float sum=0;
    for(int i=0; i<3; i++)
        {   for(int j=0; j<3; j++)
            {	sum=0;
                for(int k=0; k<3; k++)
                {	sum = sum + A[i][k] * B[k][j]; }
            C[i][j] = sum;
            }
        }
}
void generateDCM(float YPR[3],float DCM[3][3])
{
    float pi = 3.1415;
    float y = YPR[0]*pi/180.0;  //  gamma
    float p = YPR[1]*pi/180.0;  // beta
    float r = YPR[2]*pi/180.0; // theta

    float DCMtemp[3][3] =
    {{ cos(p)*cos(y) , -cos(r)*sin(y) + sin(r)*sin(p)*cos(y) ,  sin(r)*sin(y) + cos(r)*sin(p)*cos(y)},
     { cos(p)*sin(y) ,  cos(r)*cos(y) + sin(r)*sin(p)*sin(y) , -sin(r)*cos(y) + cos(r)*sin(p)*sin(y)},
     {-sin(p)        ,  sin(r)*cos(p)                        ,  cos(r)*cos(p)                       }};

  for(int i = 0;i<3;i++)
    for(int j = 0;j<3;j++)
        DCM[i][j] = DCMtemp[i][j];
}
void calcInertialYPR()
{
  float C[3][3];
  float dcm[3][3];
  float acc33[3][3] = {{acc[0],acc[1],acc[2]},{0,0,0},{0,0,0}};
  generateDCM(ypr,dcm);
  matrix_mul(dcm,acc33,C);
  ROS_INFO("Axb: %f  Ayb: %f  Azb: %f  Axi: %f  Ayi: %f  Azi: %f",acc[0],acc[1],acc[2],C[0][0],C[0][1],C[0][2]);
}
int main(int argc, char **argv)
{
  yprrec = false;
  accrec = false;
  ros::init(argc, argv, "dcmnode");
  ros::NodeHandle n;
  ros::Subscriber subacc = n.subscribe<vn100imu::xyz_data>("/vn100imu/acceleration_data", 1, newaccelrecieved);
  ros::Subscriber subypr = n.subscribe<vn100imu::ypr_data>("/vn100imu/orientation_data", 1, newyprrecieved);
  while(1)
  {
    if(yprrec && accrec)
    {
      calcInertialYPR();
      yprrec = false;
      accrec = false;
    }
    ros::spinOnce();
  }
return 0;
}
