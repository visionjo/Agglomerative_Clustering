cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 11)
project( Agglomerative )


find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

FILE(GLOB folder_source *.cpp)
FILE(GLOB folder_header *.h)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

# create the project
ADD_EXECUTABLE(Agglomerative ${folder_source} ${folder_header})


 
# add include directories
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${OPENCV_INCLUDE_DIR})
   
# link opencv libraries 
TARGET_LINK_LIBRARIES(Agglomerative ${OpenCV_LIBS})