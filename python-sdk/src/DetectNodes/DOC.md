
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

print(DetectNodesParameter) should print

- input files / input files list
- output file / output file list
- log file(s)
