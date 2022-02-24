# edep2supera

C++/Python software to run `Supera` ([repository](https://github.com/DeepLearnPhysics/SuperaAtomic)), a library that can generate true labels for a ML based data reconstruction chain called `lartpc_mlreco3d` ([repository](https://github.com/DeepLearnPhysics/lartpc_mlreco3d)). This repository is dedicated to interface [EDepSim](https://github.com/ClarkMcGrew/edep-sim) and [larcv](https://github.com/DeepLearnPhysics/larcv2) frameworks that input and output information in data files respectively. 

## Python binding
Executable scripts are in Python and C++ classes and functions are bound to Python via [PyROOT](https://root.cern/manual/python/) following a pattern used by EDepSim (as opposed to Supera which uses [pybind11](https://pybind11.readthedocs.io/en/stable/)). This allows both edep2supera and EDepSim methods to access C++ objects within Python in a common manner.

## How to build and install
1. Use [this docker image](https://hub.docker.com/layers/deeplearnphysics/larcv2/ub20.04-cuda11.3-cudnn8-pytorch1.10.0-larndsim/images/sha256-b9a67dfabf5190dbd67745cf739f9aeb6a357a6f4580df4702210bdfafa0221b?context=explore) (or any other container image derived from it) to get the _most_ of required softwares. Only additional item you would need is [scikit-build](https://scikit-build.readthedocs.io/en/latest/skbuild.html) which you can install with `pip`. 
  - If you know how to use `cmake` for building a software, you can actually skip `scikit-build` and run `cmake` from `src` directory.
2. Clone this repository (below) or fork-and-clone.
```
> git clone https://github.com/DeepLearnPhysics/edep2supera
```
3. Build 
```
> python3 setup.py build
```
4. Install (the example below installs under your `$HOME/.local` path)
```
> python3 setup.py install --user
```

## Software validation (unit test)
Simply try:
```
pytest
```
at the top-level directory. 

## How to contribute
1. Fork this repository to your personal github account.
2. Clone the repository to your local machine. Follow the build/install instruction above and make sure you can set up.
3. Create your branch to contain your own development.  Code code code.
4. When it's ready to be shared, make sure a unit test passes. Then request to merge by sending a pull request.

**Optional but strongly recommended**: implement a unit test for the added component of your code so that we can reduce chance of someone else breaking in future development.

## Status

Milestones

- [ ] Fill Supera datatypes from EDepSim datatypes (i.e. TG4Event attributes)
- [ ] Implement particle hierarchy tracking algorithm from [old Supera](https://github.com/DeepLearnPhysics/Supera)
- [ ] Implement a python run script to fill input data and run Supera
- [ ] Implement C++ code to drive larcv to create an output file and store information from Supera
