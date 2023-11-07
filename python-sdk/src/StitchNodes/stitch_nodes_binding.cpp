#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../src/nodes/DetectNodes.cpp"
#include "../src/base/Variable.h"
#include "../src/nodes/ClosedContourOp.h"
#include "../src/base/FilenameList.h"

namespace py = pybind11;

int main(int argc, char **argv); 

// Wrapper function that can be called from Python
int call_main_from_python(const std::vector<std::string> &args) {
    std::vector<char*> argv;
    for (const auto& arg : args)
        argv.push_back((char*)arg.data());
    argv.push_back(nullptr);  // argv must be null terminated
    return main(static_cast<int>(argv.size()) - 1, argv.data());
}

PYBIND11_MODULE(StitchNodes, m) {
    m.def("call_main", &call_main_from_python, "A function which calls the C++ main function");
}