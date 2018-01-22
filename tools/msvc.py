import os, sys, subprocess

__archmapping__ = {
    'x64': 'amd64',
    'x86': 'x86',
}

def get_vs2017_env(arch):
    vs150common = os.environ['VS150COMNTOOLS']
    vsvarbat = os.path.join(vs150common, '..', '..', 'VC/Auxiliary/Build/vcvarsall.bat')
    process = subprocess.Popen('("%s" %s>nul)&&"%s" -c "import os; print repr(os.environ)"' % (vsvarbat, __archmapping__[arch], sys.executable), stdout=subprocess.PIPE, shell=True)
    stdout, _ = process.communicate()
    exitcode = process.wait()
    return eval(stdout.strip())

def enable_vs2017_env(arch = 'x64'):
    envs = get_vs2017_env(arch)
    for (k, v) in envs.items():
        os.environ[k] = v