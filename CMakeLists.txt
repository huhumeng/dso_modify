cmake_minimum_required(VERSION 3.4)
project(dso_modi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(EXECUTABLE_OUTPUT_PATH      ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH         ${PROJECT_SOURCE_DIR}/lib)

include_directories(include)

include_directories(/usr/include/eigen3)

find_package(OpenCV 3.3.1 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Sophus REQUIRED)
include_directories(${Sophus_INCLUDE_DIRECTORIES})

add_library(${PROJECT_NAME} SHARED
    src/global_calib.cc
    src/minimal_image.cc
    src/dataset_reader.cc
    src/frame_shell.cc
    src/system.cc
)
target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
    ${Sophus_LIBRARIES}
)

add_executable(ex_zed example/ex_zed.cc)
target_link_libraries(ex_zed
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
    ${Sophus_LIBRARIES}
)

