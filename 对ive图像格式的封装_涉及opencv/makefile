c++:=arm-himix200-linux-g++
ar:=arm-himix200-linux-ar
root=../../../
inc:=-I ./inc	\
	-I$(root)Hi3516CV500_SDK_V2.0.2.0/smp/a7_linux/mpp/include	\
	-I./ 

# all 编译例子代码的示例
all:
	arm-himix200-linux-g++ -static -Wl,--gc-sections -o ivetest *.cpp $(inc) -L../../Hi3516CV500_SDK_V2.0.2.0/smp/a7_linux/mpp/lib -L./lib -lmpi -live 	\
	-lpthread	-lsecurec -lhisiUtils	\

getlib:
	$(c++) -c ./src/*.cpp $(inc)
	$(ar) -crv libhisiUtils.a *.o
	mv *.o src/object
	mv libhisiUtils.a lib -f

clean:
	rm -f *.a *.o