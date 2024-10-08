rm -rf build && mkdir build && cd build
capnp compile --src-prefix=.. --output=c++ ../image.capnp
cd -