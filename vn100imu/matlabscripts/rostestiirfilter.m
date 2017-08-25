clc; clear; close all;

current_accbody = zeros(3,1);
current_ypr = zeros(3,1);
newyprreceived=0;
newaccelreceived=0;
accsub = rossubscriber('/vn100imu/acceleration_data',@accelCallback);
yprsub = rossubscriber('/vn100imu/orientation_data',@yprCallback);

accbodyplot = zeros(3,100);
accinerplot = zeros(3,100);
accfiltplot = zeros(1,100);

filtsize = 5;
[b,a] = butter(filtsize,[0.01 0.628],'bandpass');
fs = 10;
acc_array = zeros(3,filtsize);

while(1)
    if(newyprreceived && newaccelreceived)
        newyprreceived=0;
        newaccelreceived=0;
        accbodyplot = [current_accbody,accbodyplot(:,1:end-1)];
        
        %yaw = current_ypr(1)*pi/180.0;
        %pitch = current_ypr(2)*pi/180.0;
        %roll = current_ypr(3)*pi/180.0;
        
        acc_array = [current_accbody,acc_array(:,1:filtsize-1)];
        acc_xa = acc_array(1,:);
        acc_xaf = filter(b,a,acc_xa);
        accfiltplot = [acc_xaf(end),accfiltplot(1,1:end-1)];
        
        %dcm_ypr = angle2dcm(yaw,pitch,roll);
        %current_accinertial = dcm_ypr*current_accbody;
        %accinerplot = [current_accinertial,accinerplot(:,1:end-1)];
        
        plot(accbodyplot(1,:),'b'); hold on
        plot(accfiltplot,'r'); hold off
        %subplot(1,3,1); plot(accbodyplot(1,:),'b'); hold on
        %subplot(1,3,1); plot(accinerplot(1,:),'r'); hold off
        %subplot(1,3,2); plot(accbodyplot(2,:),'b'); hold on
        %subplot(1,3,2); plot(accinerplot(2,:),'r'); hold off
        %subplot(1,3,3); plot(accbodyplot(3,:),'b'); hold on
        %subplot(1,3,3); plot(accinerplot(3,:),'r'); hold off
    end
    pause(1e-4);
end