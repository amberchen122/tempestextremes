#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <mpi.h>

#include "../src/nodes/DetectNodes.cpp"
#include "../src/base/Variable.h"
#include "../src/nodes/ClosedContourOp.h"
#include "../src/base/FilenameList.h"

namespace py = pybind11;



/**
 * @brief Parses a list of operations and returns a vector of the specified operation type.
 * 
 * @tparam OpType The type of operation to parse.
 * @param pyOpList The list of operations to parse.
 * @param varreg The variable registry to use for parsing.
 * @return std::vector<OpType>* A pointer to the vector of parsed operations.
 */
template <typename OpType>
std::vector<OpType>* parseOperations(
    const py::list& pyOpList, 
    VariableRegistry& varreg
) {
    auto vecOps = new std::vector<OpType>(); // Allocate on the heap
    for (const auto& item : pyOpList) {
        std::string strOp = item.cast<std::string>();
        OpType op;
        op.Parse(varreg, strOp);
        vecOps->push_back(op);
    }
    return vecOps;
}

class DetectNodesParameter {
	public:
		DetectNodesParameter(
			// TODO: should only specify one of --in_data and --in_data_list
			// TODO: should only specify one of --out_data and --out_data_list
			std::string strInputFile = "", // in_data file
			std::string strOutputFile = "", // out data file, used when in_data is specified
			std::string strInputFileList = "", // in_data_list file
			std::string strOutputFileList = "", // out_data_list file, used when in_data_list is specified

			std::string strConnectivity = "", // connectivity file

			// variables to set variableRegistry and detectCyclonesParam
			bool diag_connect = false, // --diag_connect <bool> //TODO: what should be the default value?
			// Search by minima
			bool searchByMin = false, 
			std::string strSearchBy = "PSL", 
			std::string strSearchByThreshold = "", //TODO: what should be the default value?
			// Maximum latitude for detection
			double dMaxLatitude = 0,
			// Minimum latitude for detection
			double dMinLatitude = 0,
			// Minimum absolute value of latitude for detection
			double dMinAbsLatitude = 0,
			// Maximum longitude for detection
			double dMaxLongitude = 0,
			// Minimum longitude for detection
			double dMinLongitude = 0,
			// Merge distance
			double dMergeDist = 0,
			// Time stride
			int nTimeStride = 1,
			// Time filter
			std::string strTimeFilter = "",
			// Name of the latitude dimension
			std::string strLatitudeName = "lat",
			// Name of the longitude dimension
			std::string strLongitudeName = "lon",
			// Regional (do not wrap longitudinal boundaries)
			bool fRegional = false,
			// Output header
			bool fOutputHeader = false, //TODO: what should be the default value?
			// Output seconds as part of timestamp
			bool fOutputSeconds = false, //TODO: what should be the default value?
			// Verbosity level
			int iVerbosityLevel = 0,
			// TODO: logdir
			std::string strLogDir = "."
		): 
		varreg(), dcuparam()
		{
			// initialize vecInputFiles and vecOutputFiles
			// building a unified interface for input and output files
			if (strInputFile != "") {
				this->vecInputFiles.push_back(strInputFile);
				this->vecOutputFiles.push_back(strOutputFile);
			} else if (strInputFileList != "") {
				this->vecInputFiles.FromFile(strInputFileList, true);
				this->vecOutputFiles.FromFile(strOutputFileList);
				if (this->vecOutputFiles.size() == 0) {
					/* default output file names
					if out_data_list is not specified, then the output files are named as out000000.dat, out000001.dat, ... */
					for (int f = 0; f < this->numInputFilesLines; f++) {
						char szFileIndex[32];
						snprintf(szFileIndex, 32, "%06i", f);	
						std::string strOutputFile = "out" + std::string(szFileIndex) + ".dat";
						this->vecOutputFiles.push_back(strOutputFile);
					}
					
				}
			} else {
				_EXCEPTIONT("Either in_data or in_data_list must be specified");
			}
		
			if (this->vecInputFiles.size() != this->vecOutputFiles.size()) {
				_EXCEPTIONT("Number of input files must equal number of output files");
			}

			this->numInputFilesLines = this->vecInputFiles.size();

			this->strConnectivity = strConnectivity;              

			// initialize detectCyclonesParam
			this->dcuparam.fDiagonalConnectivity = diag_connect;

			this->dcuparam.fSearchByMinima = searchByMin;
			this->dcuparam.ixSearchBy = varreg.FindOrRegister(strSearchBy);
			this->dcuparam.strSearchByThreshold = strSearchByThreshold;

			this->dcuparam.dMaxLatitude = dMaxLatitude;
			this->dcuparam.dMinLatitude = dMinLatitude;
			this->dcuparam.dMinAbsLatitude = dMinAbsLatitude;
			this->dcuparam.dMaxLongitude = dMaxLongitude;
			this->dcuparam.dMinLongitude = dMinLongitude;
			this->dcuparam.dMergeDist = dMergeDist;
			LongLatValidationProcess();

			this->dcuparam.nTimeStride = nTimeStride;
			this->dcuparam.strTimeFilter = strTimeFilter;
			// Note timestride is deprecated
			if (dcuparam.nTimeStride != 1) {
				//Announce("WARNING: --timestride is deprecated.  Consider using --timefilter instead.");
				if (dcuparam.strTimeFilter != "") {
					_EXCEPTIONT("Only one of --timestride and --timefilter can be used.");
				}
			}
			this->dcuparam.strLatitudeName = strLatitudeName;
			this->dcuparam.strLongitudeName = strLongitudeName;
			this->dcuparam.fRegional = fRegional;
			this->dcuparam.fOutputHeader = fOutputHeader;
			this->dcuparam.fOutputSeconds = fOutputSeconds;
			this->dcuparam.iVerbosityLevel = iVerbosityLevel;

			this->strLogDir = strLogDir;

			// initialize vecLogFiles
			for (int f = 0; f < this->numInputFilesLines; f++) {
				char szFileIndex[32];
				snprintf(szFileIndex, 32, "%06i", f);	
				std::string strLogFile = this->strLogDir + "/log" + std::string(szFileIndex) + ".txt";
				this->vecLogFiles.push_back(strLogFile);
			}
		}

