#cmake file for project emaild
#author:Shincey
#email:shinceytao@gmail
#create:2020-06-24

cmake_minimum_required(VERSION 3.0.0)
project(emaild VERSION 0.1.0)

SET(CMAKE_CXX_COMPILER "g++")
SET(CMAKE_CXX_FLAGS "-g -fuse-ld=gold -std=c++17 -Wall -lpthread -rdynamic -ldl")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -use-ld=gold -std=c++17 -lpthread -rdynamic -ldl -DDEBUG_MODE")
SET(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "-g -o2 -fuse-ld=gold -std=c++17 -lpthread -rdynamic -ldl -DDEBUG_MODE")
SET(CMAKE_CXX_FLAGS_RELEASE "-g -o3 -fuse-ld=gold -std=c++17 -lpthread -rdynamic -ldl")

SET(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

#设置可执行二进制文件目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置编译的库文件目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#设置链接目录
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
#设置头文件目录
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/public"
                    "${PROJECT_SOURCE_DIR}/src/interface"
                    "${PROJECT_SOURCE_DIR}/src/server"
                    "${PROJECT_SOURCE_DIR}/src/server/core"
                    "${PROJECT_SOURCE_DIR}/src/server/logic"
                    "${PROJECT_SOURCE_DIR}/src/server/log"
                    "${PROJECT_SOURCE_DIR}/src/server/udp"
                    "${PROJECT_SOURCE_DIR}/src/server/tcp"
)

ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/public)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/server)

AUX_SOURCE_DIRECTORY(. MAIN)


#include(CTest)
#enable_testing()

#add_definitions(" -Wall -D DEBUG_MODE -std=c++17")

add_executable(emaild ${MAIN})

#指明可执行文件需要链接的库
TARGET_LINK_LIBRARIES(emaild LIB_PUBLIC LIB_SERVER)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)




