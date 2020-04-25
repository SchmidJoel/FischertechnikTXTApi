INCLUDE = -I"../deps/include" -I"include"

CXX =arm-linux-gnueabihf-g++ $(INCLUDE)
CXXFLAGS = -std=gnu++17 -O3 -Wall -c -fmessage-length=0 -Wno-psabi -shared -Wnarrowing -Wparentheses

$(shell mkdir -p "obj")

all: libutils.so libmqttclient.so libtxthighlevelapi.so libtxtlowlevelapi.so libdebug.so

libutils.so: include/utils.h
	$(CXX) $(CXXFLAGS) src/utils.cpp -o obj/$@

libmqttclient.so: include/TxtMqttFactoryClient.h
	$(CXX) $(CXXFLAGS) src/TxtMqttFactoryClient.cpp -o obj/$@

libtxthighlevelapi.so: include/TXT_highlevel_API.h
	$(CXX) $(CXXFLAGS) src/TXT_highlevel_API.cpp -o obj/$@

libtxtlowlevelapi.so: include/TXT_lowlevel_API.h
	$(CXX) $(CXXFLAGS) src/TXT_lowlevel_API.cpp -o obj/$@

libdebug.so: include/debug.h
	$(CXX) $(CXXFLAGS) src/debug.cpp -o obj/$@