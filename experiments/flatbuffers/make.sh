rm -rf build && mkdir build && cd build
flatc --reflect-types --reflect-names --gen-object-api --cpp --python ../*.fbs 
cd -