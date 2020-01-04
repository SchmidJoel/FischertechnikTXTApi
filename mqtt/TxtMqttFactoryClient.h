#ifndef TxtMqttFactoryClient_H_
#define TxtMqttFactoryClient_H_

#include "mqtt/async_client.h"


#define DFLT_TIMEOUT_MS_PUBLISH 5000
#define DFLT_TIMEOUT_CONNECT 90
#define DFLT_KEEPALIVE_INTERVAL 60
#define DFLT_CLEAN_SESSION true
#define DFLT_AUTOMATIC_RECONNECT true
#define DFLT_QUALITY_OF_SERVICE 1
#define DFLT_SERVER_PORT "1883"
#define DFLT_MESSAGE_RETAIN false

#define TOPIC_INPUT_STOCK	"f/i/wh/stock"

class callback : public virtual mqtt::callback
{
public:
	void connected(const std::string& cause) override {
		std::cout << "\nConnection established" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;
	}

	void connection_lost(const std::string& cause) override {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;
	}

	void delivery_complete(mqtt::delivery_token_ptr tok) override {
		std::cout << "\tDelivery complete for token: "
			<< (tok ? tok->get_message_id() : -1) << std::endl;
	}

	void message_arrived(mqtt::const_message_ptr msg) override {
		// TODO message received
	}
};

class TxtMqttFactoryClient {
public:
	TxtMqttFactoryClient(std::string clientname, std::string host,
			std::string mqtt_user, mqtt::binary_ref mqtt_pass, std::string port = DFLT_SERVER_PORT, bool bretained = DFLT_MESSAGE_RETAIN, int iqos = DFLT_QUALITY_OF_SERVICE);
	virtual ~TxtMqttFactoryClient();

	bool is_connected() { return cli.is_connected(); }

	bool connect(long timeout);
	void disconnect(long timeout);

protected:
	void publishMessageSync(const std::string& topic, const std::string& message);
	void publishMessageSync(const std::string& topic, const std::string& message, long timeout = DFLT_TIMEOUT_MS_PUBLISH, int qos = DFLT_QUALITY_OF_SERVICE, bool retained = false);
	void publishMessageAsync(const std::string& topic, const std::string& message);
	void publishMessageAsync(const std::string& topic, const std::string& message, int qos = 1, bool retained = false);
	void subTopic(const std::string& topicFilter, int qos = DFLT_QUALITY_OF_SERVICE, long timeout = DFLT_TIMEOUT_MS_PUBLISH);
	void unsubTopic(const std::string& topicFilter, long timeout = DFLT_TIMEOUT_MS_PUBLISH);

	bool bretained;
	int iqos;
	std::string clientname;
	mqtt::string host;
	mqtt::string port;
	mqtt::string mqtt_user;
	mqtt::binary_ref mqtt_pass;

	mqtt::async_client cli;
	mqtt::connect_options connOpts;
};

#endif