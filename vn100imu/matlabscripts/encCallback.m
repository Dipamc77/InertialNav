function encCallback (~,encmsg)

encleft = encmsg.XYZ.X;
encright = encmsg.XYZ.Y;
movementflag = encmsg.XYZ.Z;
assignin('base','current_encleft',encleft);
assignin('base','current_encright',encright);
assignin('base','movementflag',movementflag);
assignin('base','newencreceived',1);

end