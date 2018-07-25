# Android.mk，其它 linux 系统请自己写 Makefile
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -O2 -W -Wall

LOCAL_C_INCLUDES:=  \
        $(LOCAL_PATH)/include

LOCAL_SRC_FILES := \
                i2c_tool.c \
                i2c_api.c

LOCAL_MODULE := i2ctool

LOCAL_STATIC_LIBRARIES := libc

LOCAL_FORCE_STATIC_EXECUTABLE := true

include $(BUILD_EXECUTABLE)
