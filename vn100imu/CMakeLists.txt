cmake_minimum_required(VERSION 2.8.3)
project(vn100imu)

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
  qt_build
)

## System dependencies are found with CMake's conventions
find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

################################################
## Find files 
################################################
file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
			   ui/tuning_ui.ui
			   ui/setparameters.ui)
set(QT_MOC include/tuning_ui.h 
	   include/setparameters.h
	   include/graphicsviewmouse.h)
SET(QT_SOURCES src/tuning_ui_main.cpp
	       src/tuning_ui.cpp
	       src/graphicsviewmouse.cpp
	       src/setparameters.cpp)

################################################
## Setup QT
################################################
QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})


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
    ${QT_INCLUDE_DIRS}
)

include_directories(vnccpplib/include)
add_library(vn100
    vnccpplib/src/vn100.c
)
add_library(vncp_services
 vnccpplib/src/arch/linux/vncp_services.c
)

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

add_executable(acceldcm src/ins_dcm.cpp)
target_link_libraries(acceldcm ${catkin_LIBRARIES})

add_library(uituninglib ${QT_SOURCES} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(uituninglib ${QT_LIBRARIES} ${catkin_LIBRARIES})

add_executable(uituning src/tuning_ui_main.cpp)
target_link_libraries(uituning uituninglib)


