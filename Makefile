# lwws source and objects
src = $(filter-out test.c, $(wildcard *.c))
obj = $(src:.c=.o)
lwws = lwws

#

# tags!
tags = .tags

# compiler flags
DEBUG = -g
LDFLAGS = -L . -lc $(DEBUG)
CC = cc

# lwws
$(lwws): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

$(obj): $(src)
	$(CC) -c  $(DEBUG) $^

# expand symbols
preprocess: $(src)
	$(CC) -E $(CFLAGS) $< -o all.$@

# tags for code navigation in editor
$(tags): $(src)
	ctags -f $(tags) -a $^

clean:
	rm -f $(obj) $(OUTPOUT)
	rm -f $(tags)
	rm -f $(lwws)
	rm -f *.preprocess
	rm test
	rm test.o

clean-port:
	sudo ifconfig en0 down
	sudo ifconfig en0 up

.PHONY: clean
