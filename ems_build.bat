call "D:\Projects\Third party Libraries\emsdk\emsdk_env.bat"

md build\bin\Release\WASM

call em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 src/main.cpp src/Game.cpp src/Map.cpp src/Object.cpp src/olcPixelGameEngine.cpp -o build/bin/Release/WASM/pge.html --preload-file .\assets

call emrun build/bin/Release/WASM/basic_template.html