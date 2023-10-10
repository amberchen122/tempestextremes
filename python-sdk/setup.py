# Available at setup time due to pyproject.toml
from glob import glob
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

__version__ = "0.0.1"

ext_modules = [
    Pybind11Extension("TempestExtremes",
        sorted(glob("src/*.cpp")) + sorted(glob("../src/*.cpp")) + sorted(glob("../src/*.h")), # added ../src/*.cpp to the list of source files
        define_macros = [('VERSION_INFO', __version__)],
        ),
]

setup(
    name="TempestExtremes",
    version=__version__,
    author="Shujuan Chen",
    author_email="csjchen@ucdavis.edu",
    url="",
    description="",
    long_description="",
    ext_modules=ext_modules,
    # extras_require={"test": "pytest"},
    # Currently, build_ext only provides an optional "highest supported C++
    # level" feature, but in the future it may provide more features.
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)