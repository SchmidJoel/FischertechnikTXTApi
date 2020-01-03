
#include "TxtMqttFactoryClient.h"

TxtMqttFactoryClient::TxtMqttFactoryClient(std::string clientname, std::string host, std::string port,
		std::string mqtt_user, mqtt::binary_ref mqtt_pass, bool bretained, int iqos)
      : clientname(clientname), host(host), port(port), mqtt_user(mqtt_user), mqtt_pass(mqtt_pass),
	  bretained(bretained), iqos(iqos),
	cli("tcp://" + host + ":" + port, clientname+"V")
{
    connOpts.set_connect_timeout(90);
	connOpts.set_keep_alive_interval(60);
	connOpts.set_clean_session(true);
	connOpts.set_automatic_reconnect(true);
	connOpts.set_mqtt_version(MQTTVERSION_3_1_1);
	connOpts.set_user_name(mqtt_user);
	connOpts.set_password(mqtt_pass);
}