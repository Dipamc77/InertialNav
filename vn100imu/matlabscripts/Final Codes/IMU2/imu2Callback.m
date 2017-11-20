function imu2Callback (~,accelmsg)

acc = [accelmsg.XYZ.X;accelmsg.XYZ.Y;accelmsg.XYZ.Z];
assignin('base','current_imu2accbody',acc);
assignin('base','newimu2received',1);

end