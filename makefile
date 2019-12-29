INCLUDE_PATH="-IC:\Users\joels\Programmieren\fischertechnik\txt_demo_c_download\deps\include"
LIB_PATH="-LC:\Users\joels\Programmieren\fischertechnik\txt_demo_c_download\deps\lib"
LIBS=-lKeLibTxt -lMotorIOLib -lROBOProLib -lSDLWidgetsLib -lTxtControlLib -lSDL -lSDL_gfx -lSDL_ttf -lSDL_image -lts -lfreetype -lz -lpng16 -lbz2 -ljpeg -lasound
COMPILER="arm-linux-gnueabihf-g++" 


all: output/Example.o output/txtlowlevelapi.o output/txthighlevelapi.o
	$(COMPILER) $(LIB_PATH) -o output/Example_compiled $^ $(LIBS) -std=c++17

output/txtlowlevelapi.o: TXT_lowlevel_API.cpp
	$(COMPILER) $(INCLUDE_PATH) -O3 -Wall -c -fmessage-length=0 -o output/txtlowlevelapi.o TXT_lowlevel_API.cpp -std=c++17

output/txthighlevelapi.o: TXT_highlevel_API.cpp
	$(COMPILER) $(INCLUDE_PATH) -O3 -Wall -c -fmessage-length=0 -o output/txthighlevelapi.o TXT_highlevel_API.cpp -std=c++17

output/Example.o: Example.cpp
	$(COMPILER) $(INCLUDE_PATH) -O3 -Wall -c -fmessage-length=0 -o output/Example.o Example.cpp -std=c++17
