mkdir build
cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && make && ./game && cd ..