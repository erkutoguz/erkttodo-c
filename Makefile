src = erkttodo.c
target = $(basename $(src))

all:
	gcc $(src) -Wall -g -o $(target)

clean:
	rm -f $(target)