		~DetectNodesParameter() {
		}	

	public:
		VariableRegistry varreg;       // Instance of VariableRegistry class, its constructor does not take any arguments
		DetectCyclonesParam dcuparam; // Instance of DetectCyclonesParam class, its constructor does not take any arguments
		
		FilenameList vecInputFiles;
		FilenameList vecOutputFiles;
		// number of input files, equal to 1 if --in_data is specified, otherwise equal to the number of files in --in_data_list
		int numInputFilesLines;
		std::string strConnectivity;
		std::string strLogDir;
		std::vector<std::string> vecLogFiles;
	
	private:
		// perform validation on longitudes and latitudes
		// also convert longitudes and latitudes to radians
		void LongLatValidationProcess() {
			// Check minimum/maximum latitude/longitude
			if ((dcuparam.dMaxLatitude < -90.0) || (dcuparam.dMaxLatitude > 90.0)) {
				_EXCEPTIONT("--maxlat must in the range [-90,90]");
			}
			if ((dcuparam.dMinLatitude < -90.0) || (dcuparam.dMinLatitude > 90.0)) {
				_EXCEPTIONT("--minlat must in the range [-90,90]");
			}
			if (dcuparam.dMinLatitude > dcuparam.dMaxLatitude) {
				_EXCEPTIONT("--minlat must be less than --maxlat");
			}

			dcuparam.dMaxLatitude *= M_PI / 180.0;
			dcuparam.dMinLatitude *= M_PI / 180.0;
			dcuparam.dMinAbsLatitude *= M_PI / 180.0;

			if (dcuparam.dMinLongitude < 0.0) {
				int iMinLongitude =
					static_cast<int>(-dcuparam.dMinLongitude / 360.0);
				dcuparam.dMinLongitude +=
					static_cast<double>(iMinLongitude + 1) * 360.0;
			}
			if (dcuparam.dMinLongitude >= 360.0) {
				int iMinLongitude =
					static_cast<int>(dcuparam.dMinLongitude / 360.0);
				dcuparam.dMinLongitude -=
					static_cast<double>(iMinLongitude - 1) * 360.0;
			}
			if (dcuparam.dMaxLongitude < 0.0) {
				int iMaxLongitude =
					static_cast<int>(-dcuparam.dMaxLongitude / 360.0);
				dcuparam.dMaxLongitude +=
					static_cast<double>(iMaxLongitude + 1) * 360.0;
			}
			if (dcuparam.dMaxLongitude >= 360.0) {
				int iMaxLongitude =
					static_cast<int>(dcuparam.dMaxLongitude / 360.0);
				dcuparam.dMaxLongitude -=
					static_cast<double>(iMaxLongitude - 1) * 360.0;
			}

			dcuparam.dMaxLongitude *= M_PI / 180.0;
			dcuparam.dMinLongitude *= M_PI / 180.0;
		}
};



