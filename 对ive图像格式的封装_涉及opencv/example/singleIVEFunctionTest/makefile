c++:=arm-himix200-linux-g++
ar:=arm-himix200-linux-ar
root=../../../../../

inc:=-I ../../inc -I. 	\
	-I $(root)Hi3516CV500_SDK_V2.0.2.0/smp/a7_linux/mpp/include	\

lib:=-L $(root)Hi3516CV500_SDK_V2.0.2.0/smp/a7_linux/mpp/lib -L../../lib -lmpi -live 	\
	-lpthread	-lsecurec -lhisiUtils	\

all:
	arm-himix200-linux-g++ -static -Wl,--gc-sections -o ivetest *.cpp $(inc) $(lib)

	rm -f /nfs/bjwang/cameraProgram/c212/demo/ivetest
	cp ivetest /nfs/bjwang/cameraProgram/c212/demo

clean:
	rm -f *.a *.o