close all;

tlen = len;
sz = 15;

x = conv(accbodyvals(1,:),ones(1,16)/16)/10;
d = conv(accencmean_corrected,(ones(1,16)/16))/10;
%x = imubodyvels(1,:);
%d = encmeanvels;
xpad = [zeros(1,sz-1),x];
y = zeros(size(x));
nn = zeros(size(x));

hn = 30; % number of hidden neurons
wh = randn(hn,sz)/sqrt(hn); 
wh2 = randn(hn,hn)/sqrt(hn);
%wh = [ones(hn,1),zeros(hn,sz-1)];
wo = randn(1,hn);

actfh = @tansig;
actfo = @tansig;

lrate = 1/hn;
for sam = 1:10
for i = 1:len
    xwin = xpad(i:i+sz-1);
    xi = xwin;
    hout1 = actfh(xi*wh');
    xh1 = hout1;
    hout2 = actfh(xh1*wh2');
    xh = hout2;
    yout = actfo(xh*wo');
    y(i) = yout;
    delo = d(i)-yout;
    diff_o = (sech(yout)^2)*delo;
    dwo = diff_o*xh;
    for h = 1:hn
        delh = diff_o*wo(h);
        diff_h2(h) = (sech(hout2(h))^2)*delh;
        dwh = diff_h2(h)*xh1;
        wh2(h,:) = wh2(h,:) + lrate*dwh;
    end
    for h = 1:hn
        delh = diff_h2*wh2(h,:)';
        diff_h1 = sech(hout1(h)^2)*delh;
        dwh = diff_h1*xi;
        wh(h,:) = wh(h,:) + lrate*dwh;
    end
    wo = wo + lrate*dwo;
end
end

for i = 1:numel(x)
    xwin = xpad(i:i+sz-1);
    xi = xwin;
    hout1 = actfh(xi*wh');
    xh1 = hout1;
    hout2 = actfh(xh1*wh2');
    xh = hout2;
    yout = actfo(xh*wo');
    nn(i) = yout;
end


%plot(x); hold on
plot(d); hold on
plot(y); hold on
plot(nn); hold off
legend('encoder','online','trained');

%legend('encoder','trained');
figure;

trainedvel = cumsum(y(1:len)).*moveflags;

plot(cumsum(d)); hold on
plot(imuvels(1,:)); hold on
plot(cumsum(y)); hold on
plot(trainedvel); hold off
legend('filtered encoder','imu','online','trained');

trainedimuvels = zeros(2,len);
trainedimupositions = zeros(2,len);

for i = 2:len
    ypr = yprvals(:,i);
    yaw = deg2rad(ypr(1));
    pitch =  deg2rad(ypr(2));
    roll = deg2rad(ypr(3));
    dcm_ypr = angle2dcm(yaw,pitch,roll);
    trainedimuinervel = dcm_ypr'*[trainedvel(i);0;0];
    trainedimuvels(:,i) = trainedimuinervel(1:2);
    trainedimupositions(:,i) = trainedimupositions(:,i-1) + trainedimuvels(:,i)/samplingfreq;
end

figure;
plot(encpositions(1,:),-encpositions(2,:)); hold on
plot(imupositions(1,:),-imupositions(2,:)); hold on
plot(trainedimupositions(1,:)/5,-trainedimupositions(2,:)/5); hold on
legend('enc','imu','trained');

save('trainednn','sz','hn','wo','wh');