build: src/main.cpp include/vendor/* build/vendor/httplib.o
	mkdir -p build
	g++ -o build/main src/main.cpp -I./include build/vendor/httplib.o

build/vendor/httplib.o: include/vendor/httplib.h src/vendor/httplib.cpp
	mkdir -p build/vendor
	g++ -c src/vendor/httplib.cpp -I./include -o build/vendor/httplib.o

run:
	@./build/main
