%clc; clear; close all;
samplingfreq = 50; % Hz
len = numel(moveflags);

accinervals = zeros(size(accbodyvals));
accinervals_corrected  = zeros(size(accbodyvals));
accencvals = zeros(size(encvals));
imuvels = zeros(2,len);
imupositions = zeros(2,len);
encpositions = zeros(size(encvals));

%inertial acceleration
%corrected acceleration(from encoder)
%encoder acceleration
%imu velocites
%imu positions
%encoder positions
for i = 2:len
    ypr = yprvals(:,i);
    yaw = deg2rad(ypr(1));
    pitch =  deg2rad(ypr(2));
    roll = deg2rad(ypr(3));
    dcm_ypr = angle2dcm(yaw,pitch,roll);
    
    accbody = accbodyvals(:,i);
    acciner = dcm_ypr*accbody;
    accinervals(:,i) = acciner;
    
    accencvals(:,i) = (encvals(:,i) - encvals(:,i-1))*samplingfreq;
    encvel = mean(encvals(:,i));
    %encoder position -> use yaw equation
    
    encpositions(:,i) = encpositions(:,i-1) + [cos(yaw);sin(yaw)]*encvel/samplingfreq;
    
    %corrected acceleration -> use encoder data or movementflag
    if(sum(encvals(:,i))+sum(accencvals(:,i)) == 0) 
        %acc_corrected = zeros(2,1);
        imuvels(:,i) = zeros(2,1);
    else
        acc_corrected = acciner(1:2);
        imuvels(:,i) = imuvels(:,i-1) + acc_corrected/samplingfreq;
    end
   imupositions(:,i) = imupositions(:,i-1) + imuvels(:,i)/samplingfreq;
    %imu velocity -> use corrected acceleration
end 
figure;
plot(encpositions(1,:),-encpositions(2,:));
hold on
%plot(imupositions(1,:)*1.8/2.3,-imupositions(2,:)*0.16/0.4);
%hold on
plot(imupositions(1,:),-imupositions(2,:));
hold on
filteredimupos = [conv(imupositions(1,:),h);conv(imupositions(2,:),h)];
plot(filteredimupos(1,1:end-sz+1),-filteredimupos(2,1:end-sz+1));
% figure;
% subplot(1,2,1);
% plot(imuvels(1,:));
% hold on
% plot(encvals(1,:));
% hold off
% subplot(1,2,2);
% plot(imuvels(2,:));
% hold on
% %plot(encvals(2,:));
% 
% figure;
% subplot(1,2,1);
% plot(accinervals(1,:)); hold on
% plot(accencvals(1,:));
% subplot(1,2,2);
% plot(accinervals(2,:)); hold on
% %plot(accencvals(2,:));
% 
% 
