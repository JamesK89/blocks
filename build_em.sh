#!/bin/bash

em++ src/blocks.cpp src/shapes.cpp src/vector3.cpp src/states/gs_game_play.cpp src/states/gs_main_menu.cpp -s USE_SDL=2 --preload-file assets/sheet.bmp -O3 -s ALLOW_MEMORY_GROWTH=1 -Iinc/ -o html/blocks.html
cd html
python -m SimpleHTTPServer 8080
cd ..
