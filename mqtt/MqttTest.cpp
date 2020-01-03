#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>	// For sleep
#include <atomic>
#include <chrono>
#include <cstring>
#include <mqtt/async_client.h>

unsigned int DebugFlags;
FILE *DebugFile;

const std::string DFLT_SERVER_ADDRESS	{ "tcp://192.168.178.69:1883" };
const std::string DFLT_CLIENT_ID		{ "async_publish" };

const std::string TOPIC { "hello" };

const char* PAYLOAD1 = "Hello World!";
const char* PAYLOAD2 = "Hi there!";
const char* PAYLOAD3 = "Is anyone listening?";
const char* PAYLOAD4 = "Someone is always listening.";

const char* LWT_PAYLOAD = "Last will and testament.";

const int  QOS = 1;

const auto TIMEOUT = std::chrono::seconds(10);


int main(int argc, char* argv[])
{
    std::string	address  = (argc > 1) ? std::string(argv[1]) : DFLT_SERVER_ADDRESS,
			clientID = (argc > 2) ? std::string(argv[2]) : DFLT_CLIENT_ID;

	std::cout << "Initializing for server '" << address << "'..." << std::endl;
	mqtt::async_client client(address, clientID);

	mqtt::callback cb;
	client.set_callback(cb);

	mqtt::connect_options conopts;
	mqtt::message willmsg(TOPIC, LWT_PAYLOAD, 1, true);
	mqtt::will_options will(willmsg);
	conopts.set_will(will);

	std::cout << "  ...OK" << std::endl;

    try {
        // Connect
        std::cout << "\nConnecting..." << std::endl;
		mqtt::token_ptr conntok = client.connect(conopts);
		std::cout << "Waiting for the connection..." << std::endl;
		conntok->wait();
		std::cout << "  ...OK" << std::endl;

        // Wait
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Disconnect
		std::cout << "\nDisconnecting..." << std::endl;
		conntok = client.disconnect();
		conntok->wait();
		std::cout << "  ...OK" << std::endl;
    }
    catch (const mqtt::exception& exc) {
		std::cerr << exc.what() << std::endl;
		return 1;
	}

    return 0;
}