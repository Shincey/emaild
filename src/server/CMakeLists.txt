
cmake_minimum_required(VERSION 3.0)

SET(server
    "${PROJECT_SOURCE_DIR}/src/server/core.h"
    "${PROJECT_SOURCE_DIR}/src/server/core.cpp"
)

ADD_LIBRARY(LIB_SERVER STATIC ${server})