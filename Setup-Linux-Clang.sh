#!/bin/bash

if ! command -v git || ! command -v make || ! command -v clang || \
    ! dpkg -l | grep -q 'build-essential' || ! dpkg -l | grep -q 'libgl1-mesa-dev' || \
    ! dpkg -l | grep -q 'libgtk-3-dev' || ! dpkg -l | grep -q 'libxrandr-dev' || \
    ! dpkg -l | grep -q 'libxinerama-dev' || ! dpkg -l | grep -q 'libxcursor-dev' || \
    ! dpkg -l | grep -q 'libxi-dev' || ! dpkg -l | grep -q 'libxext-dev'; then
    echo "Some required libraries are missing. Do you want to install them? (y/n)"
    read -r answer
    if [ "$answer" == "y" ]; then
        sudo apt install -y git make build-essential clang libgl1-mesa-dev \
                                libgtk-3-dev libxrandr-dev libxinerama-dev \
                                libxcursor-dev libxi-dev libxext-dev
    else
        echo "Please install the required libraries manually and run the script again."
        exit 1
    fi
fi

if ! command -v premake5 > /dev/null 2>&1; then
    echo "Premake not found. Trying to download and install!"

    wget https://github.com/premake/premake-core/releases/download/v5.0.0-beta2/premake-5.0.0-beta2-linux.tar.gz
    tar -xzf premake-5.0.0-beta2-linux.tar.gz
    sudo mv premake5 /usr/local/bin/
    rm premake-5.0.0-beta2-linux.tar.gz
    rm example.so
    rm libluasocket.so

    echo "Premake has been downloaded and installed. Run the script again to prepare the build system!"
else
    echo "Premake found. Starting..."

    premake5 gmake2 --cc=clang --file=Build.lua 
fi
