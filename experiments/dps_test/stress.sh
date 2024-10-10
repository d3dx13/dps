cd ../dps;
./build.sh
cd -

# rm -rf ./build && \
mkdir -p build && \
cd build && \
cmake .. && \
make
for i in $(seq 1 1000);
do
    sudo ./main
done
cd -
tree -ahp /dev/shm/