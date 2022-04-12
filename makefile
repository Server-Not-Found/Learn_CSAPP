CC = /usr/bin/gcc
CFLAGS = -Wall -g -O2 -Werror -std=gnu99

EXE = program	#指定可执行文件名称

SRC = ./src	#用来指定头文件位置，后面接-I

#需要编译的文件
CODE = ./src/memory/instruction.c ./src/disk/code.c ./src/main.c \
./src/memory/dram.c ./src/cpu/mmu.c

.PHONY: program		#防止名字冲突同时提高效率
main: 
	$(CC) $(CFLAGS) -I $(SRC)  $(CODE) -o $(EXE)
	./$(EXE)

clean:
	rm -f ./$(EXE)