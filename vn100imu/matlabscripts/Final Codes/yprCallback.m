function yprCallback (~,yprmsg)

ypr = [yprmsg.YPR.X;yprmsg.YPR.Y;yprmsg.YPR.Z];
assignin('base','current_ypr',ypr);
assignin('base','newyprreceived',1);

end