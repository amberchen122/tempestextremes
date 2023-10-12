#include <pybind11/pybind11.h>
#include "../src/nodes/DetectNodes.cpp"
#include "../src/base/Variable.h"
#include "../src/nodes/ClosedContourOp.h"
namespace py = pybind11;

// test function
int my_add(int a, int b) {
	return a + b;
}

PYBIND11_MODULE(DetectNodes, m) {
    m.doc() = "DetectNodes is used for the detection of nodal features. "
               "This executable is analogous to the \"map\" step in the "
               "\"MapReduce\" framework. Candidate points are selected based "
               "on information at a single time slice. Typically DetectNodes "
               "is followed by StitchNodes to connect candidate points in time.";
	
	py::register_exception<Exception>(m, "Exception");
 
	py::class_<VariableRegistry>(m, "VariableRegistry")
		.def(py::init<>())
		.def("FindOrRegisterSubStr", &VariableRegistry::FindOrRegisterSubStr)
		.def("FindOrRegister", &VariableRegistry::FindOrRegister)
		.def("Get", &VariableRegistry::Get, py::return_value_policy::reference)
		.def("GetVariableString", &VariableRegistry::GetVariableString)
		.def("UnloadAllGridData", &VariableRegistry::UnloadAllGridData)
		.def("GetDependentVariableIndices", &VariableRegistry::GetDependentVariableIndices)
		.def("GetDependentVariableNames", &VariableRegistry::GetDependentVariableNames)
		// .def("GetAuxiliaryDimInfo", &VariableRegistry::GetAuxiliaryDimInfo) // not used anywhere in the TE cpp code
		.def("ClearProcessingQueue", &VariableRegistry::ClearProcessingQueue)
		.def("AppendVariableToProcessingQueue", &VariableRegistry::AppendVariableToProcessingQueue)
		.def("GetProcessingQueueVarPos", &VariableRegistry::GetProcessingQueueVarPos)
		.def("GetProcessingQueueVarIx", &VariableRegistry::GetProcessingQueueVarIx)
		.def("GetProcessingQueueVariable", &VariableRegistry::GetProcessingQueueVariable, py::return_value_policy::reference)
		.def("GetProcessingQueueAuxIx", &VariableRegistry::GetProcessingQueueAuxIx, py::return_value_policy::reference)
		.def("GetProcessingQueueAuxSize", &VariableRegistry::GetProcessingQueueAuxSize, py::return_value_policy::reference)
		.def("GetProcessingQueueOffset", &VariableRegistry::GetProcessingQueueOffset)
		.def("AdvanceProcessingQueue", &VariableRegistry::AdvanceProcessingQueue)
		.def("ResetProcessingQueue", &VariableRegistry::ResetProcessingQueue)
		.def("GetDataOp", &VariableRegistry::GetDataOp, py::return_value_policy::reference);
	py::class_<DetectCyclonesParam>(m, "DetectCyclonesParam")
		.def(py::init<>())
		.def("__repr__", [](const DetectCyclonesParam &param) {
			std::stringstream repr;
			repr << "DetectCyclonesParam("
				 << "ixSearchBy=" << param.ixSearchBy << ", "
				 << "strSearchByThreshold=" << param.strSearchByThreshold << ", "
				 << "fSearchByMinima=" << param.fSearchByMinima << ", "
				 << "nInitialSearchDist=" << param.nInitialSearchDist << ", "
				 << "nInitialSearchDistSafe=" << param.nInitialSearchDistSafe << ", "
				 << "dMaxLatitude=" << param.dMaxLatitude << ", "
				 << "dMinLatitude=" << param.dMinLatitude << ", "
				 << "dMinAbsLatitude=" << param.dMinAbsLatitude << ", "
				 << "dMaxLongitude=" << param.dMaxLongitude << ", "
				 << "dMinLongitude=" << param.dMinLongitude << ", "
				 << "dMergeDist=" << param.dMergeDist << ", "
				 << "pvecClosedContourOp=[";
			// for (const auto& op : param.pvecClosedContourOp) {
			// 	repr << op << ", ";
			// }
			// repr << "], pvecNoClosedContourOp=[";
			// for (const auto& op : param.pvecNoClosedContourOp) {
			// 	repr << op << ", ";
			// }
			// repr << "], pvecThresholdOp=[";
			// for (const auto& op : param.pvecThresholdOp) {
			// 	repr << op << ", ";
			// }
			// repr << "], pvecOutputOp=[";
			// for (const auto& op : param.pvecOutputOp) {
			// 	repr << op << ", ";
			// }
			repr << "], nTimeStride=" << param.nTimeStride << ", "
				 << "strTimeFilter=" << param.strTimeFilter << ", "
				 << "strLatitudeName=" << param.strLatitudeName << ", "
				 << "strLongitudeName=" << param.strLongitudeName << ", "
				 << "fRegional=" << param.fRegional << ", "
				 << "fDiagonalConnectivity=" << param.fDiagonalConnectivity << ", "
				 << "fOutputHeader=" << param.fOutputHeader << ", "
				 << "fOutputSeconds=" << param.fOutputSeconds << ", "
				 << "iVerbosityLevel=" << param.iVerbosityLevel << ")";
			return repr.str();
		})
		// .def_readwrite("fpLog", &DetectCyclonesParam::fpLog) //access through setter instead
		.def_readwrite("ixSearchBy", &DetectCyclonesParam::ixSearchBy)
		.def_readwrite("strSearchByThreshold", &DetectCyclonesParam::strSearchByThreshold)
		.def_readwrite("fSearchByMinima", &DetectCyclonesParam::fSearchByMinima)
		.def_readwrite("nInitialSearchDist", &DetectCyclonesParam::nInitialSearchDist)
		.def_readwrite("nInitialSearchDistSafe", &DetectCyclonesParam::nInitialSearchDistSafe)
		.def_readwrite("dMaxLatitude", &DetectCyclonesParam::dMaxLatitude)
		.def_readwrite("dMinLatitude", &DetectCyclonesParam::dMinLatitude)
		.def_readwrite("dMinAbsLatitude", &DetectCyclonesParam::dMinAbsLatitude)
		.def_readwrite("dMaxLongitude", &DetectCyclonesParam::dMaxLongitude)
		.def_readwrite("dMinLongitude", &DetectCyclonesParam::dMinLongitude)
		.def_readwrite("dMergeDist", &DetectCyclonesParam::dMergeDist)
		.def_readwrite("pvecClosedContourOp", &DetectCyclonesParam::pvecClosedContourOp)
		.def_readwrite("pvecNoClosedContourOp", &DetectCyclonesParam::pvecNoClosedContourOp)
		.def_readwrite("pvecThresholdOp", &DetectCyclonesParam::pvecThresholdOp)
		.def_readwrite("pvecOutputOp", &DetectCyclonesParam::pvecOutputOp)
		.def_readwrite("nTimeStride", &DetectCyclonesParam::nTimeStride)
		.def_readwrite("strTimeFilter", &DetectCyclonesParam::strTimeFilter)
		.def_readwrite("strLatitudeName", &DetectCyclonesParam::strLatitudeName)
		.def_readwrite("strLongitudeName", &DetectCyclonesParam::strLongitudeName)
		.def_readwrite("fRegional", &DetectCyclonesParam::fRegional)
		.def_readwrite("fDiagonalConnectivity", &DetectCyclonesParam::fDiagonalConnectivity)
		.def_readwrite("fOutputHeader", &DetectCyclonesParam::fOutputHeader)
		.def_readwrite("fOutputSeconds", &DetectCyclonesParam::fOutputSeconds)
		.def_readwrite("iVerbosityLevel", &DetectCyclonesParam::iVerbosityLevel)
		.def("set_log_path", [](DetectCyclonesParam &self, const std::string &path) {
			self.fpLog = fopen(path.c_str(), "w");
			if (!self.fpLog) {
				throw std::runtime_error("Failed to open file at " + path);
			}
		})
		// setter for pvecClosedContourOp
		.def("set_closed_contour_cmd", [](DetectCyclonesParam &self, py::list pylist) {
			std::vector<std::vector<std::string>> closed_contour_cmds;
			for (const auto& item : pylist) {
				if (py::isinstance<py::sequence>(item)) {
					py::sequence seq = item.cast<py::sequence>();
					std::vector<std::string> cmd;

					for (const auto& str_item : seq) {
						if (py::isinstance<py::str>(str_item)) {
							std::string str = str_item.cast<std::string>();
							cmd.push_back(str);
						} else {
							throw std::runtime_error("All inner items must be strings.");
						}
					}

					closed_contour_cmds.push_back(cmd);
				} else {
					throw std::runtime_error("All items in the list must be sequences of strings.");
				}
			}
			std::vector<ClosedContourOp> vecClosedContourOp;
			// for (const auto& cmd : closed_contour_cmds) {
			// 	ClosedContourOp op;
			// 	std::string strSubStr;
			// 	for (const auto& str : op) {
			// 		strSubStr += str + ",";
			// 	}
			// 	strSubStr.pop_back(); // remove the last comma
			// 	op.Parse(self.varreg, strSubStr); 
			// 	vecClosedContourOp.push_back(op);
			// }
			// for (const auto& cmd : closed_contour_cmds) {
			// 	for (const auto& str : cmd) {
			// 		std::cout << str << std::endl;
			// 	}
			// }
		})

		;

    m.def("DetectNodes", [](
				const std::string& strInputFiles, 
				const std::string& strOutputFile, 
				const std::string& strConnectivity, 
				VariableRegistry& varreg, 
				const DetectCyclonesParam& param) 
		{
			int iFile = 0; //placeholder value since iFile is not used in DetectCyclonesUnstructured
			// try
			// {
			// 	DetectCyclonesUnstructured(iFile, strInputFiles, strOutputFile, strConnectivity, varreg, param);
			// }
			// catch(const std::exception& e)
			// {
			// 	std::cerr << e.what() << '\n';
			// }
			DetectCyclonesUnstructured(iFile, strInputFiles, strOutputFile, strConnectivity, varreg, param);

		}, 
        py::arg("strInputFiles"), 
        py::arg("strOutputFile"), 
        py::arg("strConnectivity"), 
        py::arg("varreg"), 
        py::arg("param"));


	// just a test function 
	// m.def("my_add", &my_add, "A function which adds two numbers");
}