TARGET := $(strip $(TARGET))
TYPE := $(strip $(TYPE))
INSTALL_DIR := $(strip $(INSTALL_DIR))
CC = gcc
CXX = g++
CPP = cpp
CFLAGS   += $(C_INC)
CXXFLAGS += $(C_INC)
CFLAGS   += $(EXTRA_CFLAGS)
CXXFLAGS += $(EXTRA_CXXFLAGS)

CFLAGS   += -Wall
CXXFLAGS += -Wall
EXTRA_CXXFLAGS += -std=c++11

SRC_DIR  += .
OBJ_DIR  ?= obj/
INC_DIR  += $(COMM_INC_DIR)
INC_DIR  += /usr/include/mysql/ /usr/include/ ../ ./ ../../
SYS_LIB  += libpthread libcurl libmysqlclient # and more needed
LIB_DIR  += $(COMM_LIB_DIR)
LIB_DIR  += /usr/lib/ /usr/lib64/ /usr/lib64/mysql/
C_INC    = $(INC_DIR:%=-I%)
C_LIB    = $(LIB_DIR:%=-L%) $(LIB_DEPENDS:lib%=-l%) $(SYS_LIB:lib%=-l%)