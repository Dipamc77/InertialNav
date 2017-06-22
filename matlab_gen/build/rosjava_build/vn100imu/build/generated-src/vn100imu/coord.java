package vn100imu;

public interface coord extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "vn100imu/coord";
  static final java.lang.String _DEFINITION = "std_msgs/Header           header\ngeometry_msgs/Point       point\n";
  std_msgs.Header getHeader();
  void setHeader(std_msgs.Header value);
  geometry_msgs.Point getPoint();
  void setPoint(geometry_msgs.Point value);
}
