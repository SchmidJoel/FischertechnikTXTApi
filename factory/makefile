INCLUDE = -I"../deps/include" -I"../FactoryLib/include" -I"include"

CXX =arm-linux-gnueabihf-g++ $(INCLUDE)
CXXFLAGS = -std=gnu++17 -O3 -Wall -c -fmessage-length=0 -Wno-psabi 
LFLAGS = -Wall
LIBPATH = -L"../deps/lib" -L"../FactoryLib/obj" 

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
	-l"KeLibTxt"\
	-l"txtlowlevelapi"\
	-l"txthighlevelapi"\
	-l"utils"\
	-l"mqttclient"\
	-l"debug"

$(shell mkdir -p "bin")
$(shell mkdir -p "obj")

all: SortingLineMain ProcessingStationMain MainUnit

VacuumRobot.o : src/VacuumRobot.cpp
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

WarehouseStorage.o : src/WarehouseStorage.cpp
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

HighBayWarehouse.o : src/HighBayWarehouse.cpp
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

MainUnit.o: src/MainUnit.cpp
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

ProcessingStationMain.o: src/ProcessingStationMain.cpp 
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

SortingLineMain.o: src/SortingLineMain.cpp
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

MainUnit: MainUnit.o HighBayWarehouse.o VacuumRobot.o WarehouseStorage.o
	$(CXX) $(LFLAGS) $(LIBPATH) obj/MainUnit.o obj/HighBayWarehouse.o obj/VacuumRobot.o obj/WarehouseStorage.o $(LIBS) -o bin/$@

ProcessingStationMain: ProcessingStationMain.o
	$(CXX) $(LFLAGS) $(LIBPATH) obj/$^ $(LIBS) -o bin/$@

SortingLineMain: SortingLineMain.o
	$(CXX) $(LFLAGS) $(LIBPATH) obj/$^ $(LIBS) -o bin/$@