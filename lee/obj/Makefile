OBJS := *.o

main.o: main.c
	$(CC) $(LDFLAGS) $(INCLUDE_DIR) $(CFLAGS) -c $^ -o $@

$(ROOT_DIR)/$(BIN):$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

