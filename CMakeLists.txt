cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_VERBOSE_MAKEFILE true)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_GENERATOR_PLATFORM x64)
endif()

project(imcam VERSION 0.1.0 LANGUAGES C CXX)


set(dep_libs)

### glfw ###
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)
set_target_properties(glfw PROPERTIES CONFIG Release)
target_compile_definitions(glfw INTERFACE GLFW_INCLUDE_NONE) # rid glad include
set_target_properties(glfw PROPERTIES FOLDER vendor)
list(APPEND dep_libs ${lib_name})



### glad ### C
set(lib_name glad)
if(WIN32)
    set(lib_dir ${CMAKE_SOURCE_DIR}/vendor/glad-4.6-core)
else()
    set(lib_dir ${CMAKE_SOURCE_DIR}/vendor/glad-4.1-core)
endif()
file(GLOB_RECURSE lib_files ${lib_dir}/*.*)
add_library(${lib_name} STATIC ${lib_files})
source_group(TREE ${lib_dir} FILES ${lib_files})
target_include_directories(${lib_name} PUBLIC ${lib_dir}/include)
set_target_properties(${lib_name} PROPERTIES FOLDER vendor)
list(APPEND dep_libs ${lib_name})



### stb ### C
set(lib_name stb)
set(lib_dir ${CMAKE_SOURCE_DIR}/vendor/stb)
file(GLOB_RECURSE lib_files ${lib_dir}/*.c ${lib_dir}/*.h)
add_library(${lib_name} STATIC ${lib_files})
source_group(TREE ${lib_dir} FILES ${lib_files})
target_include_directories(${lib_name} PUBLIC ${lib_dir})
set_target_properties(${lib_name} PROPERTIES FOLDER vendor)
list(APPEND dep_libs ${lib_name})



### imgui ###
set(lib_name imgui)
set(lib_dir ${CMAKE_SOURCE_DIR}/vendor/imgui-docking)
file(GLOB_RECURSE lib_files ${lib_dir}/*.h ${lib_dir}/*.cpp)
list(REMOVE_ITEM lib_files ${lib_dir}/example_glfw_opengl3/main.cpp)
list(REMOVE_ITEM lib_files ${lib_dir}/misc/freetype/imgui_freetype.h)
list(REMOVE_ITEM lib_files ${lib_dir}/misc/freetype/imgui_freetype.cpp)
list(REMOVE_ITEM lib_files ${lib_dir}/misc/fonts/binary_to_compressed_c.cpp)
add_library(${lib_name} STATIC ${lib_files})
source_group(TREE ${lib_dir} FILES ${lib_files})
target_include_directories(${lib_name} PUBLIC ${lib_dir})
target_link_libraries(${lib_name} PUBLIC glfw stb)
add_dependencies(${lib_name} glfw stb)
target_compile_definitions(${lib_name}
    PRIVATE
        IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
        IMGUI_STB_TRUETYPE_FILENAME="../stb/stb_truetype.h"
        IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
        IMGUI_STB_RECT_PACK_FILENAME="../stb/stb_rect_pack.h"
)
set_target_properties(${lib_name} PROPERTIES FOLDER vendor)
list(APPEND dep_libs ${lib_name})



### glm (header only) ###
set(lib_name glm)
set(lib_dir ${CMAKE_SOURCE_DIR}/vendor/glm)
file(GLOB_RECURSE lib_files ${lib_dir}/*.*)
add_library(${lib_name} INTERFACE ${lib_files})
source_group(TREE ${lib_dir}/glm FILES ${lib_files})
target_include_directories(${lib_name} INTERFACE ${lib_dir})
target_compile_definitions(${lib_name} 
    INTERFACE
        GLM_FORCE_SILENT_WARNINGS
        GLM_ENABLE_EXPERIMENTAL # for transform and norm
)
set_target_properties(${lib_name} PROPERTIES FOLDER vendor)
list(APPEND dep_libs ${lib_name})



### opencv ###
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
list(APPEND dep_libs ${lib_name})



### limbrary ###
set(lib_name limbrary)
set(lib_dir ${CMAKE_SOURCE_DIR}/limbrary)
file(GLOB_RECURSE lib_files CONFIGURE_DEPENDS ${lib_dir}/*.*)
add_library(${lib_name} STATIC ${lib_files})
source_group(TREE ${lib_dir} FILES ${lib_files})
target_include_directories(${lib_name} PUBLIC ${lib_dir}/include)
target_link_libraries(${lib_name} PUBLIC ${dep_libs})
add_dependencies(${lib_name} ${dep_libs})
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_definitions(${lib_name}
        PUBLIC
            _SCL_SECURE_NO_WARNINGS
            _CRT_SECURE_NO_WARNINGS
    )
    target_compile_options(${lib_name} 
        PUBLIC
            /MD$<$<CONFIG:Debug>:d>
            /utf-8
        PRIVATE
            /W4
            /wd4100
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_definitions(${lib_name} 
        PUBLIC
            GL_SILENCE_DEPRECATION
    )
    target_compile_options(${lib_name} 
        PRIVATE
            -Wall
    )
endif()


### imcam_exe ###
set(exe_name imcam_exe)
add_executable(${exe_name} main/main.cpp)
target_link_libraries(${exe_name} PRIVATE limbrary)

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