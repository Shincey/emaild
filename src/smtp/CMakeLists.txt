
cmake_minimum_required(VERSION 3.0.0)

set(smtp
    "${PROJECT_SOURCE_DIR}/src/smtp/types/mime.h"
    "${PROJECT_SOURCE_DIR}/src/smtp/types/mime.cpp"
    "${PROJECT_SOURCE_DIR}/src/smtp/types/command.h"
    "${PROJECT_SOURCE_DIR}/src/smtp/types/command.cpp"
    "${PROJECT_SOURCE_DIR}/src/smtp/types/email.h"
    "${PROJECT_SOURCE_DIR}/src/smtp/types/email.cpp"
    "${PROJECT_SOURCE_DIR}/src/smtp/types/smtp_response.h"
    "${PROJECT_SOURCE_DIR}/src/smtp/types/smtp_response.cpp"
    "${PROJECT_SOURCE_DIR}/src/smtp/types/setting.h"
    "${PROJECT_SOURCE_DIR}/src/smtp/server/smtp_server.h"
    "${PROJECT_SOURCE_DIR}/src/smtp/server/smtp_server.cpp"
)

ADD_LIBRARY(LIB_SMTP STATIC ${smtp})