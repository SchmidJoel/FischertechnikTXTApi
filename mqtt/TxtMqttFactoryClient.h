#ifndef TxtMqttFactoryClient_H_
#define TxtMqttFactoryClient_H_

#include <mqtt/client.h>


class TxtMqttFactoryClient {
public:
	TxtMqttFactoryClient(std::string clientname, std::string host, std::string port,
			std::string mqtt_user, mqtt::binary_ref mqtt_pass, bool bretained=false, int iqos=1);
	virtual ~TxtMqttFactoryClient();

	bool is_connected() { return cli.is_connected(); }

	bool connect(long int timeout);
	void disconnect(long int timeout);

protected:

	std::string clientname;
	mqtt::string host;
	mqtt::string port;
	mqtt::string mqtt_user;
	mqtt::binary_ref mqtt_pass;
	bool bretained;
	int iqos;
	mqtt::async_client cli;
	mqtt::connect_options connOpts;
};

#endif