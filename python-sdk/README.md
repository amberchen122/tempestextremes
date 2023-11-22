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
