# SW II - Game Engine
Just to have some fun with C++ and OpenGL

<p align="center">
  <img src="./docs/icon.png" width="300">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white" width="100">
  <img src="https://img.shields.io/badge/Visual%20Studio-5C2D91.svg?style=for-the-badge&logo=visual-studio&logoColor=white" width="210">
  <img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" width="156">
</p>

# Getting Started
On Windows Visual Studio 2022 is recommended. On linux tested on Ubuntu 22.04 LTS with clang compiler.

## <ins>**1. Downloading the repository:**</ins>

Start by cloning the repository with `git clone --recursive https://github.com/Tycjan-Fortuna-IT/SW_II.git`.

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

## <ins>**2. Generating solution:**</ins>

- `Windows` - Run the [Setup-Windows-VS2022.bat](https://github.com/Tycjan-Fortuna-IT/SW_II/blob/master/Setup-Windows-VS2022.bat) file. This will (using premake5) generate all neccessary projects, folder structure, links and solution automatically.
- `Linux` - Run the [Setup-Linux-Clang.sh](https://github.com/Tycjan-Fortuna-IT/SW_II/blob/master/Setup-Linux-Clang.sh) file. This ask and download all required libraries to compile and run the engine, also will install premake5 if not present. Follow the appearing instructions. When everything is set running the script again will (using premake5) generate all neccessary projects, folder structure, links and solution automatically.

## <ins>**3. Building engine for production:**</ins>

Build the engine in x64 Release mode to compile all files and create static library (.lib).

## <ins>**4. Generating documentation:**</ins>

Additional configuration:
- `language of the documentation` - by default it is set to english, but you can change it to english by editing [Doxyfile](/docs/Doxyfile) file and changing `OUTPUT_LANGUAGE` to `some_supported_language`.

Run the [Generate-Documentation.bat](https://github.com/Tycjan-Fortuna-IT/SW_II/blob/master/Generate-Documentation.bat) file. This will (using doxygen) generate the documentation that will be awailable inside [index.html](/docs/html/index.html) file.
