"""
This module contains the setup script for the TempestExtremes package. 
It defines the package metadata, the extension modules, and the custom build command.
"""

from glob import glob
import os
from pybind11.setup_helpers import Pybind11Extension
from setuptools import setup
from setuptools.command.build_ext import build_ext

def read(fname):
    """
    Utility function to read the README file.

    Args:
        fname (str): The name of the file to read.

    Returns:
        str: The contents of the file.
    """
    return open(os.path.join(os.path.dirname(__file__), fname), encoding='utf-8').read()

def get_version():
    """
    Returns the version number from the VERSION file.

    :return: The version number as a string.
    """
    with open('VERSION', encoding='utf-8') as version_file:
        return version_file.read().strip()

def collect_sources(root_dir, sub_dirs):
    """
    Collects all the source files with the extension '.cpp' in the specified subdirectories.

    Args:
        root_dir (str): The root directory to search for source files.
        sub_dirs (list): A list of subdirectories to search for source files.

    Returns:
        list: A list of all the source files found in the specified subdirectories.
    """
    sources = []
    for sub_dir in sub_dirs:
        sources.extend(sorted(glob(f"{root_dir}/{sub_dir}/*.cpp", recursive=True)))
    return sources

__version__ = get_version()

TEMPEST_EXTREMES_ROOT = "../src"
UTILS_SRC = collect_sources(TEMPEST_EXTREMES_ROOT, ["base", "netcdf-cxx-4.2"])
INCLUDE_DIRS = glob(TEMPEST_EXTREMES_ROOT + "/**/", recursive=True)

modules = {
    "DetectNodes": "detect_nodes_binding.cpp",
    "StitchNodes": "stitch_nodes_binding.cpp",
    # Add other modules here
}

ext_modules = [
    Pybind11Extension(
        f"{module_name}",
        [f"src/{src}"] + UTILS_SRC,
        language="c++",
        include_dirs=INCLUDE_DIRS,
        define_macros=[("VERSION_INFO", __version__)],
        libraries=["netcdf"],
        extra_compile_args=["-w", "-std=c++14"],
    ) for module_name, src in modules.items()
]

setup(
    name="python-sdk/setup.py",
    version=__version__,
    author="Shujuan (Amber) Chen",
    author_email="csjchen@ucdavis.edu",
    url="",  #TODO Add package URL here
    description="",  #TODO Add a short description here
    long_description=read('README.md'),
    long_description_content_type='text/markdown',
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
    install_requires=read('requirements.txt').splitlines(),  #TODO Revise requirements.txt
    # include_package_data=True,
    # packages=find_packages(),
    # license="Your License",  #TODO Specify license
)