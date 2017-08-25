leftvel = encvals(1,:);
rightvel = encvals(2,:);
plot(leftvel,'o-')
hold on
plot(rightvel,'o-')
hold on


body_xacc = accinervals(1,:);
body_yacc = accinervals(2,:);

velx = zeros(size(body_xacc));
vely = zeros(size(body_yacc));
distenc = velx;
distx = velx;
disty = vely;

for i = 2:numel(velx)
    velx(i) = velx(i-1) + body_xacc(i)/50.0;
    vely(i) = vely(i-1) + body_yacc(i)/50.0;
    if(leftvel(i)==0 && rightvel(i) == 0)
        velx(i) = 0;
        vely(i) = 0;
    end
    distenc(i) = distenc(i-1) + (rightvel(i)+leftvel(i))/2/50;
    distx(i) = distx(i-1) + velx(i)/50.0;
    disty(i) = disty(i-1) + vely(i)/50.0;
end

plot(velx,'o-'); hold on
plot(vely,'o-'); hold off
legend('encleft','encright','imux','imuy');

figure;

plot(distenc,'-'); hold on
plot(distx,'-'); hold on
plot(disty,'-'); hold off
figure;

%R = 14*leftvel./(leftvel-rightvel);
%plot(R,'o-')

 encaccright = encvals(4,:);
 encaccleft = encvals(3,:);
 
 %plot(encaccleft,'-'); hold on
 %plot(encaccright,'-'); hold on
 plot(body_xacc,'o-'); hold on
 plot(body_yacc,'-'); hold off
 legend('encleft','encright','imux','imuy');
% %legend('x','enc','y','z')