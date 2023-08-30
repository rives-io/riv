include ../demo-base.mk

$(NAME).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean::
	rm -f *.o
