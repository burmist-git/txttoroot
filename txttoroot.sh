#!/bin/bash

function printHelp {
    echo " --> ERROR in input arguments "
    echo " [0] -d : default"
    echo " [0] -h : print help"
}

if [ $# -eq 0 ] 
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
	rm -rf txttoroot.log
	firstFile=`head -n 1 doConvert.txt | awk '{print $2}'`
	nwf=`grep waveform $firstFile | wc -l`
	Npoints=`grep Npoints $firstFile | head -n 1 | awk '{print $2}'`
	for file_to_convert in `more doConvert.txt | awk '{print $2}' | xargs`; do
	    time ./txttoroot $file_to_convert $Npoints $nwf | tee -a txttoroot.log
	done
    elif [ "$1" = "-h" ]; then
        printHelp
    else
        printHelp
    fi
fi

#espeak "I have done"
