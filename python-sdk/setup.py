from glob import glob
from pybind11.setup_helpers import Pybind11Extension, build_ext as build_pybind11
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as setuptools_build_ext
import os

class build_ext(setuptools_build_ext):
    def build_extensions(self):
        # We're setting these environment variables just for the build, so
        # it uses mpicxx to compile the extension.
        os.environ['CC'] = 'mpicxx'
        os.environ['CXX'] = 'mpicxx'
        super().build_extensions()

__version__ = "0.0.1"

TEMPEST_EXTREMES_ROOT = "../src"

ext_modules = [
    Pybind11Extension(
        "DetectNodes",
        ["src/DetectNodes/detect_nodes_binding.cpp"] \
            + sorted(glob(TEMPEST_EXTREMES_ROOT + "/base/*.cpp", recursive=True)) \
            + sorted(glob(TEMPEST_EXTREMES_ROOT + "/netcdf-cxx-4.2/*.cpp", recursive=True)),
        language='c++',
        include_dirs=sorted(glob(TEMPEST_EXTREMES_ROOT + '/**/', recursive=True)),
        define_macros=[('VERSION_INFO', __version__)],
        libraries=['netcdf'],
        extra_compile_args=['-w', '-lnetcdf'],
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
    cmdclass={"build_ext": build_ext},  # Use our custom class
    zip_safe=False,
    python_requires=">=3.7",
)
