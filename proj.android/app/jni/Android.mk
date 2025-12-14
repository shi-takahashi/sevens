LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   CocosHelper.cpp \
                   ../../../Classes/AI1.cpp \
                   ../../../Classes/AI2.cpp \
                   ../../../Classes/AI3.cpp \
                   ../../../Classes/AIBase.cpp \
                   ../../../Classes/AppDelegate.cpp \
                   ../../../Classes/Card.cpp \
                   ../../../Classes/Character.cpp \
                   ../../../Classes/Common.cpp \
                   ../../../Classes/GameScene.cpp \
                   ../../../Classes/Hands.cpp \
                   ../../../Classes/Manager.cpp \
                   ../../../Classes/NameEdit.cpp \
                   ../../../Classes/NativeCodeLauncher.cpp \
                   ../../../Classes/NetRanking.cpp \
                   ../../../Classes/NonPlayerCharacter.cpp \
                   ../../../Classes/OptionDialog.cpp \
                   ../../../Classes/PlayerCharacter.cpp \
                   ../../../Classes/RankingDialog.cpp \
                   ../../../Classes/ReportDialog.cpp \
                   ../../../Classes/TitleScene.cpp \
                   ../../../Classes/UIDialog.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cc_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
