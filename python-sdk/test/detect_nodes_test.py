import sys
from pathlib import Path

# Get the absolute path of the parent directory
parent_dir = Path(__file__).resolve().parent.parent

# Add the parent directory to sys.path
sys.path.insert(0, str(parent_dir))

import DetectNodes

# Test the following command:

"""bin/DetectNodes --in_data ./python-sdk/test/cn_files/outCSne30_test2.nc --timefilter "6hr" --out out1.da
t --searchbymin MSL --closedcontourcmd "MSL,200.0,5.5,0;_DIFF(Z(300hPa),Z(500hP
a)),-58.8,6.5,1.0" --mergedist 6.0 --outputcmd "MSL,min,0; _VECMAG(VAR_10U,VAR_
10V),max,2;ZS,min,0" --latname lat --lonname lon"""



# Create a DetectNodes.DetectCyclonesParam object
param = DetectNodes.DetectNodesParameter(
    inputFile="./test/cn_files/outCSne30_test2.nc",
    outputFile="out.dat",
    searchByMin=True,
    searchBy='MSL',
    timeFilter='6hr',
    latitudeName='lat',
    longitudeName='lon',
    mergeDist=6.0
)


print(param)

DetectNodes.DetectNodes(param)


