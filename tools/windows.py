import os, sys, platform, subprocess, shutil, zipfile
from tools.common import zipdir, copy_files_by_ext, copy_files
from tools.download import download_and_extract

lib_posix_mapping = {
    'debug': 'win64_vc150d',
    'release': 'win64_vc150r'
} 

config_mapping = {
    'debug': 'Debug',
    'release': 'Release'
}

def build_x64(config, src, artifact_dir):
    #src = os.getcwd()
    bld = os.path.join(src, 'build', 'win64', config)
    ins = os.path.join(src, 'artifacts', 'win64_' + config)
    
    P = subprocess.Popen(['cmake', 
        '-GVisual Studio 15 2017 Win64',
        '-H{0}'.format(src),
        '-B{0}'.format(bld),
        '-DCMAKE_BUILD_TYPE={0}'.format(config_mapping[config]),
        '-DCMAKE_INSTALL_PREFIX={0}'.format(ins),
        '-Dgtest_force_shared_crt=ON'
        ])
    P.wait()

    B = subprocess.Popen(['cmake',
        '--build', bld,
        '--config', config_mapping[config],
        '--target', 'install'
        ])
    B.wait()
    
    #artifact_dir = os.path.join(src, 'artifacts', 'win64')
    artifact_incdir = os.path.join(artifact_dir, 'include')
    artifact_libdir = os.path.join(artifact_dir, 'lib', lib_posix_mapping[config])
    artifact_bindir = os.path.join(artifact_dir, 'bin', lib_posix_mapping[config])
    #if not os.path.exists(artifact_incdir):
    copy_files(os.path.join(ins, 'include'), artifact_incdir)
    copy_files_by_ext(os.path.join(ins, 'lib'), '.lib', artifact_libdir)
    copy_files_by_ext(os.path.join(ins, 'lib'), '.pdb', artifact_libdir)
    copy_files_by_ext(os.path.join(ins, 'bin'), '.dll', artifact_bindir)

def build_win64_instance(need_v8=False, need_openssl=False):
    src = os.getcwd()
    artifact_dir = os.path.join(src, 'artifacts', 'win64')
    if need_openssl:
        download_and_extract('https://ci.appveyor.com/api/projects/tomicyo/openssl-ci/artifacts/artifacts/openssl_windows_md_shared.zip', artifact_dir)
    build_x64('debug', src, artifact_dir)
    build_x64('release', src, artifact_dir)
    if need_v8:
        download_and_extract('https://ci.appveyor.com/api/projects/tomicyo/v8-ci/artifacts/v8/v8.zip', artifact_dir)
    copy_files_by_ext(os.path.join(src, 'tools'), '.cmake', os.path.join(artifact_dir, 'lib', 'cmake'))
    archive_name = os.path.join(os.getcwd(), 'artifacts', 'third_party_windows.zip')
    zipf = zipfile.ZipFile(archive_name, 'w', zipfile.ZIP_DEFLATED)
    zipdir(os.path.join(os.getcwd(), 'artifacts', 'win64'), zipf)
    zipf.close()