cl:=arm-hisiv300-linux-g++
inc:= -I../comm/base/inc/proto  -I../comm/osa/inc  -I../comm/base/inc -I../hisi/hi_3516a/inc -I../hisi/hi_3516a/hi_sdk/include 
lib:=-L../hisi/hi_3516a/hi_sdk/lib  -lmpi -lVoiceEngine -ldnvqe  -lupvqe -lpthread -ldl
src:=./*.c ../comm/osa/src/*.c  ../comm/base/src/*.cpp ../comm/base/src/*.c
flags:=-fpermissive

all:
	$(cl) -o capture $(src) $(inc) $(lib) $(flags)