package vn100imu;

public interface ypr_data extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "vn100imu/ypr_data";
  static final java.lang.String _DEFINITION = "std_msgs/Header        header\ngeometry_msgs/Vector3  YPR\n\n";
  std_msgs.Header getHeader();
  void setHeader(std_msgs.Header value);
  geometry_msgs.Vector3 getYPR();
  void setYPR(geometry_msgs.Vector3 value);
}
