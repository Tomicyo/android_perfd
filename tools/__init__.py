import os

if 'ANDROID_HOME' not in os.environ:
    print 'ANDROID_HOME not found!!!'
    
if 'ANDROID_NDK_HOME' not in os.environ:
    print 'ANDROID_NDK_HOME not found!!!'

android_home = os.environ['ANDROID_HOME']
android_ndk_home = os.environ['ANDROID_NDK_HOME']
