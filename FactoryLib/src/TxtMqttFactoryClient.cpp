#include "TxtMqttFactoryClient.h"

TxtMqttFactoryClient::TxtMqttFactoryClient(std::string clientname, std::string host,
										   std::string mqtt_user, mqtt::binary_ref mqtt_pass, std::string port)
	: clientname(clientname), host(host), port(port), mqtt_user(mqtt_user), mqtt_pass(mqtt_pass),
	  cli("tcp://" + host + ":" + port, clientname)
{
	connOpts.set_connect_timeout(DFLT_TIMEOUT_CONNECT);
	connOpts.set_keep_alive_interval(DFLT_KEEPALIVE_INTERVAL);
	connOpts.set_clean_session(DFLT_CLEAN_SESSION);
	connOpts.set_automatic_reconnect(DFLT_AUTOMATIC_RECONNECT);
	connOpts.set_mqtt_version(MQTTVERSION_3_1_1);
	connOpts.set_user_name(mqtt_user);
	connOpts.set_password(mqtt_pass);

	cli.set_callback(cb);
	
	// TODO will message
}

TxtMqttFactoryClient::~TxtMqttFactoryClient()
{
	disconnect(1000);
}

bool TxtMqttFactoryClient::connect(long timeout)
{
	try
	{
		mqtt::token_ptr conntok = cli.connect(connOpts);
		return conntok->wait_for(timeout);
	}
	catch (const mqtt::exception &exc)
	{
		std::cout << "connect: " << exc.what() << " " << exc.get_reason_code() << std::endl;
	}
	return false;
}

void TxtMqttFactoryClient::disconnect(long timeout)
{
	// auto toks = cli.get_pending_delivery_tokens();
	// TODO wait for pending tokens
	// TODO unsub?
	cli.disconnect(timeout);
}

bool TxtMqttFactoryClient::publishMessageSync(const std::string &topicFilter, const std::string &message, long timeout, int qos, bool retained)
{
	if (!cli.is_connected()) {
		return false;
	}

	mqtt::delivery_token_ptr pubtok = cli.publish(topicFilter, message, qos, retained);
	return pubtok->wait_for(timeout);
}

void TxtMqttFactoryClient::publishMessageAsync(const std::string &topicFilter, const std::string &message, int qos, bool retained)
{
	if (!cli.is_connected()) {
		return;
	}

	mqtt::delivery_token_ptr pubtok = cli.publish(topicFilter, message, qos, retained);
}

bool TxtMqttFactoryClient::subTopicAsync(const std::string &topicFilter, void (*func)(const std::string &message), int qos, long timeout)
{
	if (!cli.is_connected()) {
		return false;
	}

	mqtt::token_ptr subtok = cli.subscribe(topicFilter, qos);
	bool r = subtok->wait_for(timeout);

	if (r) {
		cb.register_topic(topicFilter, func);
	}
	
	return r;
}

bool TxtMqttFactoryClient::subTopicSync(const std::string &topicFilter, int qos, long timeout)
{
	if (!cli.is_connected()) {
		return false;
	}

	mqtt::token_ptr subtok = cli.subscribe(topicFilter, qos);
	return subtok->wait_for(timeout);
}

bool TxtMqttFactoryClient::unsubTopic(const std::string &topicFilter, long timeout)
{
	if (!cli.is_connected()) {
		return false;
	}

	mqtt::token_ptr unsubtok = cli.unsubscribe(topicFilter);
	bool r = unsubtok->wait_for(timeout);

	cb.remove_topic(topicFilter);

	return r;
}

std::string TxtMqttFactoryClient::consume_topic(const std::string &topicFilter)
{
	if (!cli.is_connected()) {
		return "";
	}

	std::string message;

	cli.start_consuming();

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		auto msg = cli.consume_message();
		if ((msg->get_topic().compare(topicFilter)) == 0)
		{
			message = msg->get_payload_str();
			break;
		}
	}

	cli.stop_consuming();

	return message;
}
