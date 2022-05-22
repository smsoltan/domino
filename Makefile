GCC = g++
CFLAGS = -Wall -std=c++0x
#No linking flags for now
OBJECTS = main.o domino.o
NAME = domino

.PHONY: all
all: $(NAME)

$(NAME): $(OBJECTS)
	$(GCC) -o $@ $(OBJECTS)

%.o: %.cpp
	$(GCC) -c $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f $(OBJECTS)  $(NAME)

