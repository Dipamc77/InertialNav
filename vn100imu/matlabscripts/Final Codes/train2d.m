%clc; clear; close all;

sz = 5;
h = [zeros(sz-1,1);1];
%load hundredhertz.mat
x = imupositions(1,:);
d = encpositions(1,:);
xpad = [zeros(1,sz-1),x];
h = [zeros(sz-1,1);1];
mu = 10e-5;

P = zeros(sz,1);
R = zeros(sz,sz);

for i=1:numel(x)
    xwin = xpad(i:i+sz-1);
    P = (P*(i-1) + d(i)*xwin')/i;
    R = (R*(i-1) + xwin'*xwin)/i;
    o = xwin*h;
    e =d(i)-o;
    h = h + mu*xwin'*e;
end
plot(x); hold on
plot(d); hold on
%plot(out); hold on
%h = P\R;
xo = x;
xopad = [zeros(1,sz-1),xo];
filt = zeros(size(xo));
for i=1:numel(xo)
    xwin = xopad(i:i+sz-1);
    o = xwin*h;
    filt(i) = o;
end
plot(filt); hold off

legend('imu','encoder','trained','convolution');