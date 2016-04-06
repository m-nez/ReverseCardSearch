LIBS=`pkg-config --libs --cflags sdl2 SDL2_image`
CFLAGS=-Wall -ggdb

main: main.c img.o data.o
	$(CC) $^ $(LIBS) $(CFLAGS) -o $@

dataset: dataset.c img.o data.o
	$(CC) $^ $(LIBS) $(CFLAGS) -o $@

search: search.c img.o data.o compare.o search_pipeline.o
	$(CC) $^ $(LIBS) $(CFLAGS) -o $@

test_img: test_img.c
	$(CC) $^ $(LIBS) $(CFLAGS) -o $@

.PHONY: all
.DEFAULT: all
all: main dataset search
	cp -t ../bin $^

.PHONY: clean
clean:
	rm ./*.o
