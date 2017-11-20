clc; clear; close all;

current_accbody = zeros(3,1);
current_ypr = zeros(3,1);
current_imu2accbody = zeros(3,1);
current_encleft = 0;
current_encright = 0;
movementflag = 0;

newencreceived=0;
newaccelreceived=0;
newyprreceived=0;
newimu2received=0;

encvals = zeros(2,1);
accbodyvals = zeros(3,1);
imu2vals = zeros(3,1);
yprvals = zeros(3,1);
moveflags = 0;
pwmvals = 0;

accsub = rossubscriber('/vn100imu/acceleration_data',@accelCallback);
yprsub = rossubscriber('/vn100imu/orientation_data',@yprCallback);
encsub = rossubscriber('/encoder/velocity_data',@encCallback);
%imu2sub = rossubscriber('/imu2/acceleration_data',@imu2Callback);

while(1)
    if(newencreceived && newaccelreceived && newyprreceived)
       newencreceived=0;
       newaccelreceived=0;
       newyprreceieved = 0;
        
       encvals = [encvals,[current_encleft; current_encright;]];
       accbodyvals = [accbodyvals, current_accbody];
       yprvals = [yprvals,current_ypr];
       moveflags = [moveflags,movementflag];
       pwmvals = [pwmvals,pwmval*movementflag];
       imu2vals = [imu2vals, current_imu2accbody];
       
    end
    pause(1e-4);
end