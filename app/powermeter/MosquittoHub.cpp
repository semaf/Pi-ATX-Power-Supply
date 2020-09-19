#include "MosquittoHub.hpp"
#include <iostream>


MosquittoHub::MosquittoHub(const char *id) : MosquittoCpp::MosquittoCpp(id, true){}

void MosquittoHub::connect(std::string serverAddress, int port) {
	MosquittoCpp::MosquittoCpp::connect(serverAddress.c_str(), port, 60);
	server_address_ = serverAddress;
}

void MosquittoHub::publish(const std::string& topic, const std::string& payload) {
	MosquittoCpp::MosquittoCpp::publish(nullptr, topic.c_str(), payload.length(), payload.c_str(), 0, false);
}

void MosquittoHub::onConnect(int rc) {
	std::cout << __PRETTY_FUNCTION__ << "connected to MQTT host " << server_address_ << std::endl;
}

void MosquittoHub::subscribeTopic(const std::string& topic) {
	int mid;
	MosquittoCpp::MosquittoCpp::subscribe(&mid, topic.c_str(), 0);
}

