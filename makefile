INCLUDE_PATH = -std=gnu++0x -std=c++0x -I"deps\include" -I"FactoryLib/include" -O3 -Wall -c -fmessage-length=0 -Wno-psabi
LIB_PATH = -L"deps\lib"
COMPILER = "arm-linux-gnueabihf-g++"

BIN_DIR = "bin"
SRC_DIR = FactoryLib/src/
EXAMPLE_DIR = examples/
FACTORY_DIR = factory/

LIBS = -l"SDLWidgetsLib" \
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

	
#	-l"paho-mqtt3c" \
	-l"paho-mqtt3a" \
	-l"paho-mqttpp3" \

$(shell mkdir -p $(BIN_DIR))

all: $(BIN_DIR)/main.o $(BIN_DIR)/txtlowlevelapi.o $(BIN_DIR)/txthighlevelapi.o $(BIN_DIR)/utils.o
	$(COMPILER) $(LIB_PATH) -o $(BIN_DIR)/Main_compiled $^ $(LIBS)

mqtt: $(BIN_DIR)/MqttTest.o $(BIN_DIR)/TxtMqttFactoryClient.o
	$(COMPILER) $(LIB_PATH) -o $(BIN_DIR)/MqttTest_compiled $^ $(LIBS)

threads: $(BIN_DIR)/Threads.o $(BIN_DIR)/txtlowlevelapi.o
	$(COMPILER) $(LIB_PATH) -o $(BIN_DIR)/Threads_compiled $^ $(LIBS)

$(BIN_DIR)/Threads.o : $(EXAMPLE_DIR)Threads.cpp
	$(COMPILER) $(INCLUDE_PATH) -o $(BIN_DIR)/Threads.o $(EXAMPLE_DIR)Threads.cpp

$(BIN_DIR)/MqttTest.o : $(EXAMPLE_DIR)MqttTest.cpp
	$(COMPILER) $(INCLUDE_PATH) -o $(BIN_DIR)/MqttTest.o $(EXAMPLE_DIR)MqttTest.cpp

$(BIN_DIR)/TxtMqttFactoryClient.o : $(SRC_DIR)TxtMqttFactoryClient.cpp
	$(COMPILER) $(INCLUDE_PATH) -o $(BIN_DIR)/TxtMqttFactoryClient.o $(SRC_DIR)TxtMqttFactoryClient.cpp

$(BIN_DIR)/txtlowlevelapi.o: $(SRC_DIR)TXT_lowlevel_API.cpp
	$(COMPILER) $(INCLUDE_PATH) -o $(BIN_DIR)/txtlowlevelapi.o $(SRC_DIR)TXT_lowlevel_API.cpp

$(BIN_DIR)/txthighlevelapi.o: $(SRC_DIR)TXT_highlevel_API.cpp
	$(COMPILER) $(INCLUDE_PATH) -o $(BIN_DIR)/txthighlevelapi.o $(SRC_DIR)TXT_highlevel_API.cpp

$(BIN_DIR)/utils.o: $(SRC_DIR)utils.cpp
	$(COMPILER) $(INCLUDE_PATH) -o $(BIN_DIR)/utils.o $(SRC_DIR)utils.cpp

$(BIN_DIR)/main.o: main.cpp
	$(COMPILER) $(INCLUDE_PATH) -o $(BIN_DIR)/main.o main.cpp

$(BIN_DIR)/highbay.o: $(FACTORY_DIR)HighBayWarehouse.hpp
	$(COMPILER) $(INCLUDE_PATH) -o $(BIN_DIR)/highbay.o $(FACTORY_DIR)HighBayWarehouse.hpp

$(BIN_DIR)/vacuum.o: $(FACTORY_DIR)VacuumRobot.hpp
	$(COMPILER) $(INCLUDE_PATH) -o $(BIN_DIR)/vacuum.o $(FACTORY_DIR)VacuumRobot.hpp

.PHONY: clean
clean:
	rm -f -r $(BIN_DIR)
