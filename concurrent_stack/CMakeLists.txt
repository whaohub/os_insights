cmake_minimum_required(VERSION 3.10)
project(StackExamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define executables
add_executable(lockfree_stack lockfree_stack.cpp)
add_executable(simple_stack simple_stack.cpp)
add_executable(threadsafe_stack threadsafe_stack.cpp)

# Link pthread if needed
find_package(Threads REQUIRED)
target_link_libraries(lockfree_stack Threads::Threads)
target_link_libraries(simple_stack Threads::Threads)
target_link_libraries(threadsafe_stack Threads::Threads)