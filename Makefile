.PHONY:clean all
 
BIN = cor

all: $(BIN)
 
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
 
cor:test01.o sub.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
 
clean:
	echo  "$(LOCAL_PATH)"
	rm -f $(BIN) *.o
