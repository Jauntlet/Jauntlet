sudo apt install make cmake libglm-dev libsdl2-dev libglew-dev g++
#non debian nerds can die

mkdir build
cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && make && ./game && cd ..