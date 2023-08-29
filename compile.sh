#sudo apt install make cmake libglm-dev libsdl2-dev libglew-dev g++
#non debian nerds can die

mkdir build
cd build
cmake ..
make
./game
cd ..