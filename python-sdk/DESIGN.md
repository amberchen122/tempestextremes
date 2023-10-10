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
    |-- bindings/
        |-- detect_nodes_binding.cpp
        |-- ...
    |-- __init__.py
    |-- tempest_extremes.py
```

### 4.2 Python Bindings with pybind11

1. **DetectNodes**: Start with creating bindings for `DetectNodes` as it's a core functionality.
   
   - File: `pybindings/detectnodes_bindings.cpp`
   - Expose necessary functions/classes from `DetectNodes` to Python.
   - Use Python's `argparse` module to mirror the `CommandLine` macros.

2. **Other Modules**: Follow a similar approach for other modules in the `src/` directory.

### 4.3 Compilation

1. Use `c++` to compile the bindings. The compiled shared library (`.so` or `.pyd`) will be the primary output that Python users will import.

2. Use the `setup.py` script within the `python-sdk/` directory to automate the build process.

### 4.4 Testing

1. **Unit Tests**: Write unit tests for each Python binding to ensure correctness. These tests should reside in a `tests/` directory within `python-sdk/`.

2. **Continuous Integration**: Use platforms like GitHub Actions to automatically run tests whenever the C++ code or bindings are updated.

### 4.5 Documentation

1. **Inline Comments**: Ensure that each binding has inline comments detailing its functionality and any nuances related to its operation.

2. **User Guide**: Update the existing user guide or create a separate guide specifically for the Python SDK. This guide should provide examples, usage scenarios, and best practices.

## 5. Future Work

1. **Performance Optimization**: Monitor the performance of the Python SDK and consider optimizations, especially for computationally intensive tasks.

2. **Extended Functionality**: As TempestExtremes evolves, ensure that the Python SDK is updated to reflect new functionalities or changes in the C++ codebase.

3. **Community Feedback**: Engage with the user community to gather feedback and make iterative improvements to the SDK.

## 6. Conclusion

Building a Python SDK for TempestExtremes will significantly enhance its accessibility and usability within the Python community. By following the design outlined in this document, we aim to provide a robust, maintainable, and user-friendly interface that mirrors the power and functionality of the original C++ codebase.