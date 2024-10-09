cd ../dps;
./build.sh
cd -

# rm -rf ./build && \
mkdir -p build && \
cd build && \
cmake .. && \
make && \
sudo ./main && \
cd -
tree -ahp /dev/shm/