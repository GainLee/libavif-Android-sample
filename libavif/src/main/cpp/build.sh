#!/bin/bash

# This script will build libgav1 for the default ABI targets supported by
# android. You must pass the path to the android NDK as a parameter to this
# script.
#
# Android NDK: https://developer.android.com/ndk/downloads
#
# The git tag below is known to work, and will occasionally be updated. Feel
# free to use a more recent commit.

if [ $# -ne 2 ]; then
  echo "Usage: ${0} <path_to_android_ndk> <path_to CmakeList.txt>"
  exit 1
fi

cd ${2}

mkdir build
cd build

ABI_LIST="armeabi-v7a arm64-v8a x86 x86_64"
for abi in ${ABI_LIST}; do
  mkdir "${abi}"
  cd "${abi}"
  cmake ../.. \
    -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=${1}/build/cmake/android.toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DANDROID_ABI=${abi}
  ninja
  cd ..
done

