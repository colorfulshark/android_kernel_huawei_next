#!/bin/bash

CURDIR=$(pwd)
export PATH=$PATH:${CURDIR}/android-ndk-r14b/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin
export ARCH=arm64
export CROSS_COMPILE=aarch64-linux-android-
