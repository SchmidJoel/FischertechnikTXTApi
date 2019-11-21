INCLUDE_PATH="-ID:\\Code\\fischertechnik\\txt_demo_c_download\\TxtDemo\\Txt_Includes"
TXT_LIB_PATH="-LD:\\Code\\fischertechnik\\txt_demo_c_download\\TxtDemo\\Txt_Libs"
TXT_LIB_DEP_PATH="-LD:\\Code\\fischertechnik\\txt_demo_c_download\\TxtDemo\\Txt_Libs_deps"

all: output/Example.o output/FTLib.o
	arm-linux-gnueabihf-g++ $(TXT_LIB_PATH) $(TXT_LIB_DEP_PATH) -o output/Example_compiled $^ -lKeLibTxt -lMotorIOLib -lROBOProLib -lSDLWidgetsLib -lTxtControlLib -lSDL -lSDL_gfx -lSDL_ttf -lSDL_image -lts -lfreetype -lz -lpng16 -lbz2 -ljpeg -lasound -ldirectfb -lfusion -ldirect -ltiff -std=c++17

FTLib.o: FTLib.cpp
	arm-linux-gnueabihf-g++.exe $(INCLUDE_PATH) -O3 -Wall -c -fmessage-length=0 -o output/FTLib.o FTLib.cpp -std=c++17

Example.o: Example.cpp
	arm-linux-gnueabihf-g++.exe $(INCLUDE_PATH) -O3 -Wall -c -fmessage-length=0 -o output/Example.o Example.cpp -std=c++17
