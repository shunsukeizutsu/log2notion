cmake_minimum_required(VERSION 3.13)
project(test_notion)

# Find libcurl
find_package(CURL REQUIRED)

include_directories(./src ./include ./etc ~/creatFig/src)

# Add the executable
add_executable(log2notion src/log2notion.cpp)
# Link libcurl
target_link_libraries(log2notion CURL::libcurl)

add_executable(auto_plotgnss src/auto_plotgnss.cpp ~/creatFig/src/creatfig.cpp)
