.PHONY: tags perf

all:
	mkdir -p build
	cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && cd .. && make -C build -j12

debug:
	mkdir -p build
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && cd .. && make -C build -j12

serial:
	mkdir -p build
	cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make VERBOSE=1 && cd ..

clean:
	rm -rf build

check: all
	make CTEST_OUTPUT_ON_FAILURE=1 -C build test

tags: 
	@echo Making tags...
	/usr/bin/find . -name '*.c' -o -name '*.cpp' -o -name '*.h' | grep -v "moc_" | grep -v "ui_" | grep -v "/o/"> flist && \
	etags --file-tags=yes --language-force=C++ -L flist
	rm flist
	@echo tags complete.
