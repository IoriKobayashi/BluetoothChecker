LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ext_recast

LOCAL_MODULE_FILENAME := librecast

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
DebugUtils/DebugDraw.cpp \
DebugUtils/DetourDebugDraw.cpp \
Detour/DetourAlloc.cpp \
Detour/DetourCommon.cpp \
Detour/DetourNavMesh.cpp \
Detour/DetourNavMeshBuilder.cpp \
Detour/DetourNavMeshQuery.cpp \
Detour/DetourNode.cpp \
DetourCrowd/DetourCrowd.cpp \
DetourCrowd/DetourLocalBoundary.cpp \
DetourCrowd/DetourObstacleAvoidance.cpp \
DetourCrowd/DetourPathCorridor.cpp \
DetourCrowd/DetourPathQueue.cpp \
DetourCrowd/DetourProximityGrid.cpp \
DetourTileCache/DetourTileCache.cpp \
DetourTileCache/DetourTileCacheBuilder.cpp \
fastlz/fastlz.c

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..
                                 
include $(BUILD_STATIC_LIBRARY)