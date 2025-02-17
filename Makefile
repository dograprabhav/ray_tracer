.PHONY: all build run clean

all: build run

build:
	cmake -B build
	cmake --build build --verbose

v1:
	make build
	./build/v1 > v1.ppm

v1:
	make build
	./build/v2 > v2.ppm

clean:
	rm -rf build image.ppm
