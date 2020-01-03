INCLUDE_PATH = -std=gnu++0x -std=c++0x -I"deps\include" -O3 -Wall -c -fmessage-length=0 -Wno-psabi
LIB_PATH = -L"deps\lib"
COMPILER = "arm-linux-gnueabihf-g++" 

LIBS = -l"SDLWidgetsLib" \
	-l"paho-mqtt3c" \
	-l"paho-mqtt3a" \
	-l"paho-mqttpp3" \
	-l"jsoncpp" \
	-l"pthread" \
	-l"SDL" \
	-l"SDL_gfx" \
	-l"SDL_ttf" \
	-l"ts" \
	-l"freetype" \
	-l"z" \
	-l"png16" \
	-l"bz2" \
	-l"jpeg" \
	-l"asound" \
	-l"SDL_image" \
	-l"crypto" \
	-l"TxtControlLib" \
	-l"ROBOProLib" \
	-l"MotorIOLib" \
	-l"KeLibTxt"

all: output/Example.o output/txtlowlevelapi.o output/txthighlevelapi.o
	$(COMPILER) $(LIB_PATH) -o output/Example_compiled $^ $(LIBS)

mqtt: output/MqttTest.o
	$(COMPILER) $(LIB_PATH) -o output/MqttTest_compiled $^ $(LIBS)

output/MqttTest.o : mqtt/MqttTest.cpp
	$(COMPILER) $(INCLUDE_PATH) -o output/MqttTest.o mqtt/MqttTest.cpp

output/txtlowlevelapi.o: TXT_lowlevel_API.cpp
	$(COMPILER) $(INCLUDE_PATH) -o output/txtlowlevelapi.o TXT_lowlevel_API.cpp

output/txthighlevelapi.o: TXT_highlevel_API.cpp
	$(COMPILER) $(INCLUDE_PATH) -o output/txthighlevelapi.o TXT_highlevel_API.cpp

output/Example.o: Example.cpp
	$(COMPILER) $(INCLUDE_PATH) -o output/Example.o Example.cpp