PYBIND11_MODULE(DetectNodes, m) {
	m.doc() = "DetectNodes is used for the detection of nodal features. "
			   "This executable is analogous to the \"map\" step in the "
			   "\"MapReduce\" framework. Candidate points are selected based "
			   "on information at a single time slice. Typically DetectNodes "
			   "is followed by StitchNodes to connect candidate points in time.";
	
	// Register cpp exeptions with python
	// Register exception translator
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p); // This allows the exception to be re-thrown to identify its type
        } catch (const Exception &e) {
            // Translate the C++ exception into a Python exception
            PyErr_SetString(PyExc_RuntimeError, e.ToString().c_str());
        }
    });
	
	// Expose the DetectNodesParameter class to python
	py::class_<DetectNodesParameter>(m, "DetectNodesParameter", 
		R"pbdoc(
			DetectNodesParameter class is used for configuring the parameters required for DeteNoodes function.
			Parameters: 
			----------
			Parameters to specify input and output files, either InputFile or InputFileList must be specified:
			Option 1: specify InputFile 
			inputFile (str) [""]: A list of input data files in NetCDF format, separated by semicolons. Example: "input1.nc;input2.nc; ..." 
			outputFile (str) ["out.dat"]): Path to the output nodefile to write from the detection procedure. Used if InputFile is specified.
			Option 2: specify InputFileList
			inputFileList (str) [""]: Path to a a text file containing the InputFile argument for a sequence of processing operations (one per line). Example: "input1.nc;input2.nc;\n input3.nc;input4.nc;\n ..."
			outputFileList (str) [""]: Path to a text file containingan equal number of lines to InputFileList specifying the output nodefiles from each input datafile. If not specified, the output files are named as out000000.dat, out000001.dat, ...
			----------
			logDir (str) ["."]: Path to the directory where the log files will be written. The log files are named as log000000.txt, log000001.txt, ...
			----------
			connectivityFile (str) [""]: Path to a a connectivity file that describes the unstructured grid.
			----------
			diag_connect (bool) [False]: If True: when the data is on a structured grid, consider grid cells to be connected in the diagonal (across the vertex).
			----------
			Parameters for initially selecting candidate points (defined as local minima or local maxima).
			searchByMin (bool) [False]: If True: search for local minima, otherwise search for local maxima.
			searchBy (str) ["PSL"]: The variable to use for searching for local minima or maxima.
			searchByThreshold (str) [""]: The threshold to use for searching for local minima or maxima. //TODO: Describe the default behavior if this is not specified.
			----------
			maxLatitude (float) [0.0]: The maximum latitude for candidate points. If maxLatitude and minLatitude are equal then these arguments are ignored.
			minLatitude (float) [0.0]: The minimum latitude for candidate points. If maxLatitude and minLatitude are equal then these arguments are ignored.
		)pbdoc")
        .def(py::init([](//TODO: change the order of arguements
						const std::string& strInputFile,
                         const std::string& strOutputFile,
                         const std::string& strInputFileList,
                         const std::string& strOutputFileList,
                         const std::string& strConnectivity,
                         bool diag_connect,
                         bool searchByMin,
                         const std::string& strSearchBy,
                         const std::string& strSearchByThreshold,
                         double dMaxLatitude,
                         double dMinLatitude,
                         double dMinAbsLatitude,
                         double dMaxLongitude,
                         double dMinLongitude,
                         double dMergeDist,
                         int nTimeStride,
                         const std::string& strTimeFilter,
                         const std::string& strLatitudeName,
                         const std::string& strLongitudeName,
                         bool fRegional,
                         bool fOutputHeader,
                         bool fOutputSeconds,
                         int iVerbosityLevel,
                         const std::string& strLogDir,
						 py::list closedContourOp,
						 py::list noClosedContourOp,
						 py::list thresholdOp,
						 py::list nodeOutputOp
						 ) 
		{
            DetectNodesParameter param = DetectNodesParameter(strInputFile, strOutputFile, strInputFileList, strOutputFileList,
                                            strConnectivity, diag_connect, searchByMin, strSearchBy, strSearchByThreshold,
                                            dMaxLatitude, dMinLatitude, dMinAbsLatitude, dMaxLongitude, dMinLongitude,
                                            dMergeDist, nTimeStride, strTimeFilter, strLatitudeName, strLongitudeName,
                                            fRegional, fOutputHeader, fOutputSeconds, iVerbosityLevel, strLogDir);

			// Set the pointers after parsing the operations
			param.dcuparam.pvecClosedContourOp = parseOperations<ClosedContourOp>(closedContourOp, param.varreg);
			param.dcuparam.pvecNoClosedContourOp = parseOperations<ClosedContourOp>(noClosedContourOp, param.varreg);
			param.dcuparam.pvecThresholdOp = parseOperations<ThresholdOp>(thresholdOp, param.varreg);
			param.dcuparam.pvecOutputOp = parseOperations<NodeOutputOp>(nodeOutputOp, param.varreg);

			return param;
		}
		
		),
			"DetectNodesParameter constructor", // TODO: add docstring
            py::arg("inputFile") = "",
            py::arg("outputFile") = "out.dat",
            py::arg("inputFileList") = "",
			py::arg("outputFileList") = "",
			py::arg("logDir") = ".",
            py::arg("connectivityFile") = "",
            py::arg("diagConnect") = false,
            py::arg("searchByMin") = false,
            py::arg("searchBy") = "PSL",
            py::arg("searchByThreshold") = "",
            py::arg("maxLatitude") = 0.0,
            py::arg("minLatitude") = 0.0,
            py::arg("minAbsLatitude") = 0.0,
            py::arg("maxLongitude") = 0.0,
            py::arg("minLongitude") = 0.0,
            py::arg("mergeDist") = 0.0,

            py::arg("nTimeStride") = 1,
            py::arg("strTimeFilter") = "",
            py::arg("strLatitudeName") = "lat",
            py::arg("strLongitudeName") = "lon",
            py::arg("fRegional") = false,
            py::arg("fOutputHeader") = false,
            py::arg("fOutputSeconds") = false,
            py::arg("iVerbosityLevel") = 0,
			
            py::arg("closedContourOp") = py::list(),
			py::arg("noClosedContourOp") = py::list(),
			py::arg("thresholdOp") = py::list(),
			py::arg("nodeOutputOp") = py::list()
			)

			// TODO: add setter for the above parameters
			// TODO add __repr__ for the class
			.def("__repr__", [](const DetectNodesParameter &param) {
				std::stringstream repr;

				// TODO: describe how output will be written to file (one on one mapping of vecInputFiles and vecOutputFiles)
				

				repr << "DetectNodesParameter(";
				repr << "strConnectivity='" << param.strConnectivity << "', ";
				repr << "diag_connect=" << (param.dcuparam.fDiagonalConnectivity ? "True" : "False") << ", ";
				repr << "searchByMin=" << (param.dcuparam.fSearchByMinima ? "True" : "False") << ", ";
				repr << "strSearchBy='" << param.dcuparam.ixSearchBy << "', ";
				repr << "strSearchByThreshold='" << param.dcuparam.strSearchByThreshold << "', ";
				repr << "dMaxLatitude=" << param.dcuparam.dMaxLatitude << ", ";
				repr << "dMinLatitude=" << param.dcuparam.dMinLatitude << ", ";
				repr << "dMinAbsLatitude=" << param.dcuparam.dMinAbsLatitude << ", ";
				repr << "dMaxLongitude=" << param.dcuparam.dMaxLongitude << ", ";
				repr << "dMinLongitude=" << param.dcuparam.dMinLongitude << ", ";
				repr << "dMergeDist=" << param.dcuparam.dMergeDist << ", ";
				repr << "nTimeStride=" << param.dcuparam.nTimeStride << ", ";
				repr << "strTimeFilter='" << param.dcuparam.strTimeFilter << "', ";
				repr << "strLatitudeName='" << param.dcuparam.strLatitudeName << "', ";
				repr << "strLongitudeName='" << param.dcuparam.strLongitudeName << "', ";
				repr << "fRegional=" << (param.dcuparam.fRegional ? "True" : "False") << ", ";
				repr << "fOutputHeader=" << (param.dcuparam.fOutputHeader ? "True" : "False") << ", ";
				repr << "fOutputSeconds=" << (param.dcuparam.fOutputSeconds ? "True" : "False") << ", ";
				repr << "iVerbosityLevel=" << param.dcuparam.iVerbosityLevel << ", ";
				repr << "strLogDir='" << param.strLogDir << "'";
				repr << ")";
				return repr.str();
			})

			;


	// main function calling DetectCyclonesUnstructured for each file in DetectNodesParameter.vecInputFiles
	m.def("DetectNodes", 
		[](DetectNodesParameter& param){
			for (int f = 0; f < param.numInputFilesLines; f++) {

				// open the log file
				param.dcuparam.fpLog = fopen(param.vecLogFiles[f].c_str(), "w");

				DetectCyclonesUnstructured(f, 
											param.vecInputFiles[f], 
											param.vecOutputFiles[f], 
											param.strConnectivity, 
											const_cast<VariableRegistry&>(param.varreg), 
											const_cast<DetectCyclonesParam&>(param.dcuparam));
				// close the log file
				fclose(param.dcuparam.fpLog);
			}
		}, 
		py::arg("DetectNodesParameter")
	);


}