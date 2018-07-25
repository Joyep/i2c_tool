LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE := i2ctool

#LOCAL_CFLAGS := -O2 -W -Wall

LOCAL_C_INCLUDES:=  \
        ./include

LOCAL_SRC_FILES := \
                src/i2c_tool.c \
                src/i2c_api.c


LOCAL_CFLAGS := -fPIC
LOCAL_LDLIBS := -lc

#LOCAL_STATIC_LIBRARIES := libc

#LOCAL_FORCE_STATIC_EXECUTABLE := true

include $(BUILD_EXECUTABLE)
