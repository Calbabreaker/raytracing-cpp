# Makefile to make it a bit easier to build on unix

BUILD_TYPE := Release

.PHONY: run clean build/bin/*

build/bin/*: build/Makefile
	cmake --build build

build/Makefile: 
	mkdir build -p
	cd build && cmake ../ -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

run: build/bin/*
	$^

clean: 
	rm -r build/
