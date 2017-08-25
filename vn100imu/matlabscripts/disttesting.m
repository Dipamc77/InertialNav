load yzdata.mat
savedbodyvel = allvals(1,:);
savedencvel = allvals(2,:);
savedbody_yacc = allvals(5,:);
savedbody_zacc = allvals(6,:);
plot(savedbodyvel,'o-')
hold on
plot(savedencvel,'o-')

distbody = zeros(size(savedbodyvel));
distenc = zeros(size(savedencvel));
disty = zeros(size(savedbody_yacc));
distz = zeros(size(savedbody_zacc));
vely = disty;
velz = distz;

for i = 2:numel(savedbodyvel)
    distbody(i) = distbody(i-1) + savedbodyvel(i)/50;
    distenc(i) = distenc(i-1) + savedencvel(i);
    vely(i) = vely(i-1) + savedbody_yacc(i)/50.0;
    disty(i) = disty(i-1) + vely(i)/50;
    velz(i) = velz(i-1) + savedbody_zacc(i)/50.0;
    distz(i) = distz(i-1) + velz(i)/50;
end

figure
plot(distbody,'-')
hold on
plot(distenc,'-')
hold on
plot(disty,'-')
hold on
plot(distz,'-')
legend('x','e','y','z')