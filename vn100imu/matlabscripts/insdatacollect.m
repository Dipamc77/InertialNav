clc; clear; close all;

current_accbody = zeros(3,1);
current_ypr = zeros(3,1);
current_encleft = 0;
current_encright = 0;
movementflag = 0;

newencreceived=0;
newaccelreceived=0;
newyprreceived=0;

encvals = zeros(2,1);
accbodyvals = zeros(3,1);
yprvals = zeros(3,1);
moveflags = 0;

accsub = rossubscriber('/vn100imu/acceleration_data',@accelCallback);
yprsub = rossubscriber('/vn100imu/orientation_data',@yprCallback);
encsub = rossubscriber('/encoder/velocity_data',@encCallback);

while(1)
    if(newencreceived && newaccelreceived && newyprreceived)
       newencreceived=0;
       newaccelreceived=0;
       newyprreceieved = 0;
        
       encvals = [encvals,[current_encleft; current_encright;]];
       accbodyvals = [accbodyvals, current_accbody];
       yprvals = [yprvals,current_ypr];
       moveflags = [moveflags,movementflag];
          
    end
    pause(1e-4);
end