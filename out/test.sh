#!/bin/bash
adb_push=yes
modules=$(ls)
if [ "${adb_push}" == "yes" ];then
        cd ${OUTDIR}
        for module in ${modules}
        do
		echo ${module}
        done
else
	echo "no push"
fi
