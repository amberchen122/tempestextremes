import sys
from pathlib import Path

# Get the absolute path of the parent directory
parent_dir = Path(__file__).resolve().parent.parent

# Add the parent directory to sys.path
sys.path.insert(0, str(parent_dir))

from TempestExtremes import *

# Original command line:
cmd = '--in_data ./test/cn_files/outCSne30_test2.nc --timefilter "6hr" --out out1.dat --searchbymin MSL --closedcontourcmd "MSL,200.0,5.5,0;_DIFF(Z(300hPa),Z(500hPa)),-58.8,6.5,1.0" --mergedist 6.0 --outputcmd "MSL,min,0;_VECMAG(VAR_10U,VAR_10V),max,2;ZS,min,0" --latname lat --lonname lon'
print(cmd)
print("----------------------------------------")
# Call the function with a dictionary of key-value pairs, key is the option name, value is the option value
cmd_dict = {
    "--in_data": "./test/cn_files/outCSne30_test2.nc",
    "--timefilter": '"6hr"',
    "--out": "out1.dat",
    "--searchbymin": "MSL",
    "--closedcontourcmd": "PRMSL_L101,200.,4,0;TMP_L100,-0.4,8.0,1.1",
    "--mergedist": "6.0",
    "--outputcmd": "MSL,min,0;_VECMAG(VAR_10U,VAR_10V),max,2;ZS,min,0",
    "--latname": "lat",
    "--lonname": "lon",
}
print(cmd_dict)
print("----------------------------------------")
DetectNodes(cmd_dict)
print("----------------------------------------")
StitchNodes(cmd_dict)
