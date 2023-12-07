import sys
from pathlib import Path

# Get the absolute path of the parent directory
parent_dir = Path(__file__).resolve().parent.parent

# Add the parent directory to sys.path
sys.path.insert(0, str(parent_dir))

from TempestExtremes import *

# Original command line:
cmd = '--in_data python-sdk/test/mslp_test.nc --out detected_nodes.txt --searchbymin msl --closedcontourcmd "msl,100,5.5,0"  --mergedist 6.0 --outputcmd "msl,min,0;msl,posclosedcontour,5.5,0" --timefilter "3hr" --latname "latitude" --lonname "longitude" --verbosity 1'
print(cmd)
print("----------------------------------------")
# Call the function with a dictionary of key-value pairs, key is the option name, value is the option value
cmd_dict = {
    "--in_data": "test/mslp_test.nc",
    "--out": "detected_nodes.txt",
    "--searchbymin": "msl",
    "--closedcontourcmd": 'msl,100,5.5,0',
    "--mergedist": "6.0",
    "--outputcmd": 'msl,min,0;msl,posclosedcontour,5.5,0',
    "--timefilter": '"3hr"',
    "--latname": "latitude",
    "--lonname": "longitude",
    "--verbosity": "1"
}
print(cmd_dict)
print("----------------------------------------")
DetectNodes(cmd_dict)
