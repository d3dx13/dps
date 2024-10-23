rm -rf build && mkdir build && cd build
flatc --gen-mutable --gen-object-api --cpp --python ../*.fbs 
cd -
python3 check.py
g++ flat_diz_buffer.cpp -o flat_diz_buffer -Ibuild
./flat_diz_buffer