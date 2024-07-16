rm -rf bin
rm -rf build
cmake cmake -S . -Bbuild -DCMAKE_INSTALL_PREFIX=bin -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=x86_64
cmake --build build --config Release  --target install