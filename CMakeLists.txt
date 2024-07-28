cmake_minimum_required(VERSION 3.13)
project(test_notion)

# Find libcurl
find_package(CURL REQUIRED)

include_directories(./src ./include ./etc ~/creatFig/src)


add_executable(auto_plotgnss src/auto_plotgnss.cpp ~/creatFig/src/creatfig.cpp)

#add_executable(readname etc/readname.cpp ~/creatFig/src/creatfig.cpp)

add_executable(log2notion_v2 src/log2notion_v2.cpp ~/creatFig/src/creatfig.cpp)
target_link_libraries(log2notion_v2 CURL::libcurl)
