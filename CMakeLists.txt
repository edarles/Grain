cmake_minimum_required(VERSION 2.8.3)
project(Grains)

#export CXX=/opt/local/bin/x86_64-apple-darwin16-g++-mp-4.9
#export CC=/opt/local/bin/x86_64-apple-darwin16-gcc-4.9.4

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++14 -O3") 
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3") 
SET(CMAKE_CC_FLAGS "${CMAKE_CC_FLAGS} -std=c++14 -O3")

SET(CMAKE_VERBOSE_MAKEFILE OFF) 
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/Modules")
message("cmake module path: " ${CMAKE_MODULE_PATH})

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(OpenCL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(Eigen3 REQUIRED)
FIND_PACKAGE(QGLViewer REQUIRED)
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5OpenGL)
find_package(Qt5Xml)
add_definitions(${Qt5Widgets_DEFINITIONS})

OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
  FIND_PACKAGE(OpenMP)
  SET(OpenMP_C_FLAGS "${OpenMP_C_FLAGS} -fopenmp")
  SET(OpenMP_CC_FLAGS "${OpenMP_CC_FLAGS} -fopenmp")
  SET(OpenMP_CXX_FLAGS "${OpenMP_CXX_FLAGS} -fopenmp")
  SET(OpenMP_EXE_FLAGS "${OpenMP_EXE_LINKER_FLAGS} -fopenmp")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_C_FLAGS "${CMAKE_CC_FLAGS} ${OpenMP_CC_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ENDIF()

include_directories(${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${QGLVIEWER_INCLUDE_DIR})
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Xml_INCLUDE_DIRS})
include_directories(${OpenCL_INCLUDE_DIRS})

include(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt)

ADD_EXECUTABLE(Grains ${APP_SOURCES})

TARGET_LINK_LIBRARIES(Grains -lstdc++ "-fopenmp" -L/opt/local/lib -lopencv_core 
-lopencv_imgproc -lopencv_highgui 
-lopencv_calib3d ${OpenCV2_LIBRARIES} ${OpenCL_LIBRARIES} ${QGLVIEWER_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARY} ${GLUT_glut_LIBRARY} ${EIGEN3_LIBRARY}
	${Qt5Widgets_LIBRARIES} ${Qt5Xml_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5OpenGL_LIBRARIES})
