package vn100imu;

public interface xyz_data extends org.ros.internal.message.Message {
  static final java.lang.String _TYPE = "vn100imu/xyz_data";
  static final java.lang.String _DEFINITION = "std_msgs/Header           header\ngeometry_msgs/Vector3     XYZ\nfloat32          Period\n";
  std_msgs.Header getHeader();
  void setHeader(std_msgs.Header value);
  geometry_msgs.Vector3 getXYZ();
  void setXYZ(geometry_msgs.Vector3 value);
  float getPeriod();
  void setPeriod(float value);
}
