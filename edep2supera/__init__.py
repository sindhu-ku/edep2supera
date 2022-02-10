import ROOT,os

lib_path = os.path.dirname(__file__) + "/lib/"
inc_path = os.path.dirname(__file__) + "/include/"
ROOT.gSystem.Load(os.path.join(lib_path,'libedep2supera.so'))

from ROOT import simple

def get_includes():
    return inc_path

def get_lib_dir():
    return lib_path
