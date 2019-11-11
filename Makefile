CC = cl 
CFLAGS = /O2 /Za
LFLAGS = 
OBJS = obj\bitpack.obj obj\main.obj 
BIN = bitpack.exe 
 
$(BIN): $(OBJS) 
	$(CC) /Fe$@ $(OBJS) $(LFLAGS) 
 
obj\bitpack.obj: src\bitpack.c src\bitpack.h 
	$(CC) /Fo$@ /c src\bitpack.c $(CFLAGS) 
 
obj\main.obj: src\main.c src\bitpack.h 
	$(CC) /Fo$@ /c src\main.c $(CFLAGS) 
 
.PHONY : clean
clean: 
	del $(OBJS) $(BIN) 
