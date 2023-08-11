#!/bin/bash
cpus=$(grep -c processor /proc/cpuinfo)

mkdir build
cd build || exit
cmake .. -DCMAKE_BUILD_TYPE=RELEASE || exit
MSBuild.exe UVKBuildTool.sln -property:Configuration=Release -property:Platform=x64 -property:maxCpuCount="${cpus}" || make -j "${cpus}" || exit
