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

allvals = zeros(4,1);

accwindow = zeros(1,3);
encwindow = zeros(1,3);
prevenc = 0;
tic;
kg = 0.5;
filtenc = 0;
est =0;

while(1)
    if(newencreceived && newaccelreceived)
        newencreceived=0;
        newaccelreceived=0;
        toc;
        tic;
        
        if(accenc == 0 && current_enc == 0)
            current_accbody(1) = 0;
            velbody = 0;
            filtenc = current_enc;
            est = 0;
        else
            emea = filtenc - current_enc;
            eest = filtenc -est;
            kg = eest/(eest+emea);
            
        end
       % current_accbody = (abs(current_accbody)>0.06).*current_accbody;
       
       
        
        accwindow = [current_accbody(1),accwindow(1:end-1)];
        accenc = current_enc-prevenc;           % 100 hz and 100 cm to 1 m cancel out
        encwindow = [accenc,encwindow(1:end-1)];
        prevenc = current_enc;
        
        accfilt = sum(accwindow)/3;
        encfilt = sum(encwindow)/3;
        
        accbodyplot = [current_accbody,accbodyplot(:,1:end-1)];
        accencplot = [accenc,accencplot(1:end-1)];
        
       if(current_enc ~= 0)
            distmovedenc  = distmovedenc + current_enc;
          %  disp(distmoved)
            velbody = velbody + current_accbody(1);
            distmovedimu  = distmovedimu + velbody;
            disp([distmovedenc,distmovedimu]);
       end
       
       velbodyplot = [velbody,velbodyplot(1:end-1)];
       velencplot = [current_enc,velencplot(1:end-1)];
       
       allvals = [allvals,[velbody; current_enc; current_accbody(1); accenc]];
        
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