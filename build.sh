rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
cp as_bundler ../pcx-bundler
cd ..
