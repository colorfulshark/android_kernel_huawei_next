#!/bin/bash

PHONE_DIR=/sdcard/modules
CURDIR=$(pwd)
source ${CURDIR}/build/setenv.sh
mkdir ${CURDIR}/out 2> /dev/null
OUTDIR=${CURDIR}/out
make clean
make modules

cp ${CURDIR}/drivers/net/mii.ko ${OUTDIR}
cp ${CURDIR}/drivers/net/usb/usbnet.ko ${OUTDIR}
cp ${CURDIR}/drivers/net/usb/smsc95xx.ko ${OUTDIR}
cp ${CURDIR}/drivers/net/usb/dm9601.ko ${OUTDIR}
cp ${CURDIR}/drivers/net/usb/r8152.ko ${OUTDIR}

adb_push=yes
if [ "${adb_push}" == "yes" ];then
	cd ${OUTDIR}
	modules=$(ls)
	for module in ${modules}
	do
		adb push ${module} ${PHONE_DIR}
	done
fi
