# Design Document: Python SDK for TempestExtremes using pybind11

## 1. Introduction

This document outlines the design for building a Python SDK/interface for the TempestExtremes C++ executables using `pybind11`. The primary goal is to provide a seamless interface for Python users to leverage the capabilities of TempestExtremes without delving into the C++ codebase.

## 2. Background

TempestExtremes is a comprehensive toolset designed for the analysis of extreme weather events. The codebase is primarily written in C++ and provides a set of executables for various analyses. The aim is to extend its usability to the Python community.

## 3. Requirements

- **Seamless Integration**: The Python SDK should provide a straightforward interface, mirroring the functionalities of the C++ executables.

- **Performance**: The interface should be efficient and take advantage of the speed of the original C++ implementations.
  
- **Maintainability**: The Python SDK should be easily maintainable and updatable given any TempestExtremes codebase update.
  
- **CommandLine Mirroring**: The Python SDK should mirror the `CommandLine` macros used in the C++ code, providing a consistent user experience.

- **Distribution**: Package and distribute the Python interface via conda-forge.

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
    |-- build
    |-- src/
        |-- bindings.cpp
        |-- main_overrides.h
    |-- test/
        └── test.py
    |-- __init__.py
    |-- setup.py
    |-- README.md
```

### 4.2 Python Bindings with pybind11

### 4.3 Compilation

Use the `setup.py` script within the `python-sdk/` directory to automate the build process. 

Run the following command under python-sdk directory:

`python setup.py build_ext --inplace`

### 4.4 Testing

1. **Unit Tests**: `python .python-sdk/test/test.py`

2. **Continuous Integration**: Use platforms like GitHub Actions to automatically run tests whenever the C++ code or bindings are updated.

### 4.5 Documentation

1. **Inline Comments**: Ensure that each binding has inline comments detailing its functionality and any nuances related to its operation.

2. **User Guide**: 
    - Update the existing user guide or create a separate guide specifically for the Python SDK. This guide should provide examples, usage scenarios, and best practices.
    - Make sure the binding code include doc string so that user can access the documentation through help() from python terminal. 
