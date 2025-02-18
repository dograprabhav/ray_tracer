.PHONY: all build run clean

all: build run

build:
	cmake -B build
	cmake --build build --verbose

v1:
	make build
	./build/v1 > v1.ppm

v2:
	make build
	./build/v2 > v2.ppm

v3:
	make build
	./build/v3 > v3.ppm

clean:
	rm -rf build image.ppm
