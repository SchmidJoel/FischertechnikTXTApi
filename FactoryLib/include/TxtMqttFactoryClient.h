#ifndef TxtMqttFactoryClient_H_
#define TxtMqttFactoryClient_H_

#include "mqtt/async_client.h"
#include <unordered_map>

#define DFLT_TIMEOUT_MS_PUBLISH 1000
#define DFLT_TIMEOUT_CONNECT 90
#define DFLT_KEEPALIVE_INTERVAL 60
#define DFLT_CLEAN_SESSION true
#define DFLT_AUTOMATIC_RECONNECT true
#define DFLT_QUALITY_OF_SERVICE 1
#define DFLT_SERVER_PORT "1883"
#define DFLT_MESSAGE_RETAIN false

#define TOPIC_INPUT_STOCK "/i/wh/stock"
#define TOPIC_INPUT_SORTINGLINE_STATE "/i/sl/state"
#define TOPIC_INPUT_SORTINGLINE_LAST_COLOR "/i/sl/lastcolor"
#define TOPIC_INPUT_SORTINGLINE_RAW_LAST_COLOR "/i/sl/rawlastcolor"
#define TOPIC_INPUT_VACUUMROBOT_STATE "/i/vr/state"
#define TOPIC_INPUT_WAREHOUSE_STATE "/i/wh/state"
#define TOPIC_INPUT_PROCESSINGSTATION_STATE "/i/ps/state"

#define TOPIC_DEBUG_PROCESSINGSTATION "/d/ps"
#define TOPIC_DEBUG_SORTINGLINE "/d/sl"
#define TOPIC_DEBUG_VACUUMROBOT "/d/vr"
#define TOPIC_DEBUG_MESSAGE "/d/msg"

#define TOPIC_MONITOR_VR_M2_TEMPERATURE "/m/vr/m2/t"
#define TOPIC_MONITOR_VR_O7_VOLTAGE "/m/vr/o7/v"
#define TOPIC_MONITOR_SL_M1_TEMPERATURE "/m/sl/m1/t"
#define TOPIC_MONITOR_SL_M1_VOLTAGE "/m/sl/m1/v"

class FactoryCallback : public virtual mqtt::callback
{
public:
	virtual ~FactoryCallback() {}

	void register_topic(std::string topic, void (*func)(const std::string &message))
	{
		if (callback_func[topic] == nullptr)
		{
			callback_func[topic] = func;
		}
		else
		{
			throw std::invalid_argument(topic + " is already registered");
		}
	}

	void remove_topic(const std::string &topic)
	{
		callback_func[topic] = nullptr;
	}

private:
	std::unordered_map<std::string, void (*)(const std::string &message)> callback_func;

	/**
	 * This method is called when the client is connected.
	 * @param cause
	 */
	void connected(const std::string& cause) override {}

	/**
	 * This method is called when the connection to the server is lost.
	 * @param cause
	 */
	void connection_lost(const std::string& cause) override {}

	/**
	 * This method is called when a message arrives from the server.
	 * @param msg The message
	 */
	void message_arrived(mqtt::const_message_ptr msg) override
	{
		callback_func[msg->get_topic()](msg->get_payload_str());
	}

	/**
	 * Called when delivery for a message has been completed, and all
	 * acknowledgments have been received.
	 * @param tok The token tracking the message delivery.
	 */
	void delivery_complete(mqtt::delivery_token_ptr tok) override {}
};

class TxtMqttFactoryClient
{
public:
	TxtMqttFactoryClient(std::string clientname, std::string host,
						 std::string mqtt_user, mqtt::binary_ref mqtt_pass, std::string port = DFLT_SERVER_PORT);
	virtual ~TxtMqttFactoryClient();

	bool is_connected() { return cli.is_connected(); }

	bool connect(long timeout);
	void disconnect(long timeout);

	bool publishMessageSync(const std::string &topicFilter, const std::string &message, long timeout = DFLT_TIMEOUT_MS_PUBLISH, int qos = DFLT_QUALITY_OF_SERVICE, bool retained = DFLT_MESSAGE_RETAIN);
	void publishMessageAsync(const std::string &topicFilter, const std::string &message, int qos = DFLT_QUALITY_OF_SERVICE, bool retained = DFLT_MESSAGE_RETAIN);
	bool subTopicAsync(const std::string &topicFilter, void (*func)(const std::string &message), int qos = DFLT_QUALITY_OF_SERVICE, long timeout = DFLT_TIMEOUT_MS_PUBLISH);
	bool subTopicSync(const std::string &topicFilter, int qos = DFLT_QUALITY_OF_SERVICE, long timeout = DFLT_TIMEOUT_MS_PUBLISH);
	bool unsubTopic(const std::string &topicFilter, long timeout = DFLT_TIMEOUT_MS_PUBLISH);
	std::string consume_topic(const std::string &topicFilter);

protected:
	std::string clientname;
	mqtt::string host;
	mqtt::string port;
	mqtt::string mqtt_user;
	mqtt::binary_ref mqtt_pass;

	mqtt::async_client cli;
	mqtt::connect_options connOpts;

	FactoryCallback cb;
};

#endif