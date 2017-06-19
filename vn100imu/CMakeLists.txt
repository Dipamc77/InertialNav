cmake_minimum_required(VERSION 2.8.3)
project(vn100imu)

## Add support for C++11, supported in ROS Kinetic and newer
 add_definitions(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  rospy
  std_msgs
  message_generation
  genmsg
  cv_bridge
  image_transport
)


## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
 add_message_files(
   FILES
   ypr_data.msg
   xyz_data.msg
   coord.msg
 )

# Generate services in the 'srv' folder
 add_service_files(
   FILES
   YPR.srv
 )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

# Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
   geometry_msgs
 )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  #INCLUDE_DIRS include
  LIBRARIES vn100imu
  CATKIN_DEPENDS roscpp rospy std_msgs geometry_msgs
  DEPENDS system_lib
 CATKIN_DEPENDS message_runtime
)


###########
## Build ##
###########

include_directories(include)
include_directories(${catkin_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

include_directories(vnccpplib/include)
add_library(vn100
    vnccpplib/src/vn100.c
)
add_library(vncp_services
 vnccpplib/src/arch/linux/vncp_services.c
)

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/vn100imu.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/vn100imu_node.cpp)


add_executable(rawvn100Data  src/vn100node.cpp
                           vnccpplib/src/arch/linux/vncp_services.c
                           vnccpplib/src/vn100.c
)
target_link_libraries(vncp_services
pthread
)
target_link_libraries(rawvn100Data
${catkin_LIBRARIES}
vncp_services
vn100
)

add_executable(veldata src/velcalc.cpp)
target_link_libraries(veldata ${catkin_LIBRARIES})

add_executable(accfilt src/acc_filt_tf.cpp)
target_link_libraries(accfilt ${catkin_LIBRARIES})

add_executable(camtrackoverhead src/overheadtrack.cpp)
target_link_libraries(camtrackoverhead ${catkin_LIBRARIES})

add_executable(dataplotter2d src/plotting2d.cpp)
target_link_libraries(dataplotter2d ${catkin_LIBRARIES})

add_executable(plotoverlay src/overlay.cpp)
target_link_libraries(plotoverlay ${catkin_LIBRARIES})




