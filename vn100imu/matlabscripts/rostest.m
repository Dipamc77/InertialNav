clc; clear; close all;

current_accbody = zeros(3,1);
current_ypr = zeros(3,1);
current_enc = 0;
newencreceived=0;
newaccelreceived=0;
accsub = rossubscriber('/vn100imu/acceleration_data',@accelCallback);
yprsub = rossubscriber('/vn100imu/orientation_data',@yprCallback);
encsub = rossubscriber('/encoder/velocity_data',@encCallback);

accbodyplot = zeros(3,1000);
accencplot = zeros(1,1000);
velbodyplot = zeros(1,1000);
velencplot = zeros(1,1000);

accenc = 0;
velbody =0;

distmovedenc = 0;
distmovedimu = 0;

allvals = zeros(6,1);

accwindow = zeros(1,3);
encwindow = zeros(1,3);
prevenc = 0;
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
        
        if(accenc == 0 && current_enc == 0)
            current_accbody(1) = 0;
            velbody = 0;
        end
       % current_accbody = (abs(current_accbody)>0.06).*current_accbody;
        
        accwindow = [current_accbody(1),accwindow(1:end-1)];
        accenc = (current_enc-prevenc)*50;         
        encwindow = [accenc,encwindow(1:end-1)];
        prevenc = current_enc;
        
        accfilt = sum(accwindow)/3;
        encfilt = sum(encwindow)/3;
        
        accbodyplot = [current_accbody,accbodyplot(:,1:end-1)];
        accencplot = [accenc,accencplot(1:end-1)];
        
       if(current_enc ~= 0)
            distmovedenc  = distmovedenc + current_enc;
          %  disp(distmoved)
            velbody = velbody + current_accbody(1)/50.0;
            distmovedimu  = distmovedimu + velbody/50.0;
            disp([distmovedenc,distmovedimu]);
       end
       
       velbodyplot = [velbody,velbodyplot(1:end-1)];
       velencplot = [current_enc,velencplot(1:end-1)];
       
       allvals = [allvals,[velbody; current_enc; current_accbody(1); accenc; current_accbody(2); current_accbody(3)]];
       yprvals = [yprvals,current_ypr'];
        
      % if(accenc == 0 && current_enc == 0)
      %  subplot(2,1,2); plot(velbodyplot(1,:),'b'); hold on
      %  subplot(2,1,2); plot(velencplot,'r'); hold off 
      %  
      %  subplot(2,1,1); plot(accbodyplot(1,:),'b'); hold on
      %  subplot(2,1,1); plot(accencplot,'r'); hold off
      % end
        
    end
    pause(1e-4);
end