cmake_minimum_required(VERSION 3.10.0)
project(imcam VERSION 0.1.0 LANGUAGES C CXX)



set(lib_name opencv)
add_library(${lib_name} INTERFACE)
if(WIN32)
    # you must set under path & dll path to env path var
    set(opencv_path D:/library/opencv/build)
    target_include_directories(${lib_name} INTERFACE ${opencv_path}/include)
    target_link_libraries(${lib_name} INTERFACE ${opencv_path}/x64/vc16/lib/opencv_world4110$<$<CONFIG:Debug>:d>.lib)
else()
    # brew install opencv
    find_package(OpenCV REQUIRED)
    target_include_directories(${lib_name} INTERFACE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${lib_name} INTERFACE ${OpenCV_LIBS})
endif()



set(exe_name imcam_exe)
add_executable(${exe_name} main/main.cpp)
target_link_libraries(${exe_name} PRIVATE opencv)



set(working_dir ${CMAKE_SOURCE_DIR}/main)
if(CMAKE_GENERATOR MATCHES "Visual Studio*")
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${exe_name})
    set_target_properties(${exe_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${working_dir})
elseif(CMAKE_GENERATOR MATCHES "Xcode")
    set_target_properties(${exe_name} PROPERTIES 
        XCODE_GENERATE_SCHEME TRUE
        XCODE_SCHEME_WORKING_DIRECTORY ${working_dir}
    )
endif()