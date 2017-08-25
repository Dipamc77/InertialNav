%clc; clear; close all;

sz = 5;
h = [zeros(sz-1,1);1];
%load hundredhertz.mat
savedbodyvel = distbody;%allvals(1,500:1000);
savedencvel = distenc;%allvals(2,500:1000);
x = savedbodyvel(1000:2000);
d = savedencvel(1000:2000);
xpad = [zeros(1,sz-1),x];
mu = 10e-6;
out = zeros(1,numel(x));
for i=1:numel(x)
    xwin = xpad(i:i+sz-1);
    o = xwin*h;
    e =d(i)-o;
    h = h + mu*xwin'*e;
    out(i) = o;
    if(e~=0)
        debugging = 1;
    end
end
plot(savedbodyvel); hold on
plot(savedencvel); hold on
%plot(out); hold on
xo = savedbodyvel;
xopad = [zeros(1,sz-1),xo];
filt = zeros(size(xo));
for i=1:numel(xo)
    xwin = xopad(i:i+sz-1);
    o = xwin*h;
    filt(i) = o;
end
plot(filt); hold off

legend('imu','encoder','trained','convolution');