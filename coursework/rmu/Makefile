all : RMUDemo/RMUDemo

RMUDemo/RMUDemo:
	if [ ! -d  "./Libs" ];
	then mkdir Libs; 
	fi
	cd RMUDemo; 
	make -f Makefile
clean:
	cd RMUDemo; 
	make clean -f Makefile
	