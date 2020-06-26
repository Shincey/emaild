
cmake_minimum_required(VERSION 3.0)

SET(server
    "${PROJECT_SOURCE_DIR}/src/server/core/core.h"
    "${PROJECT_SOURCE_DIR}/src/server/core/core.cpp"
    "${PROJECT_SOURCE_DIR}/src/server/logic/logic.h"
    "${PROJECT_SOURCE_DIR}/src/server/logic/logic.cpp"
    "${PROJECT_SOURCE_DIR}/src/server/log/logger.h"
    "${PROJECT_SOURCE_DIR}/src/server/log/logger.cpp"
    "${PROJECT_SOURCE_DIR}/src/server/log/logfile.h"
    "${PROJECT_SOURCE_DIR}/src/server/log/logfile.cpp"
)

ADD_LIBRARY(LIB_SERVER STATIC ${server})