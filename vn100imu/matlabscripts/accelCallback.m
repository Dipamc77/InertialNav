function accelCallback (~,accelmsg)

acc = [accelmsg.XYZ.X;accelmsg.XYZ.Y;accelmsg.XYZ.Z];
assignin('base','current_accbody',acc);
assignin('base','newaccelreceived',1);

end