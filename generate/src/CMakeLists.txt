#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目
set(CMAKE_SHARED_LIBRARY_SUFFIX ".passoa")
SET(CMAKE_SHARED_LIBRARY_PREFIX "")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,–rpath=.")
add_library(fb SHARED
${PROJECT_SOURCE_DIR}/src/fb.cpp
${PROJECT_SOURCE_DIR}/src/fbdev.cpp
)

install(TARGETS fb LIBRARY DESTINATION node_modules/fb)
