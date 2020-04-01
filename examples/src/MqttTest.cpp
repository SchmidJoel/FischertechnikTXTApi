#include "TxtMqttFactoryClient.h"

bool end = false;

void test(const std::string &message)
{
	end = true;
}

int main(void)
{
	TxtMqttFactoryClient mqttClient("TestClient", "192.168.178.69", "", "");

	mqttClient.connect(1000);

	mqttClient.subTopicAsync(TOPIC_INPUT_STOCK, test);

	//mqttClient.publishMessage(TOPIC_INPUT_STOCK, "1");

	while (!end)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}
