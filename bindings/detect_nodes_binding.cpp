#include <pybind11/pybind11.h>
#include "src\nodes\DetectNodes.cpp.h"

namespace py = pybind11;

PYBIND11_MODULE(DetectNodes, m) {
    m.def("DetectNodes", &DetectNodesFunction, "A function that wraps DetectNodes functionality");
}
