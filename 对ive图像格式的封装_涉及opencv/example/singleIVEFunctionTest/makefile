c++:=arm-himix200-linux-g++
ar:=arm-himix200-linux-ar
inc:=-I ./inc	\
	-I../../Hi3516CV500_SDK_V2.0.2.0/smp/a7_linux/mpp/include	\
	-I./ 

all:
	arm-himix200-linux-g++ -static -Wl,--gc-sections -o ivetest *.cpp $(inc) -L../../Hi3516CV500_SDK_V2.0.2.0/smp/a7_linux/mpp/lib -L./lib -lmpi -live 	\
	-lpthread	-lsecurec -lhisiUtils	\
	# -ldnvqe -lupvqe -lVoiceEngine

	rm -f /nfs/bjwang/cameraProgram/c212/demo/ivetest
	cp ivetest /nfs/bjwang/cameraProgram/c212/demo

clean:
	rm -f *.a *.o