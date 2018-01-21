GETS= sum_on_thread

CROSS_TOOL =
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -Werror -pthread

add: clean $(TARGETS)

$(TARGETS):
        $(CC_C) $(CFLAGS) $@.c -o $@

clean:
        rm -f $(TARGETS)
