from tools.msvc import enable_vs2017_env
from tools.android import build_android_instance
from tools.common import zipdir
import os, subprocess, sys, platform, zipfile

curdir = os.path.abspath(os.path.dirname(__file__))
print(curdir)

lib_types = ['static', 'shared']
archs = ['x64', 'armv7', 'arm64']

os_name_l = platform.system().lower()

# lib_type could be 'static' or 'shared'
def build_install_dir(lib_type = 'shared'):
    return os.path.join(curdir, 'build', os_name_l, lib_type)

def build_cmake_vs2017(lib_type = 'shared'):
    build_dir = os.path.join(curdir, 'build', 'cmake')
    install_dir = build_install_dir(lib_type)
    openssl_inc = os.path.join(install_dir, 'include')
    openssl_libdir = os.path.join(install_dir, 'lib')
    print(openssl_inc)
    print(openssl_libdir)
    cmake_process = subprocess.Popen([
        'cmake',
        '-GVisual Studio 15 2017 Win64',
        '-H{0}'.format(curdir),
        '-B{0}'.format(build_dir),
        '-DOPENSSL_INC={0}'.format(openssl_inc.replace('\\', '/')),
        '-DOPENSSL_LIBDIR={0}'.format(openssl_libdir.replace('\\', '/')),
        '-DCMAKE_INSTALL_PREFIX={0}'.format(install_dir)
    ],shell=True, stdout = subprocess.PIPE)
    stdout, stderr = cmake_process.communicate()
    print(stdout)
    cmake_process = subprocess.Popen([
        'cmake',
        '--build',
        build_dir,
        '--config',
        'RelWithDebInfo',
        '--target',
        'install'
    ],shell=True, stdout = subprocess.PIPE)
    stdout, stderr = cmake_process.communicate()
    print(stdout)


def build_windows(lib_type = 'shared'):
    build_cmake_vs2017(lib_type)
    
    archive_name = os.path.join(curdir, 'build', 'third_party_clibs_windows.zip')
    zipf = zipfile.ZipFile(archive_name, 'w', zipfile.ZIP_DEFLATED)
    zipdir(os.path.join(curdir, 'build', 'windows', lib_type), zipf)
    zipf.close()

if os_name_l == 'windows':
    #build_windows()
    build_android_instance('armeabi-v7a', 'c++_static', [])
elif os_name_l == 'linux':
    build_android_instance('arm64-v8a', 'gnustl_shared', [])