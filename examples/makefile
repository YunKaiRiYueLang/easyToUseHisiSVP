cxx:=arm-himix200-linux-g++

ROOT=../../../../
TARGET=test-example

include=-I..	-I../tools -I../hisi \
		-I$(ROOT)Hi3516CV500_SDK_V2.0.2.0/smp/a7_linux/mpp/include 	\
		-I$(ROOT)opencv_build/neoninstall/include/opencv4	\
		-I$(ROOT)opencv_build/neoninstall/include/opencv4/opencv2	\
		
		# -I$(ROOT)opencv_build/neoninstall/include/opencv4	\
		-I$(ROOT)opencv_build/neoninstall/include/opencv4/opencv2	\


lib =-L$(ROOT)opencv_build/neoninstall/lib -lopencv_world		\
	-L$(ROOT)Hi3516CV500_SDK_V2.0.2.0/smp/a7_linux/mpp/lib	\
	-lpthread -ldl -lrt -lm		\
	-lmpi -live -lsecurec	\
	-ldnvqe -lupvqe -lVoiceEngine	
	# -L$(ROOT)opencv_build/neoninstall/lib -lopencv_world		\

test:
	$(cxx) -Wl,--gc-sections  -O3  -mcpu=cortex-a7  -mfloat-abi=softfp  -mfpu=neon-vfpv4     *.cpp ../tools/*.cpp $(include) $(lib) \
	-o $(TARGET)
	cp $(TARGET) /nfs/bjwang/cameraProgram/c212/demo
	cp $(ROOT)opencv_build/neoninstall/lib/libopencv_world.so /nfs/bjwang/cameraProgram/c212/demo/lib

clean:
	rm -f test