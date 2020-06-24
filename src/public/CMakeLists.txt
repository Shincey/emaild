
cmake_minimum_required(VERSION 3.0)

SET(public
    "${PROJECT_SOURCE_DIR}/src/public/zsystem.h"
    "${PROJECT_SOURCE_DIR}/src/public/zsystem.cpp"
    "${PROJECT_SOURCE_DIR}/src/public/ztools.h"
    "${PROJECT_SOURCE_DIR}/src/public/ztools.cpp"
    "${PROJECT_SOURCE_DIR}/src/public/zthread.h"
    "${PROJECT_SOURCE_DIR}/src/public/zthread.cpp"
    "${PROJECT_SOURCE_DIR}/src/public/zbase64.h"
    "${PROJECT_SOURCE_DIR}/src/public/zbase64.cpp"
    "${PROJECT_SOURCE_DIR}/src/public/zbuffer.h"
    "${PROJECT_SOURCE_DIR}/src/public/zpool.h"
)

ADD_LIBRARY(LIB_PUBLIC STATIC ${public})