#!/bin/bash

EMCC_DEBUG=1 em++ src/blocks.cpp src/shapes.cpp src/vector3.cpp src/states/gs_game_play.cpp src/states/gs_main_menu.cpp -s USE_SDL=2 --preload-file assets/sheet.bmp -g4 -s ASSERTIONS=2 -s SAFE_HEAP=1 -s SAFE_HEAP_LOG=1 -s STACK_OVERFLOW_CHECK=2 -profiling -Iinc/ -o html/blocks.html
cd html
python -m SimpleHTTPServer 8080
cd ..
