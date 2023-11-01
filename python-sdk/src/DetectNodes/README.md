
# Design

DetectNodesParameter is responsible for taking in "command line inputs"(function arguements in python) and use them to initialize properties of and varreg(), dcuparam()

DetectNodes function takes in DetectNodesParameter. It also take care of other configurations in DetectNodes.cpp/main. e.g.:

`
(NcError::silent_nonfatal); // Turn off fatal errors in NetCDF NcError error
`

# To compile

`cd python-sdk`

`python setup.py build_ext --inplace`

# To test

`python ./test/detect_nodes_test.py`

# List of DetectNodes available commands and their python arguements correspondance

Note: Descriptions for the DetectNodes commands can be found on the [TE website](https://climate.ucdavis.edu/tempestextremes.php#DetectNodes) and are also included in the `py::class<DetectNodesParameter>`  description.

- --command_line `<type>` [default_value]
  - `pythonParameterName = default_value` (`type`)
<!---------------------------------------------------->
- --in_data `<string>` [""]  
  - `str inputFile= ""` (`str`)
- --out `<string>` ["out.dat"]
  - `outputFile = "out.dat"` (`str`)
- --in_data_list `<string>` [""]
  - `inputFileList = ""` (`str`)
- --out_file_list `<string>` [""]
  - `outputFileList = ""` (`str`)
  - If not specified, the output files are named as out000000.dat, out000001.dat, ...'
<!---------------------------------------------------->
- --latname `<string>` ["lat"]
  - `latitudeName = "lat"` (`str`)
- --lonname `<string>` ["lon"]
  - `longitudeName = "lon"` (`str`)
<!---------------------------------------------------->
- --out_header `<bool>` [false]
  - `outputHeader = false` (`bool`)
- --out_seconds `<bool>` [false]
  - `outputSeconds = false` (`bool`)
- --outputcmd `<string>` [""] [var,op,dist;...]
  - `outputCmd = []` (`List[str]`)
  - Example:
    - cpp command line: "\<cmd1>;\<cmd2>;..."
            cmd = var,op,dist
            "PRMSL_L101,max,0;_VECMAG(U_GRD_L100,V_GRD_L100),max,4;HGT_L1,max,0"
        vs
    - python list input: `["PRMSL_L101,max,0", "_VECMAG(U_GRD_L100,V_GRD_L100),max,4", "HGT_L1,max,0"]`
<!---------------------------------------------------->
- --logdir `<string>` ["."]
  - `logDir = "."` (`str`)
<!---------------------------------------------------->
- --in_connect `<string>` [""]
  - `connectivityFile = ""` (`str`)
<!---------------------------------------------------->
- --diag_connect `<bool>` [false]
  - `diagConnect = false` (`bool`)
<!---------------------------------------------------->
- searchBy arguements
  - Original command lines:
    - --searchbymin `<string>` [""]
    - --searchbymax `<string>` [""]
    - --searchbythreshold `<string>` [""]
  - Changed to the following in python:
    - `searchByMin = False` (`bool`)
    - `searchBy = "PSL"` (`str`)
    - `searchbythreshold = ""` (`str`)
<!---------------------------------------------------->
- Candidate points selection criteria. Note: C++ arguement in `<double>` will be converted to/from Python's `<float>` by `pybind11`.

  - --maxlat `<double>` [0.000000] (degrees)
    - `maxLatitude = 0.0` (`bool`)
  - --minlat `<double>` [0.000000] (degrees)
    - `minLatitude = 0.0` (`bool`)
  - --minabslat `<double>` [0.000000] (degrees)
    - `minAbsLatitude = 0.0` (`bool`)
  - --maxlon `<double>` [0.000000] (degrees)
    - `maxLongitude = 0.0` (`bool`)
  - --minlon `<double>` [0.000000] (degrees)
    - `minLongitude = 0.0` (`bool`)
  - --mergedist `<double>` [0.000000] (degrees)
    - `mergeDist = 0.0` (`bool`)
  - --regional `<bool>` [false]
    - `regional = false` (`bool`)
<!---------------------------------------------------->
- --timestride `<integer>` [1]
  - `timeStride = 1` (`int`)
- --timefilter `<string>` ["6hr"]
  - `timeFilter = ""` (`str`)
<!---------------------------------------------------->
- --closedcontourcmd `<string>` [""] [var,delta,dist,minmaxdist;...]
  - `closedcontourcmd = []` (`List[str]`)
  - Example:
    - cpp command line: "\<cmd1>;\<cmd2>;..."
            cmd = var,delta,dist,minmaxdist
            `PRMSL_L101,200.,4,0;TMP_L100,-0.4,8.0,1.1`
        vs
    - python list input: `["PRMSL_L101,200.,4,0", "TMP_L100,-0.4,8.0,1.1"]`
- --noclosedcontourcmd `<string>` [""] [var,delta,dist,minmaxdist;...]
  - `noClosedcontourcmd = []` (`List[str]`)
  - Example:
    - cpp command line: "\<cmd1>;\<cmd2>;..."
            cmd = var,delta,dist,minmaxdist
            `PRMSL_L101,200.,4,0;TMP_L100,-0.4,8.0,1.1`
        vs
    - python list input: `["PRMSL_L101,200.,4,0", "TMP_L100,-0.4,8.0,1.1"]`
- --thresholdcmd `<string>` [""] [var,op,value,dist;...]
  - `thresholdCmd = []` (`List[str]`)
  - Example:
    - cpp command line: "\<cmd1>;\<cmd2>;..."
            cmd = var,op,value,dist
            "PRMSL_L101,>,100000,4; TMP_L100,<=,273.15,8.0"
        vs
    - python list input: `["PRMSL_L101,>,100000,4", "TMP_L100,<=,273.15,8.0"]`
- --verbosity `<integer>` [0]
  - `verbosityLevel = 0` (`int`)

# `DetectNodesParameter` Class 

This class is used for configuring the parameters required for `DeteNoodes` function.

## Parameters to specify input and output files
_Either `InputFile` or `InputFileList` must be specified._

**Option 1: Specify `InputFile`**
- `inputFile` (str) [""]: 
  - A list of input data files in NetCDF format, separated by semicolons.
  - Example: `input1.nc;input2.nc;...` 
- `outputFile` (str) ["out.dat"]:
  - Path to the output nodefile to write from the detection procedure.
  - Used if `InputFile` is specified.

**Option 2: Specify `InputFileList`**
- `inputFileList` (str) [""]:
  - Path to a text file containing the `InputFile` argument for a sequence of processing operations (one per line).
  - Example: `input1.nc;input2.nc;\n input3.nc;input4.nc;\n...`
- `outputFileList` (str) [""]:
  - Path to a text file containing an equal number of lines to `InputFileList` specifying the output nodefiles from each input datafile.
  - If not specified, the output files are named as `out000000.dat`, `out000001.dat`, ...

## Input specification parameters
- `latitudeName` (str) ["lat"]: Name of the latitude dimension.
- `longitudeName` (str) ["lon"]: Name of the longitude dimension.

## Output specification parameters
- `outputHeader` (bool) [False]: If True, output a header at the beginning of the output file indicating the columns of the file.
- `outputSeconds` (bool) [False]: If True, output second of the day as part of the timestamp. Otherwise, output will report hour of the day.
- `outputCmd` (List[str]) []:
  - List of strings specifying the output commands to include additional columns in the output file.
  - Example: `["PRMSL_L101,max,0", "_VECMAG(U_GRD_L100,V_GRD_L100),max,4", "HGT_L1,max,0"]`

## Logfile 
- `logDir` (str) ["."]: Path to the directory where the log files will be written. The log files are named as `log000000.txt`, `log000001.txt`, ...

## Additional Parameters
- `connectivityFile` (str) [""]: Path to a connectivity file that describes the unstructured grid.
- `diag_connect` (bool) [False]: If True, when the data is on a structured grid, consider grid cells to be connected in the diagonal (across the vertex).

## Parameters for initially selecting candidate points
(Defined as local minima or local maxima)
- `searchByMin` (bool) [False]: If True, search for local minima, otherwise search for local maxima.
- `searchBy` (str) ["PSL"]: The variable to use for searching for local minima or maxima.
- `searchByThreshold` (str) [""]: The threshold to use for searching for local minima or maxima. 
  - **TODO**: Describe the default behavior if this is not specified.

## Geographic Parameters
- `maxLatitude` (float) [0.0], `minLatitude` (float) [0.0], `minAbsLatitude` (float) [0.0]
- `maxLongitude` (float) [0.0], `minLongitude` (float) [0.0]
- `regional` (bool) [False]: Used to indicate that a given latitude-longitude grid should not be periodic in the longitudinal direction.

## Advanced Parameters
- `mergeDist` (float) [0.0]: 
  - DetectNodes merges candidate points with a distance (in degrees great-circle-distance) shorter than the specified value.
  - Among two candidates within the merge distance, only the candidate with the lowest(if `searchByMin`=True)/highest(if `searchByMin`=False) value of the `searchBy` field are retained.

## Time Parameters
- `timeStride` (int) [1]: **Deprecated** Only examine discrete times at the given stride. Consider `--timefilter` instead.
- `timeFilter` (str) [""]:
  - A regular expression used to match only those time values to be retained. 
  - Example: `"3hr"`, `"6hr"`, `"daily"`

## Eliminate Candidates
Detailed parameters related to eliminating candidates based on contour commands and threshold commands are provided.

## Verbosity
- `verbosityLevel` (int) [0]: Verbosity level of execution.


<!-- TODO -->
print(DetectNodesParameter) should print

- input files / input files list
- output file / output file list
- log file(s)
