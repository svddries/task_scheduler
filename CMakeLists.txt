cmake_minimum_required(VERSION 2.8.3)
project(task_scheduler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(Boost REQUIRED COMPONENTS thread system)

include_directories(
    include
    ${Boost_INCLUDE_DIRS}
)

add_library(task_scheduler
    src/blackboard.cpp
    src/data_channel.cpp
    src/process.cpp
    src/scheduler.cpp
    include/task_scheduler/types.h
    include/task_scheduler/blackboard.h
    include/task_scheduler/data_channel.h
    include/task_scheduler/process.h
    include/task_scheduler/scheduler.h
)
target_link_libraries(task_scheduler ${Boost_LIBRARIES})

add_executable(test_task_scheduler test/test_task_scheduler.cpp)
target_link_libraries(test_task_scheduler task_scheduler)

add_executable(foo test/foo.cpp)
target_link_libraries(foo ${Boost_LIBRARIES})
