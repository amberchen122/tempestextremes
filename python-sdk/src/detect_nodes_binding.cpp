#include <pybind11/pybind11.h>
#include "../src/nodes/DetectNodes.cpp"
#include "../src/nodes/DetectNodes.h"
namespace py = pybind11;

PYBIND11_MODULE(DetectNodes, m) {
    m.doc() = "DetectNodes is used for the detection of nodal features. "
               "This executable is analogous to the \"map\" step in the "
               "\"MapReduce\" framework. Candidate points are selected based "
               "on information at a single time slice. Typically DetectNodes "
               "is followed by StitchNodes to connect candidate points in time."; 
	// py::class_<DetectCyclonesParam>(m, "DetectCyclonesParam")
	// 	.def(py::init<>())
	// 	.def_readwrite("fpLog", &DetectCyclonesParam::fpLog)
	// 	.def_readwrite("ixSearchBy", &DetectCyclonesParam::ixSearchBy)
	// 	.def_readwrite("strSearchByThreshold", &DetectCyclonesParam::strSearchByThreshold)
	// 	.def_readwrite("fSearchByMinima", &DetectCyclonesParam::fSearchByMinima)
	// 	.def_readwrite("nInitialSearchDist", &DetectCyclonesParam::nInitialSearchDist)
	// 	.def_readwrite("nInitialSearchDistSafe", &DetectCyclonesParam::nInitialSearchDistSafe)
	// 	.def_readwrite("dMaxLatitude", &DetectCyclonesParam::dMaxLatitude)
	// 	.def_readwrite("dMinLatitude", &DetectCyclonesParam::dMinLatitude)
	// 	.def_readwrite("dMinAbsLatitude", &DetectCyclonesParam::dMinAbsLatitude)
	// 	.def_readwrite("dMaxLongitude", &DetectCyclonesParam::dMaxLongitude)
	// 	.def_readwrite("dMinLongitude", &DetectCyclonesParam::dMinLongitude)
	// 	.def_readwrite("dMergeDist", &DetectCyclonesParam::dMergeDist)
	// 	.def_readwrite("pvecClosedContourOp", &DetectCyclonesParam::pvecClosedContourOp)
	// 	.def_readwrite("pvecNoClosedContourOp", &DetectCyclonesParam::pvecNoClosedContourOp)
	// 	.def_readwrite("pvecThresholdOp", &DetectCyclonesParam::pvecThresholdOp)
	// 	.def_readwrite("pvecOutputOp", &DetectCyclonesParam::pvecOutputOp)
	// 	.def_readwrite("nTimeStride", &DetectCyclonesParam::nTimeStride)
	// 	.def_readwrite("strTimeFilter", &DetectCyclonesParam::strTimeFilter)
	// 	.def_readwrite("strLatitudeName", &DetectCyclonesParam::strLatitudeName)
	// 	.def_readwrite("strLongitudeName", &DetectCyclonesParam::strLongitudeName)
	// 	.def_readwrite("fRegional", &DetectCyclonesParam::fRegional)
	// 	.def_readwrite("fDiagonalConnectivity", &DetectCyclonesParam::fDiagonalConnectivity)
	// 	.def_readwrite("fOutputHeader", &DetectCyclonesParam::fOutputHeader)
	// 	.def_readwrite("fOutputSeconds", &DetectCyclonesParam::fOutputSeconds)
	// 	.def_readwrite("iVerbosityLevel", &DetectCyclonesParam::iVerbosityLevel);
    m.def("DetectNodes", 
        &DetectNodes, 
        py::arg("varreg"), 
        py::arg("vecTimes"), 
        py::arg("vecFiles"), 
        py::arg("grid"), 
        py::arg("vecThresholdOp"), 
        py::arg("vecClosedContourOp"), 
        py::arg("vecNoClosedContourOp"), 
        py::arg("vecOutputOp"), 
        py::arg("strOutputFile"), 
        py::arg("param"));
}