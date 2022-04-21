CC = /usr/bin/gcc
CFLAGS = -Wall -g -O2 -Werror -std=gnu99

EXE = hardware	#指定可执行文件名称

SRC = ./src	#用来指定头文件位置，后面接-I
REF = ./refactorf

#需要编译的文件
CODE_SRC = ./src/memory/instruction.c ./src/disk/code.c ./src/main.c \
./src/memory/dram.c ./src/cpu/mmu.c

CODE_REF = $(REF)/common/convert.c $(REF)/hardware/cpu/isa.c \
		   $(REF)/hardware/cpu/mmu.c $(REF)/main_hardware.c

.PHONY: hardware		#防止名字冲突同时提高效率
hardware:
	$(CC) $(CFLAGS) -I $(REF)  $(CODE_REF) -o $(EXE)
	./$(EXE)

src: 
	$(CC) $(CFLAGS) -I $(SRC)  $(CODE_SRC) -o $(EXE)
	./$(EXE)

clean:
	rm -f ./$(EXE)