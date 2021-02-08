all:
	arm-himix200-linux-g++ -static -Wl,--gc-sections -o ivetest *.cpp -I./ -I../../Hi3516CV500_SDK_V2.0.2.0/smp/a7_linux/mpp/include -L../../Hi3516CV500_SDK_V2.0.2.0/smp/a7_linux/mpp/lib -lmpi -live 	\
	-lpthread	-lsecurec	\
	# -ldnvqe -lupvqe -lVoiceEngine

	rm -f /nfs/bjwang/cameraProgram/c212/demo/ivetest
	cp ivetest /nfs/bjwang/cameraProgram/c212/demo