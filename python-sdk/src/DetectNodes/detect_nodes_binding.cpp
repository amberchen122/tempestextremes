#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../src/nodes/DetectNodes.cpp"
#include "../src/base/Variable.h"
#include "../src/nodes/ClosedContourOp.h"
#include "../src/base/FilenameList.h"

namespace py = pybind11;


/**
 * @brief Parses a list of operations and returns a vector of the specified operation type. Operation types include ClosedContourOp, ThresholdOp, and NodeOutputOp.
 * 
 * @tparam OpType The type of operation to parse.
 * @param pyOpList The list of operations to parse. Example for NodeOutputOp ["PRMSL_L101,max,0", "_VECMAG(U_GRD_L100,V_GRD_L100),max,4", "HGT_L1,max,0"]
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
			std::string strInputFile = "",
            std::string strOutputFile = "",
            std::string strInputFileList = "",
            std::string strOutputFileList = "",
            std::string strConnectivity = "",
            bool diag_connect = false,
            bool searchByMin = false, 
            std::string strSearchBy = "PSL", 
            std::string strSearchByThreshold = "",
            double dMaxLatitude = 0,
            double dMinLatitude = 0,
            double dMinAbsLatitude = 0,
            double dMaxLongitude = 0,
            double dMinLongitude = 0,
            double dMergeDist = 0,
            int nTimeStride = 1,
            std::string strTimeFilter = "",
            std::string strLatitudeName = "lat",
            std::string strLongitudeName = "lon",
            bool fRegional = false,
            bool fOutputHeader = false,
            bool fOutputSeconds = false,
            int iVerbosityLevel = 0,
            std::string strLogDir = "."
		): 
		// initialize 
		varreg(), dcuparam()
		{
			this->dcuparam.nTimeStride = nTimeStride;
			this->dcuparam.strTimeFilter = strTimeFilter;
			// Note timestride is deprecated
			if (dcuparam.nTimeStride != 1) {
				//Announce("WARNING: --timestride is deprecated.  Consider using --timefilter instead.");
				if (dcuparam.strTimeFilter != "") {
					_EXCEPTIONT("Only one of timeStride and timeFilter can be used.");
				}
			}
	// TODO: Here is where i left off 10:25 PM 10/26/2023
			
			// ensure that only one of inputFile and inputFileList is specified
			if (strInputFile != "" && strInputFileList != "")
			{
				_EXCEPTIONT("Only one of inputFile and inputFileList can be specified");
			} 
			if (strInputFile == "" and strInputFileList == "")
			{
				_EXCEPTIONT("No input data file inputFile or inputFileList specified");
			}
			// ensure that only one of outputFile and outputFileList is specified
			if (strOutputFile != "" && strOutputFileList != "")
			{
				_EXCEPTIONT("Only one of outputFile and outputFileList can be specified");
			} 

			/* initialize vecInputFiles and vecOutputFiles
			building a unified interface for input and output files
			If in_data is specified, then the output file is specified by out_data */
			if (strInputFile != "") {	// option 1: specify InputFile
				this->vecInputFiles.push_back(strInputFile);
				this->vecOutputFiles.push_back(strOutputFile);    // strOutputFile has default value "out.dat"
			} else if (strInputFileList != "") {	// option 2: specify InputFileList
				this->vecInputFiles.FromFile(strInputFileList, true);
				this->vecOutputFiles.FromFile(strOutputFileList);
				if (this->vecOutputFiles.size() == 0) {    
					// if outputFileList is not specified, use default output file names: out000000.dat, out000001.dat, ...
					for (int f = 0; f < this->numInputFilesLines; f++) {
						char szFileIndex[32];
						snprintf(szFileIndex, 32, "%06i", f);    
						std::string strOutputFile = "out" + std::string(szFileIndex) + ".dat";
						this->vecOutputFiles.push_back(strOutputFile);
					}
				} else if (this->vecInputFiles.size() != this->vecOutputFiles.size()) { // if outputFileList is specified, ensure that the number of input files equals the number of output files
					_EXCEPTIONT("inputFileList must match outputFileList");
				}    
			} 

			this->numInputFilesLines = this->vecInputFiles.size();

			// initialize searchByMinima, ixSearchBy, strSearchByThreshold
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

			this->dcuparam.strLatitudeName = strLatitudeName;
			this->dcuparam.strLongitudeName = strLongitudeName;
			this->dcuparam.fRegional = fRegional;
			this->dcuparam.fOutputHeader = fOutputHeader;
			this->dcuparam.fOutputSeconds = fOutputSeconds;
			this->dcuparam.iVerbosityLevel = iVerbosityLevel;
			this->dcuparam.fDiagonalConnectivity = diag_connect;
			this->strConnectivity = strConnectivity;
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
			// delete the pointers
			delete this->dcuparam.pvecClosedContourOp;
			delete this->dcuparam.pvecNoClosedContourOp;
			delete this->dcuparam.pvecThresholdOp;
			delete this->dcuparam.pvecOutputOp;
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
			DetectNodesParameter class is used for configuring the parameters required for DeteNoodes function.\n
			Parameters:\n
			------------------------------------------------------------------------------------------\n
			Parameters to specify input and output files, either InputFile or InputFileList must be specified:\n
			Option 1: specify InputFile\n
			inputFile (str) [""]: A list of input data files in NetCDF format, separated by semicolons. Example: "input1.nc;input2.nc; ..."\n
			outputFile (str) ["out.dat"]): Path to the output nodefile to write from the detection procedure. Used if InputFile is specified.\n
			Option 2: specify InputFileList\n
			inputFileList (str) [""]: Path to a a text file containing the InputFile argument for a sequence of processing operations (one per line). Example: "input1.nc;input2.nc;\n input3.nc;input4.nc;\n ..."\n
			outputFileList (str) [""]: Path to a text file containingan equal number of lines to InputFileList specifying the output nodefiles from each input datafile. If not specified, the output files are named as out000000.dat, out000001.dat, ...\n
			------------------------------------------------------------------------------------------\n
			Input specification parameters\n
			latitudeName (str) ["lat"]: Name of the latitude dimension.\n
			longitudeName (str) ["lon"]: Name of the longitude dimension.\n
			------------------------------------------------------------------------------------------\n
			Output specification parameters\n
			outputHeader (bool) [False]: If True: output a header at the beginning of the output file indicating the columns of the file.\n
			outputSeconds (bool) [False]: If True: output second of the day as part of the timestamp. Otherwise, output will report hour of the day.\n
			outputCmd (List[str]) []: List of strings specifying the output commands to include additional columns in the output file. Each output command takes the form "var,op,dist". These arguments are as follows.\n
				var is the name of the variable used for output.\n
				op is the operator that is applied over all points within the specified distance of the candidate (options include max, min, avg, maxdist, mindist).\n
				dist is the great-circle distance away from the candidate wherein the operator is applied.\n
			Example: ["PRMSL_L101,max,0", "_VECMAG(U_GRD_L100,V_GRD_L100),max,4", "HGT_L1,max,0"]\n
			------------------------------------------------------------------------------------------\n
			Logfile\n
			logDir (str) ["."]: Path to the directory where the log files will be written. The log files are named as log000000.txt, log000001.txt, ...\n
			------------------------------------------------------------------------------------------\n
			connectivityFile (str) [""]: Path to a a connectivity file that describes the unstructured grid.\n
			------------------------------------------------------------------------------------------\n
			diag_connect (bool) [False]: If True: when the data is on a structured grid, consider grid cells to be connected in the diagonal (across the vertex).\n
			------------------------------------------------------------------------------------------\n
			Parameters for initially selecting candidate points (defined as local minima or local maxima).\n
			searchByMin (bool) [False]: If True: search for local minima, otherwise search for local maxima.\n
			searchBy (str) ["PSL"]: The variable to use for searching for local minima or maxima.\n
			searchByThreshold (str) [""]: The threshold to use for searching for local minima or maxima.\n
			------------------------------------------------------------------------------------------\n
			maxLatitude (float) [0.0]: The maximum latitude for candidate points. If maxLatitude and minLatitude are equal then these arguments are ignored.\n
			minLatitude (float) [0.0]: The minimum latitude for candidate points. If maxLatitude and minLatitude are equal then these arguments are ignored.\n
			minAbsLatitude (float) [0.0]: The minimum absolute value of latitude for candidate points. This argument has no effect if set to zero.\n
			maxLongitude (float) [0.0]: The maximum longitude for candidate points. As longitude is a periodic dimension, when regional is set to False (default value) minLongitude may be larger than maxLongitude. If maxLongitude and maxLongitude are equal then these arguments are ignored.\n
			minLongitude (float) [0.0]: The minimum longitude for candidate points. As longitude is a periodic dimension, when regional is set to False (default value) minLongitude may be larger than maxLongitude. If maxLongitude and maxLongitude are equal then these arguments are ignored.\n
			regional (bool) [False]: Used to indicate that a given latitude-longitude grid should not be periodic in the longitudinal direction.\n
			------------------------------------------------------------------------------------------\n
			mergeDist (float) [0.0]: DetectNodes merges candidate points with a distance (in degrees great-circle-distance) shorter than the specified value. Among two candidates within the merge distance, only the candidate with the lowest(if searchByMin=True)/highest(if searchByMin=False) value of the searchBy field are retained.\n
			------------------------------------------------------------------------------------------\n
			timeStride (int) [1]: [Deprecated] Only examine discrete times at the given stride. Consider --timefilter instead.\n
			timeFilter (str) [""]: A regular expression used to match only those time values to be retained. Several default values are available as follows.\n
				"3hr": filter every 3 hourly (equivalent to "....-..-.. (00|03|06|09|12|15|18|21):00:00").\n
				"6hr": filter every 6 hourly (equivalent to "....-..-.. (00|06|12|18):00:00").\n
				"daily": filter daily (equivalent to "....-..-.. 00:00:00").\n
			------------------------------------------------------------------------------------------\n
			Eliminates candidates\n
			closedContourCmd (List[str]) []: List of strings specifying the closed contour commands to eliminate candidates. Eliminate candidates if they do not have a closed contour.\n
			The closed contour is determined by breadth first search: if any paths exist from the candidate point (or nearby minima/maxima if minmaxdist is specified) that reach the specified distance before achieving the specified delta then we say no closed contour is present.\n
			Each closed contour command takes the form "var,delta,dist,minmaxdist". These arguments are as follows.\n
				var is the name of the variable used for the contour search.\n
				dist is the great-circle distance (in degrees) from the pivot within which the closedcontour criteria must be satisfied.\n
				delta is the amount by which the field must change from the pivot value. If positive (negative) the field must increase (decrease) by this value along the contour.\n
				minmaxdist is the great-circle distance away from the candidate to search for the minima/maxima. If delta is positive (negative), the pivot is a local minimum (maximum).\n
			Example: ["PRMSL_L101,200.,4,0", "TMP_L100,-0.4,8.0,1.1"]\n
			-\n
			noClosedContourCmd (List[str]) []: List of strings specifying the no closed contour commands. As closedContourCmd, except it eliminates candidates if a closed contour is present.\n
			Example: ["PRMSL_L101,200.,4,0", "TMP_L100,-0.4,8.0,1.1"]
			-\n
			thresholdCmd (List[str]) []: List of strings specifying the threshold commands to eliminate candidates. Eliminate candidates that do not satisfy a threshold criteria (there must exist a point within a given distance of the candidate that satisfies a given equality or inequality).\n
			Search is performed by breadth-first search over the grid. Each threshold command takes the form "var,op,value,dist". These arguments are as follows.\n
				var is the name of the variable used for the thresholding.\n
				op is the operator that must be satisfied for threshold (options include >,>=,<,<=,=,!=).\n
				value is the value on the right-hand-side of the comparison.\n
				dist is the great-circle distance away from the candidate to search for a point that satisfies the threshold.\n
			Example: ["PRMSL_L101,>,100000,4", "TMP_L100,<=,273.15,8.0"]\n
			------------------------------------------------------------------------------------------\n
			verbosityLevel (int) [0]: Verbosity level of execution.\n
		)pbdoc")
        .def(
			// lambda function to initialize the DetectNodesParameter class
			// arguements are ordered as in the class documentation above
			py::init([]
			(
				const std::string& strInputFile,
				const std::string& strOutputFile,
				const std::string& strInputFileList,
				const std::string& strOutputFileList,

				const std::string& strLatitudeName,
				const std::string& strLongitudeName,

				bool fOutputHeader,
				bool fOutputSeconds,
				py::list outputCmd,

				const std::string& strLogDir,

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
				bool fRegional,

				double dMergeDist,

				int nTimeStride,
				const std::string& strTimeFilter,

				py::list closedContourCmd,
				py::list noClosedContourCmd,
				py::list thresholdCmd,

				int iVerbosityLevel
			)-> DetectNodesParameter
			{
				// pass python arguments to the cpp class constructor to initialize the class DetectNodesParameter
				DetectNodesParameter param = DetectNodesParameter(
					strInputFile,
					strOutputFile,
					strInputFileList,
					strOutputFileList,
					strConnectivity,
					diag_connect,
					searchByMin,
					strSearchBy,
					strSearchByThreshold,
					dMaxLatitude,
					dMinLatitude,
					dMinAbsLatitude,
					dMaxLongitude,
					dMinLongitude,
					dMergeDist,
					nTimeStride,
					strTimeFilter,
					strLatitudeName,
					strLongitudeName,
					fRegional,
					fOutputHeader,
					fOutputSeconds,
					iVerbosityLevel,
					strLogDir);

				// parse the outputCmd, closedContourCmd, noClosedContourCmd, and thresholdCmd commands and set the corresponding parameters in DetectNodesParameter
				param.dcuparam.pvecClosedContourOp = parseOperations<ClosedContourOp>(closedContourCmd, param.varreg);
				param.dcuparam.pvecNoClosedContourOp = parseOperations<ClosedContourOp>(noClosedContourCmd, param.varreg);
				param.dcuparam.pvecThresholdOp = parseOperations<ThresholdOp>(thresholdCmd, param.varreg);
				param.dcuparam.pvecOutputOp = parseOperations<NodeOutputOp>(outputCmd, param.varreg);

				return param;
			}),
			// arguments
            py::arg("inputFile") = "",
            py::arg("outputFile") = "out.dat",
            py::arg("inputFileList") = "",
			py::arg("outputFileList") = "",

			py::arg("latitudeName") = "lat",
            py::arg("longitudeName") = "lon",

            py::arg("outputHeader") = false,
            py::arg("outputSeconds") = false,
			py::arg("outputCmd") = py::list(),

			py::arg("logDir") = ".",

            py::arg("connectivityFile") = "",

            py::arg("diagConnect") = false,

            py::arg("searchByMin") = false,
            py::arg("searchBy") = "PSL",
            py::arg("searchByThreshold") = "",

			//double types are converted to float in python
            py::arg("maxLatitude") = 0.0,
            py::arg("minLatitude") = 0.0,
            py::arg("minAbsLatitude") = 0.0,
            py::arg("maxLongitude") = 0.0,
            py::arg("minLongitude") = 0.0,
            py::arg("regional") = false,

            py::arg("mergeDist") = 0.0,

            py::arg("timeStride") = 1, //integer
            py::arg("timeFilter") = "",
            
			py::arg("closedContourCmd") = py::list(),
			py::arg("noClosedContourCmd") = py::list(),
			py::arg("thresholdCmd") = py::list(),

            py::arg("verbosityLevel") = 0, //TODO: announcement in DetectNodes.main

			// docstring
			"Constructor for DetectNodesParameter class." //TODO: revise docstring
		)

		.def("__repr__", [](const DetectNodesParameter &param) {
			std::stringstream repr;
			
			repr << "You have initialized the DetectNodesParameter class with the following parameters:\n";

			// input and output files
			for (size_t i = 0; i < param.numInputFilesLines; i++)
			{
				repr << "InputFile: " << param.vecInputFiles[i] << "\n";
				repr << "OutputFile: " << param.vecOutputFiles[i] << "\n";
				repr << "Logfile: " << param.vecLogFiles[i] << "\n";
			}

			// lat and lon names
			repr << "Latitude Name: " << param.dcuparam.strLatitudeName << "\n";
			repr << "Longitude Name: " << param.dcuparam.strLongitudeName << "\n";

			// output specification parameters
			repr << "Output Header: " << (param.dcuparam.fOutputHeader ? "True" : "False") << "\n";
			repr << "Output Seconds: " << (param.dcuparam.fOutputSeconds ? "True" : "False") << "\n";
			// TODO: print outputCmd
			// repr << "Output Commands: \n";
			// repr << "var,op,dist\n";
			// for (const auto& op : *param.dcuparam.pvecOutputOp) {
			// 	repr << op.ToString() << "\n";
			// }

			// connectivity file
			repr << "Connectivity File: " << param.strConnectivity << "\n";

			// diag_connect
			repr << "Diagonal Connectivity: " << (param.dcuparam.fDiagonalConnectivity ? "True" : "False") << "\n";

			// searchByMin, searchBy, searchByThreshold
			if (param.dcuparam.fSearchByMinima) {
				repr << "Searching By Minima\n";
			} else {
				repr << "Searching By Maxima\n";
			}
			repr << "Search By: " << param.dcuparam.ixSearchBy << "\n";
			repr << "Search By Threshold: " << param.dcuparam.strSearchByThreshold << "\n";

			// maxLatitude, minLatitude, minAbsLatitude, maxLongitude, minLongitude, regional
			repr << "Max Latitude: " << param.dcuparam.dMaxLatitude << "\n";
			repr << "Min Latitude: " << param.dcuparam.dMinLatitude << "\n";
			repr << "Min Abs Latitude: " << param.dcuparam.dMinAbsLatitude << "\n";
			repr << "Max Longitude: " << param.dcuparam.dMaxLongitude << "\n";
			repr << "Min Longitude: " << param.dcuparam.dMinLongitude << "\n";
			repr << "Regional: " << (param.dcuparam.fRegional ? "True" : "False") << "\n";

			// mergeDist
			repr << "Merge Distance: " << param.dcuparam.dMergeDist << "\n";

			// timeStride, timeFilter
			if (param.dcuparam.nTimeStride != 1) {
				repr << "[Deprecated] Time Stride: " << param.dcuparam.nTimeStride << "\n";
			} else {
				repr << "Time Filter: " << param.dcuparam.strTimeFilter << "\n";
			}

			// TODO: print closedContourCmd, noClosedContourCmd, thresholdCmd, how to extract the information from pointer?
			// closedContourCmd, noClosedContourCmd, thresholdCmd
			// repr << "Closed Contour Commands: \n";
			// repr << "var,delta,dist,minmaxdist\n";
			// for (const auto& op : *param.dcuparam.pvecClosedContourOp) {
			// 	repr << op.ToString() << "\n";
			// }
			// repr << "No Closed Contour Commands: \n";
			// repr << "var,delta,dist,minmaxdist\n";
			// for (const auto& op : *param.dcuparam.pvecNoClosedContourOp) {
			// 	repr << op.ToString() << "\n";
			// }
			// repr << "Threshold Commands: \n";
			// repr << "var,op,value,dist\n";
			// for (const auto& op : *param.dcuparam.pvecThresholdOp) {
			// 	repr << op.ToString() << "\n";
			// }

			// verbosityLevel
			repr << "Verbosity Level: " << param.dcuparam.iVerbosityLevel << "\n";

			return repr.str();
		})
		;


	// main function calling DetectCyclonesUnstructured for each file in DetectNodesParameter.vecInputFiles
	m.def("DetectNodes", 
		[](DetectNodesParameter& param){
			// Turn off fatal errors in NetCDF
			NcError error(NcError::silent_nonfatal);

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