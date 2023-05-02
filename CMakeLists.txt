cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)
project(web_getter)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
add_executable(web_getter main.cpp)

target_compile_options(web_getter PUBLIC "-g")
target_include_directories(web_getter PUBLIC 
  ~/Documents/LFP/boost_1_82_0/
  /usr/include/openssl/
  ./include/)
target_link_libraries(web_getter ssl crypto)
