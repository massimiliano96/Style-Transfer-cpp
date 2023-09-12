prepare:
	rm -rf build

configure_d:
	mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug

configure_r:
	mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release