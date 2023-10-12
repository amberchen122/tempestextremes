from glob import glob
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

__version__ = "0.0.1"

TEMPEST_EXTREMES_ROOT = "../src"

ext_modules = [
    Pybind11Extension(
        "DetectNodes",
        ["src/DetectNodes/detect_nodes_binding.cpp"] \
            + sorted(glob(TEMPEST_EXTREMES_ROOT + "/base/*.cpp", recursive=True)) \
            + sorted(glob(TEMPEST_EXTREMES_ROOT + "/netcdf-cxx-4.2/*.cpp", recursive=True)),
        language='c++',
        include_dirs = sorted(glob(TEMPEST_EXTREMES_ROOT + '/**/', recursive=True)),
        define_macros = [('VERSION_INFO', __version__)],
        libraries = ['netcdf'],
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
    # Currently, build_ext only provides an optional "highest supported C++
    # level" feature, but in the future it may provide more features.
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)