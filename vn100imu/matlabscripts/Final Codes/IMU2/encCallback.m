function encCallback (~,encmsg)

encleft = encmsg.XYZ.X;
encright = encmsg.XYZ.Y;
movementflag = floor(encmsg.XYZ.Z/1000);
pwmval = mod((encmsg.XYZ.Z),1000);
assignin('base','current_encleft',encleft);
assignin('base','current_encright',encright);
assignin('base','movementflag',movementflag);
assignin('base','pwmval',pwmval);
assignin('base','newencreceived',1);

end