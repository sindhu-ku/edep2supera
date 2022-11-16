import ROOT,os

# Load supera library by hand as it's not bound to python through PyROOT
import supera
ROOT.gSystem.Load(os.path.join(supera.get_lib_dir(),'libsupera.so'))

# Now load edep2supera library
lib_path = os.path.join(os.path.dirname(__file__),"../../../")
inc_path = os.path.join(os.path.dirname(__file__),"../../../../include/")
ROOT.gSystem.Load(os.path.join(lib_path,'libedep2supera.so'))

from ROOT import edep2supera
from ROOT import supera
supera.EDep

def get_includes():
    return inc_path

def get_lib_dir():
    return lib_path
