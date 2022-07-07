#指定编译器
cxx:=arm-himix200-linux-g++ 
#指定sdk包含目录
inc:=-I ../../Hi3516CV500_SDK_V2.0.3.0/mpp/include  \
-I ./inc
#指定需要连接的库目录和库
lib:=-L../../Hi3516CV500_SDK_V2.0.3.0/mpp/lib	\
	 -Wl,-Bstatic -live -lmpi -lsecurec -lVoiceEngine -ldnvqe -lupvqe   -lmpi	\
	 -Wl,-Bdynamic -ldl -lm -lpthread -Wl,--as-needed  -std=c++11 	
#可选
# flags:= -fpermissive

src:=./*.cpp ./src/*.cpp
target_name=yuvSave
target=-o $(target_name)

all:
	$(cxx) $(src)  $(inc) $(lib) $(flags) $(target) 
clean:
	rm $(target_name)
	