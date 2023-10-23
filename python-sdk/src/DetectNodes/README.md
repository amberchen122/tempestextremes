Arguments:
- --command_line <type> [default_value]    
    - python parameter
- --in_data <string> [""]  
    - inputFile= ""
- --out <string> ["out.dat"] 
    - outputFile = "out.dat"
- --in_data_list <string> [""] 
    - inputFileList = ""
- --out_file_list <string> [""] 
    - outputFileList = ""
    - If not specified, the output files are named as out000000.dat, out000001.dat, ...
- --logdir <string> ["."] 
    - logDir = "."
- --in_connect <string> [""] 
    - connectivityFile = ""
- --diag_connect <bool> [false] 
    - diagConnect = false
- searchBy arguements
    - Original command lines:
        - --searchbymin <string> [""]
        - --searchbymax <string> [""]
        - --searchbythreshold <string> [""] 
    - Changed to the following in python:
        - searchByMin <bool> [False]
        - searchBy <string> ["PSL"]
        --searchbythreshold <string> [""] 

--minlon <double> [0.000000] (degrees)

--maxlon <double> [0.000000] (degrees)

--minlat <double> [0.000000] (degrees)

--maxlat <double> [0.000000] (degrees)

--minabslat <double> [0.000000] (degrees)

--mergedist <double> [0.000000] (degrees)


--closedcontourcmd <string> [""] [var,delta,dist,minmaxdist;...]

--noclosedcontourcmd <string> [""] [var,delta,dist,minmaxdist;...]

--thresholdcmd <string> [""] [var,op,value,dist;...]

--outputcmd <string> [""] [var,op,dist;...]

--timestride <integer> [1] 

--timefilter <string> ["6hr"] 

--latname <string> ["lat"] 

--lonname <string> ["lon"] 

--regional <bool> [false] 

--out_header <bool> [false] 

--out_seconds <bool> [false] 



--verbosity <integer> [0]



print(DetectNodesParameter) should print 

- input files / input files list
- output file / output file list
- log file(s) 