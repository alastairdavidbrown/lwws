src = $(wildcard *.c)
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

$(OUTPUT): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)
	

.PHONY: clean
clean:
	rm -f $(obj) $(OUTPOUT)
