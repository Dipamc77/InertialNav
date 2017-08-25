clc; clear; close all;

current_accbody = zeros(3,1);
current_accinertial = zeros(3,1);
current_ypr = zeros(3,1);
current_encleft = 0;
current_encright = 0;
newencreceived=0;
newaccelreceived=0;
accsub = rossubscriber('/vn100imu/acceleration_data',@accelCallback);
yprsub = rossubscriber('/vn100imu/orientation_data',@yprCallback);
encsub = rossubscriber('/encoder/velocity_data',@encCallback);

accbodyplot = zeros(3,1000);
accencplot = zeros(1,1000);
velbodyplot = zeros(1,1000);
velencplot = zeros(1,1000);

accencright = 0;
accencleft = 0;
velbody =0;

distmovedenc = 0;
distmovedimu = 0;

encvals = zeros(4,1);
accbodyvals = zeros(3,1);
accinervals = zeros(3,1);
yprvals = zeros(3,1);

prevencright = 0;
prevencleft = 0;
%tic;

while(1)
    if(newencreceived && newaccelreceived && newyprreceived)
        newencreceived=0;
        newaccelreceived=0;
        newyprreceieved = 0;
        
       yaw = current_ypr(1)*pi/180.0;
       pitch = current_ypr(2)*pi/180.0;
       roll = current_ypr(3)*pi/180.0;
       dcm_ypr = angle2dcm(yaw,pitch,roll);
       current_accinertial = dcm_ypr*current_accbody;
        
      %  toc;
       % tic;
        
        if(accencright == 0 && current_encright == 0 && accencleft == 0 && current_encleft == 0)
            current_accbody(1) = 0;
            current_accbody(2) = 0;
            current_accbody(3) = 0;
            velbody = 0;
        end
       % current_accbody = (abs(current_accbody)>0.06).*current_accbody;
        
        accencleft = (current_encleft-prevencleft)*50;         
        prevencleft = current_encleft;
 
        accencright = (current_encright-prevencright)*50;         
        prevencright = current_encright;
        
%        if(current_encright ~= 0)
%             distmovedenc  = distmovedenc + current_enc;
%           %  disp(distmoved)
%             velbody = velbody + current_accbody(1)/50.0;
%             distmovedimu  = distmovedimu + velbody/50.0;
%             disp([distmovedenc,distmovedimu]);
%        end
       
       encvals = [encvals,[current_encleft; current_encright; accencleft; accencright;]];
       accbodyvals = [accbodyvals, current_accbody];
       accinervals = [accinervals, current_accinertial];
       yprvals = [yprvals,current_ypr];
          
    end
    pause(1e-4);
end