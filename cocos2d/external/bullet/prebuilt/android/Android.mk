LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := ext_bullet
LOCAL_MODULE_FILENAME := bullet
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libLinearMath.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../include/bullet
LOCAL_STATIC_LIBRARIES += bulletc
LOCAL_STATIC_LIBRARIES += bulletd
LOCAL_STATIC_LIBRARIES += bulletm
LOCAL_STATIC_LIBRARIES += minicl
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := bulletc
LOCAL_MODULE_FILENAME := bulletcollision
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libBulletCollision.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_STATIC_LIBRARIES += linearmath
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := bulletd
LOCAL_MODULE_FILENAME := bulletdynamics
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libBulletDynamics.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_STATIC_LIBRARIES += bulletc
LOCAL_STATIC_LIBRARIES += linearmath
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := bulletm
LOCAL_MODULE_FILENAME := bulletmultithreaded
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libBulletMultiThreaded.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := linearmath
LOCAL_MODULE_FILENAME := linearmath
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libLinearMath.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := minicl
LOCAL_MODULE_FILENAME := minicl
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libMiniCL.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../include
include $(PREBUILT_STATIC_LIBRARY)
