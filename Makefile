all: build

build:
	mkdir -p build && cd build && cmake .. && make -j4
.PHONY: build

clean:
	rm -rf build
