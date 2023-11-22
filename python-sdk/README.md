# Design Document: Python SDK for TempestExtremes using pybind11

## 1. Introduction

This document outlines the design for building a Python SDK/interface for the TempestExtremes C++ executables using `pybind11`. The primary goal is to provide a seamless interface for Python users to leverage the capabilities of TempestExtremes without delving into the C++ codebase.

## 2. Background

TempestExtremes is a comprehensive toolset designed for the analysis of extreme weather events. The codebase is primarily written in C++ and provides a set of executables for various analyses. The aim is to extend its usability to the Python community.

## 4. Design

### 4.1 Directory Structure

```
TempestExtremes/
│
├── analysis/
│   ├── blobmetrics/
│   └── ...
│
├── src/
│   ├── atmrivers/
│   ├── base/
│   ├── blobs/
│   ├── blocking/
│   ├── metrics/
│   ├── nodes/
│   ├── util/
│   └── ...
│
├── test/
│   └── ...
│
|-- python-sdk/
    |-- build/
    |-- src/
        |-- bindings.cpp
        |-- main_overrides.h
    |-- test/
        └── test.py
    |-- __init__.py
    |-- setup.py
    |-- README.md
```

## Compilation

To compile, run the following commands:

`cd python-sdk`

`python setup.py build_ext --inplace`

## Testing

`python ./python-sdk/test.py`

## Functions
    AccumulateData(...) method of builtins.PyCapsule instance
        AccumulateData(arg0: Dict[str, str]) -> int
    
    AccumulateERA5Forecast(...) method of builtins.PyCapsule instance
        AccumulateERA5Forecast(arg0: Dict[str, str]) -> int
    
    BlobStats(...) method of builtins.PyCapsule instance
        BlobStats(arg0: Dict[str, str]) -> int
    
    Climatology(...) method of builtins.PyCapsule instance
        Climatology(arg0: Dict[str, str]) -> int
    
    CompressBlobs(...) method of builtins.PyCapsule instance
        CompressBlobs(arg0: Dict[str, str]) -> int
    
    DetectBlobs(...) method of builtins.PyCapsule instance
        DetectBlobs(arg0: Dict[str, str]) -> int
    
    DetectNodes(...) method of builtins.PyCapsule instance
        DetectNodes(arg0: Dict[str, str]) -> int
    
    FourierFilter(...) method of builtins.PyCapsule instance
        FourierFilter(arg0: Dict[str, str]) -> int
    
    GenerateConnectivityFile(...) method of builtins.PyCapsule instance
        GenerateConnectivityFile(arg0: Dict[str, str]) -> int
    
    GenerateNearestNeighborMap(...) method of builtins.PyCapsule instance
        GenerateNearestNeighborMap(arg0: Dict[str, str]) -> int

    HistogramNodes(...) method of builtins.PyCapsule instance
        HistogramNodes(arg0: Dict[str, str]) -> int
    
    IntegrateDimension(...) method of builtins.PyCapsule instance
        IntegrateDimension(arg0: Dict[str, str]) -> int
    
    LagrangianParcelTracker(...) method of builtins.PyCapsule instance
        LagrangianParcelTracker(arg0: Dict[str, str]) -> int
    
    NodeFileCompose(...) method of builtins.PyCapsule instance
        NodeFileCompose(arg0: Dict[str, str]) -> int
    
    NodeFileEditor(...) method of builtins.PyCapsule instance
        NodeFileEditor(arg0: Dict[str, str]) -> int
    
    PersistentBlobs(...) method of builtins.PyCapsule instance
        PersistentBlobs(arg0: Dict[str, str]) -> int
    
    QuantileCalculator(...) method of builtins.PyCapsule instance
        QuantileCalculator(arg0: Dict[str, str]) -> int
    
    ShapefileMask(...) method of builtins.PyCapsule instance
        ShapefileMask(arg0: Dict[str, str]) -> int
    
    SpineARs(...) method of builtins.PyCapsule instance
        SpineARs(arg0: Dict[str, str]) -> int
    
    StitchBlobs(...) method of builtins.PyCapsule instance
        StitchBlobs(arg0: Dict[str, str]) -> int
    
    StitchNodes(...) method of builtins.PyCapsule instance
        StitchNodes(arg0: Dict[str, str]) -> int
    
    VariableProcessor(...) method of builtins.PyCapsule instance
        VariableProcessor(arg0: Dict[str, str]) -> int
## Usage 

### DetectNodes Example
`from TempestExtremes import *

cmd_dict = {
    "--in_data": "./test/cn_files/outCSne30_test2.nc",
    "--timefilter": '"6hr"',
    "--out": "out1.dat",
    "--searchbymin": "MSL",
    "--closedcontourcmd": "PRMSL_L101,200.,4,0;TMP_L100,-0.4,8.0,1.1",
    "--mergedist": "6.0",
    "--outputcmd": "MSL,min,0;_VECMAG(VAR_10U,VAR_10V),max,2;ZS,min,0",
    "--latname": "lat",
    "--lonname": "lon",
}

print(cmd_dict)

DetectNodes(cmd_dict)`
