# imtex
Image retrieval using visual and textual features

## Image features
1. Install OpenCV 3.0 from source code
2. Prepare CMakeLists.txt
    cmake_minimum_required(VERSION 2.8)
    project( YourPorjectName )
    find_package( OpenCV REQUIRED )
    add_executable( YourPorjectName YourPorjectName.cc )
    target_link_libraries( YourPorjectName ${OpenCV_LIBS} )

## Log
1. 01/29/2015 Install OpenCV;