import sys
from pathlib import Path

# Get the absolute path of the parent directory
parent_dir = Path(__file__).resolve().parent.parent

# Add the parent directory to sys.path
sys.path.insert(0, str(parent_dir))

import DetectNodes

# Test the following command:
# bin/DetectNodes --in_data test/cn_files/e5.oper.fc.sfc.instan.228_003_zust.ll025sc.2023061606_2023070106.nc 
# --timefilter "6hr" --out out1.dat --searchbymin MSL 
# --closedcontourcmd "MSL,200.0,5.5,0;_DIFF(Z(300hPa),Z(500hPa)),-58.8,6.5,1.0" 
# --mergedist 6.0 
# --outputcmd "MSL,min,0; _VECMAG(VAR_10U,VAR_10V),max,2;ZS,min,0"

"""bin/DetectNodes --in_data ./python-sdk/test/cn_files/e5.oper.an.sfc.128_015_aluvp.ll025sc.2023070100_2023073123.nc --timefilter "6hr" --out out1.dat 
--searchbymin MSL --closedcontourcmd "MSL,200.0,5.5,0;_DIFF(Z(300hPa),Z(500hPa)),-58.8,6.5,1.0" --mergedist 6.0 --outputcmd "MSL,min,0; _VECMAG(VAR_10U,VAR_10V),max,2;ZS,min,0" """


# Create a DetectNodes.DetectCyclonesParam object
param = DetectNodes.DetectNodesParameter(strInputFile = "./test/cn_files/e5.oper.an.sfc.128_015_aluvp.ll025sc.2023070100_2023073123.nc", 
                                         strOutputFile = "out.dat", searchByMin = True, strSearchBy = "MSL", strTimeFilter = "6hr", strLatitudeName = "lat", 
                                         strLongitudeName = "lon", dMergeDist = 6.0,
                                        )

print(param)

DetectNodes.DetectNodes(param)


