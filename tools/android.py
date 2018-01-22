import os, sys, platform, subprocess, shutil, zipfile
from tools import *
from tools.common import zipdir, copy_files_by_ext, copy_files

def find_cmake_dir():
    cmake_dir = os.path.join(android_home, 'cmake')
    if os.path.exists(cmake_dir):
        cmake_ver_dirs = os.listdir(cmake_dir)
        if len(cmake_ver_dirs) > 0:
            cmake_version = cmake_ver_dirs[0]
            cmake_dir = os.path.join(cmake_dir, cmake_version)
            return cmake_dir
        else:
            print 'didn\'t find android cmake, need to download cmake from android studio'
            return ''
    else:
        print 'didn\'t find android cmake, need to download cmake from android studio'
        return ''

def find_ndk_toolchain_file():
    android_ndk_toolchain = os.path.join(android_ndk_home, 'build', 'cmake', 'android.toolchain.cmake')
    if os.path.exists(android_ndk_toolchain):
        print 'ndk toolchain found'
    return android_ndk_toolchain

def find_cmake_exe():
    cmake_exe = os.path.join(find_cmake_dir(), 'bin', 'cmake.exe' if platform.system().lower() == 'windows' else 'cmake')
    if os.path.exists(cmake_exe):
        return cmake_exe
    print 'cmake execute not found !!!'
    return ''

def find_cmake_ninja():
    cmake_ninja = os.path.join(find_cmake_dir(), 'bin', 'ninja.exe' if platform.system().lower() == 'windows' else 'ninja')
    if os.path.exists(cmake_ninja):
        print 'ninja found'
    return cmake_ninja

def build(config, abi, stl, sdk_platform, ndk_level, src, artifact_dir, additional_defs=[]):
    print 'Build Config is {0}, ABI is {1}, STL is {2}'.format(config, abi, stl)
    bld = os.path.join(src, 'build', 'android', '_'.join([config, abi, stl]))
    ins = os.path.join(src, 'artifacts', '_'.join(['android',config, abi, stl]))
    
    config_mapping = {
        'debug': 'Debug',
        'release': 'Release'
    }
    cmake_cmd = [find_cmake_exe(), 
        '-GAndroid Gradle - Ninja',
        '-H{0}'.format(src),
        '-B{0}'.format(bld),
        '-DCMAKE_MAKE_PROGRAM={0}'.format(find_cmake_ninja()),
        '-DANDROID_NDK={0}'.format(android_ndk_home),
        '-DANDROID_TOOLCHAIN=clang',
        '-DCMAKE_TOOLCHAIN_FILE={0}'.format(find_ndk_toolchain_file()),
        '-DANDROID_PLATFORM=android-{0}'.format(sdk_platform),
        '-DANDROID_NATIVE_API_LEVEL={0}'.format(ndk_level),
        '-DANDROID_ABI={0}'.format(abi),
        '-DANDROID_STL={0}'.format(stl),
        '-DANDROID_CPP_FEATURES=rtti;exceptions',
        '-DCMAKE_BUILD_TYPE={0}'.format(config_mapping[config]),
        '-DCMAKE_INSTALL_PREFIX={0}'.format(ins)
        ]
    cmake_cmd.extend(additional_defs)
    P = subprocess.Popen(cmake_cmd)
    P.wait()

    B = subprocess.Popen([find_cmake_exe(),
        '--build', bld,
        '--config', config_mapping[config],
        '--target', 'perfd'
        ])
    B.wait()

#    artifact_dir = os.path.join(src, 'artifacts', '_'.join([abi,stl]))
    artifact_incdir = os.path.join(artifact_dir, 'include')
    artifact_libdir = os.path.join(artifact_dir, 'lib', '_'.join([abi,stl,config]))
    copy_files(os.path.join(ins, 'include'), artifact_incdir)
    copy_files_by_ext(os.path.join(ins, 'lib'), '.a', artifact_libdir)
    
def build_android_instance(abi, stl, additional_defs=[]):
    src = os.getcwd()
    artifact_dir = os.path.join(src, 'artifacts', '_'.join([abi,stl]))
    build('release', abi, stl, 21, 21, src, artifact_dir, additional_defs)
    #build('debug', abi, stl, 19, 19, src, artifact_dir, additional_defs)
    copy_files_by_ext(os.path.join(src, 'tools'), '.cmake', os.path.join(artifact_dir, 'lib', 'cmake'))
    archive_name = os.path.join(os.getcwd(), 'artifacts', 'third_party_android_' + '_'.join([abi, stl]) +'.zip')
    zipf = zipfile.ZipFile(archive_name, 'w', zipfile.ZIP_DEFLATED)
    zipdir(os.path.join(os.getcwd(), 'artifacts', '_'.join([abi, stl])), zipf)
    zipf.close()