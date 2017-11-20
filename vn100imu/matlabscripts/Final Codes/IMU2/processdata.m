load set2imu2.mat
samplingfreq = 50; % Hz
len = numel(moveflags);

accinervals = zeros(size(accbodyvals));
accinervals_corrected  = zeros(size(accbodyvals));
accencvals = zeros(size(encvals));
imuvels = zeros(2,len);
imupositions = zeros(2,len);
encvelsiner = zeros(size(encvals));
encpositions = zeros(size(encvals));
imubodyvels = zeros(3,len);

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
    acciner = dcm_ypr'*[accbody(1);accbody(2)*0;accbody(3)*0];
    accinervals(:,i) = acciner;
    encvals(:,i) = (encvals(:,i)>1).*encvals(:,i-1) + (encvals(:,i)<=1).*encvals(:,i);
    
    accencvals(:,i) = (encvals(:,i) - encvals(:,i-1))*samplingfreq;
    encvel = mean(encvals(:,i));
    %encoder position -> use yaw equation
    encvelsiner(:,i) = [cos(yaw);sin(yaw)]*encvel;
    encpositions(:,i) = encpositions(:,i-1) + [cos(yaw);sin(yaw)]*encvel/samplingfreq;
    
    %corrected acceleration -> use encoder data or movementflag
    if(~moveflags(i))%(sum(encvals(:,i))+sum(accencvals(:,i)) == 0) 
        %acc_corrected = zeros(2,1);
        imuvels(:,i) = zeros(2,1);
    else
        imubodyvels(:,i) = imubodyvels(:,i-1) +  [accbody(1);accbody(2:3)*0.0]/samplingfreq;
        imuinervel = dcm_ypr'*imubodyvels(:,i);
        acc_corrected = acciner(1:2);
        imuvels(:,i) = imuinervel(1:2);%imuvels(:,i-1) + acc_corrected/samplingfreq;
    end
   imupositions(:,i) = imupositions(:,i-1) + imuvels(:,i)/samplingfreq;
    %imu velocity -> use corrected acceleration
end 
