#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#include "json/json.h"
#include <fstream>
#include <string>

std::string ip_adress = "192.168.178.66";
int red_lower = 1600;
int blue_lower = 1200;

void readConfig(){
    Json::Value root;
    std::ifstream ifs;
    ifs.open("config.json");
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &root, &errs)) {
        return;
    }
    ip_adress = root["ip_adress"].asString();
    red_lower = root["red_lower"].asInt();
    blue_lower = root["blue_lower"].asInt();
}

#endif