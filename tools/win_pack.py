import os, shutil, zipfile, platform

def zipdir(path, ziph):
    for root, dirs, files in os.walk(path):
        for file in files:
            ziph.write(os.path.join(root, file))

shutil.copytree('Debug/include', 'artifacts/include/')
if platform.system() == 'Windows':
    shutil.copytree('Debug/lib', 'artifacts/lib/win64_vc150d/')
    shutil.copytree('Release/lib', 'artifacts/lib/win64_vc150r/')
elif platform.system() == 'Linux':
    shutil.copytree('Debug/lib', 'artifacts/lib/linux64d/')
    shutil.copytree('Release/lib', 'artifacts/lib/linux64r/')
    os.chdir('artifacts/')
    zipf = zipfile.ZipFile('third_party_linux.zip', 'w', zipfile.ZIP_DEFLATED)
    zipdir('include/', zipf)
    zipdir('lib/', zipf)
    zipf.close()