#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "main_overrides.h"

namespace py = pybind11;

typedef int (*MainFunc)(int, char**);

template <MainFunc func>
int call_main_from_map(const std::map<std::string, std::string> &cmd_map) {
    std::vector<std::string> args;
    args.push_back("EXECUTABLE"); // argv[0] is the executable name
    for (const auto &pair : cmd_map) {
        args.push_back(pair.first);
        args.push_back(pair.second);
    }
    std::vector<char*> argv;
    for (const auto& arg : args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);

    return func(static_cast<int>(argv.size()) - 1, argv.data());
}

#define CALL_MAIN(exe) call_main_from_map<exe##_main>


PYBIND11_MODULE(TempestExtremes, m) {
    m.doc() = "DetectNodes is used for the detection of nodal features. "
               "This executable is analogous to the \"map\" step in the "
               "\"MapReduce\" framework. Candidate points are selected based "
               "on information at a single time slice. Typically DetectNodes "
               "is followed by StitchNodes to connect candidate points in time.";

    m.def("DetectNodes", & CALL_MAIN(DetectNodes));
    m.def("StitchNodes", & CALL_MAIN(StitchNodes));
    m.def("AccumulateData", & CALL_MAIN(AccumulateData));
    m.def("AccumulateERA5Forecast", & CALL_MAIN(AccumulateERA5Forecast));
    m.def("BlobStats", & CALL_MAIN(BlobStats));
    m.def("Climatology", & CALL_MAIN(Climatology));
    m.def("CompressBlobs", & CALL_MAIN(CompressBlobs));
    m.def("DetectBlobs", & CALL_MAIN(DetectBlobs));
    m.def("FourierFilter", & CALL_MAIN(FourierFilter));
    m.def("GenerateConnectivityFile", & CALL_MAIN(GenerateConnectivityFile));
    m.def("GenerateNearestNeighborMap", & CALL_MAIN(GenerateNearestNeighborMap));
    m.def("HistogramNodes", & CALL_MAIN(HistogramNodes));
    m.def("IntegrateDimension", & CALL_MAIN(IntegrateDimension));
    m.def("LagrangianParcelTracker", & CALL_MAIN(LagrangianParcelTracker));
    m.def("NodeFileCompose", & CALL_MAIN(NodeFileCompose));
    m.def("NodeFileEditor", & CALL_MAIN(NodeFileEditor));
    m.def("PersistentBlobs", & CALL_MAIN(PersistentBlobs));
    m.def("QuantileCalculator", & CALL_MAIN(QuantileCalculator));
    m.def("ShapefileMask", & CALL_MAIN(ShapefileMask));
    m.def("SpineARs", & CALL_MAIN(SpineARs));
    m.def("StitchBlobs", & CALL_MAIN(StitchBlobs));
    m.def("VariableProcessor", & CALL_MAIN(VariableProcessor));
}
