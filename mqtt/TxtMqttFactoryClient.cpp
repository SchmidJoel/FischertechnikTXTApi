
#include "TxtMqttFactoryClient.h"

TxtMqttFactoryClient::TxtMqttFactoryClient(std::string clientname, std::string host,
		std::string mqtt_user, mqtt::binary_ref mqtt_pass, std::string port, bool bretained, int iqos)
      : clientname(clientname), host(host), port(port), mqtt_user(mqtt_user), mqtt_pass(mqtt_pass),
	  bretained(bretained), iqos(iqos),
	cli("tcp://" + host + ":" + port, clientname)
{
    connOpts.set_connect_timeout(DFLT_TIMEOUT_CONNECT);
	connOpts.set_keep_alive_interval(DFLT_KEEPALIVE_INTERVAL);
	connOpts.set_clean_session(DFLT_CLEAN_SESSION);
	connOpts.set_automatic_reconnect(DFLT_AUTOMATIC_RECONNECT);
	connOpts.set_mqtt_version(MQTTVERSION_3_1_1);
	connOpts.set_user_name(mqtt_user);
	connOpts.set_password(mqtt_pass);

	callback cb;
	cli.set_callback(cb);
	// TODO will message
}

TxtMqttFactoryClient::~TxtMqttFactoryClient() {
	disconnect(1000);
}

bool TxtMqttFactoryClient::connect(long timeout) {
	try {
		mqtt::token_ptr conntok = cli.connect(connOpts);
		return conntok->wait_for(timeout);
	}
	catch (const mqtt::exception& exc) {
		std::cout << "connect: " << exc.what() << " " << exc.get_reason_code() << std::endl;
	}
	return false;
}

void TxtMqttFactoryClient::disconnect(long timeout) {
	auto toks = cli.get_pending_delivery_tokens();
	// TODO wait for pending tokens
	// TODO unsub? 
	cli.disconnect(timeout);
}

void TxtMqttFactoryClient::publishMessageSync(const std::string& topic, const std::string& message) {
	mqtt::delivery_token_ptr pubtok = cli.publish(topic, message);
	pubtok->wait_for(DFLT_TIMEOUT_MS_PUBLISH);
}

void TxtMqttFactoryClient::publishMessageSync(const std::string& topic, const std::string& message,
		long timeout = DFLT_TIMEOUT_MS_PUBLISH, int qos = DFLT_QUALITY_OF_SERVICE, bool retained = false) {
	mqtt::delivery_token_ptr pubtok = cli.publish(topic, message, qos, retained);
	pubtok->wait_for(timeout);
}

void TxtMqttFactoryClient::publishMessageAsync(const std::string& topic, const std::string& message) {
	mqtt::message_ptr pubmsg = mqtt::make_message(topic, message);
	// TODO handle token
	cli.publish(pubmsg);
}

void TxtMqttFactoryClient::publishMessageAsync(const std::string& topic, const std::string& message, int qos = 1, bool retained = false) {
	mqtt::message_ptr pubmsg = mqtt::make_message(topic, message, qos, retained);
	cli.publish(pubmsg);
}

void TxtMqttFactoryClient::subTopic(const std::string& topicFilter, int qos = DFLT_QUALITY_OF_SERVICE, long timeout = DFLT_TIMEOUT_MS_PUBLISH) {
	mqtt::token_ptr subtok = cli.subscribe(topicFilter, qos);
	subtok->wait_for(timeout);
}

void TxtMqttFactoryClient::unsubTopic(const std::string& topicFilter, long timeout = DFLT_TIMEOUT_MS_PUBLISH) {
	mqtt::token_ptr unsubtok = cli.unsubscribe(topicFilter);
	bool r = unsubtok->wait_for(timeout);
}

void TxtMqttFactoryClient