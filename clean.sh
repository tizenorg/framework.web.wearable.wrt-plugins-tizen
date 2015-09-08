#!/bin/bash

echo Performing clean

function clean {
		rm -rf CMakeCache.txt
	        rm -rf CMakeFiles
		rm -rf cmake_install.cmake
		rm -rf Makefile
		rm -rf install_manifest.txt			
		rm -rf *.so

		for i in `ls`; do
			if [ -d $i ]; then
				cd $i;
				clean;
				cd ..
			fi
		done	
	}

	clean



