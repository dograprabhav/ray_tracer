.PHONY: all build run clean

all: build run

build:
	cmake -B build
	cmake --build build --verbose

run:
	make build
	./build/v1 > v1.ppm
	./build/v2 > v2.ppm
	./build/v3 > v3.ppm
	./build/v4 > v4.ppm
	./build/v5 > v5.ppm
	./build/v6 > v6.ppm

custom:
	@read -p "Enter version (e.g., v2, v3, v4, v5, v6): " V; \
	make build; \
	./build/$$V > $$V.ppm

clean:
	rm -rf build image.ppm
