src = $(wildcard *.c )
obj = $(src:.c=.o)

DEBUG = -g
LDFLAGS = -L . -lc $(DEBUG)
CC = cc
OUTPUT = lwws

test: $(OUTPUT)
	# Really should put some tests here!
	echo "Runing tests:"

preprocess: $(src)
	$(CC) -E $^ -o $^.preprocess

tags: $(src)
	ctags $^

$(OUTPUT): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)


clean:
	rm -f $(obj) $(OUTPOUT)
	rm -f tags
	rm -f $(OUTPUT)

clean-port:
	sudo ifconfig en0 down
	sudo ifconfig en0 up

.PHONY: clean
