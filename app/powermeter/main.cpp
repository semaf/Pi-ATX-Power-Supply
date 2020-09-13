#include <getopt.h>
#include <csignal>
#include <unistd.h>
#include <iostream>
#include "device.h"
#include <mosquitto++.hpp>
#include <mutex>
#include <thread>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
const std::string SERVER_ADDRESS   { "localhost" };
const std::string CLIENT_ID        { "test_id" };
const int port = 1883;
const std::string TOPIC { "/hello/" };
const std::string MESSAGE { "{USB1:{V:5,I=1}}" };

class MosquittoHub : public MosquittoCpp::MosquittoCpp {
	public:
		MosquittoHub(const char *id): MosquittoCpp::MosquittoCpp(id, true){}
		virtual ~MosquittoHub() {}
		virtual void connect() {
			MosquittoCpp::MosquittoCpp::connect(SERVER_ADDRESS.c_str(), port, 60);
		}
		virtual void publish(const std::string& topic, const std::string& payload) {
			MosquittoCpp::MosquittoCpp::publish(nullptr, topic.c_str(), payload.length(), payload.c_str(), 0, false);
		}
		virtual void onConnect(int rc) override {
			std::cout << __PRETTY_FUNCTION__ << "connected to MQTT host " << SERVER_ADDRESS << std::endl;
		}
		virtual void subscribeTopic(const std::string& topic) {
			int mid;
			MosquittoCpp::MosquittoCpp::subscribe(&mid, topic.c_str(), 0);
		}
};

std::once_flag MosquittoCpp::MosquittoCpp::init_once_;

static void usage(int argc, char *argv[]);
static void handle_signal(int sig);
static void parse_options(int argc, char *argv[]);
void waitForSignals(sigset_t& signalSet);
void registerSignals(sigset_t& signalSet);


void periodicFunction(void)
{
	std::cout << "Time elapsed!" << std::endl;
}

int main(int argc, char **argv) {

	parse_options(argc, argv);

	//Device d1(0, 1000);
	//Device d2(1, 1000);

	MosquittoHub mhub(CLIENT_ID.c_str());
	std::string will_string{"will string"};
	mhub.will_set("/will/", will_string.length(), will_string.c_str());
	mhub.connect();
	std::cout << "connected..." << std::endl;

	std::thread thread_mqtthub([&mhub](){
		while(true)
		{
			try {
				mhub.loop_forever(20);
				break;
			} catch(...)
			{
				std::cout << "Error in loop" << std::endl;
			}
		}
	});

	std::thread thread_device_read([&](){
			while(true)
			{
				mhub.publish(TOPIC, MESSAGE);
				std::this_thread::sleep_for(std::chrono::seconds(1));

			}
		});

	sigset_t signalSet;
	registerSignals(signalSet);
	waitForSignals(signalSet);
	std::cout << "\n\nExiting...\n\n" << std::endl;
	mhub.disconnect();
	if(thread_mqtthub.joinable()) {
		thread_mqtthub.join();
	}
	if(thread_device_read.joinable()) {
		thread_device_read.join();
	}
exit(0);
}


static void parse_options(int argc, char *argv[]) {
	int c;
	while ((c = getopt(argc, argv, "ht")) != -1) {
		switch (c) {
			case 't':
				std::cout << "t" << std::endl;
				break;
			case 'h':
			default:
				usage(argc, argv);
				exit(1);
		}
	}
}


static void usage(int argc, char *argv[]) {
	printf("Usage: %s [OPTION]\n", argv[0]);
	printf("  -d\tdevice name (default \"iio_dummy_part_no\")\n");
	printf("  -t\ttrigger name (default \"instance1\")\n");
	printf("  -b\tbuffer length (default 1)\n");
	printf("  -r\tread method (default 0 pointer, 1 callback, 2 read raw, 3 read)\n");
	printf("  -c\tread count (default no limit)\n");
}

static void handle_signal(int sig) {
	printf("Waiting for process to finish... got signal : %d\n", sig);
}



void registerSignals(sigset_t& signalSet) {
	auto SignalHandler = [](int){};
	signal(SIGTERM, SignalHandler);
	signal(SIGINT, SignalHandler);

	sigemptyset(&signalSet);
	sigaddset(&signalSet, SIGINT);
	sigaddset(&signalSet, SIGTERM);
}
void waitForSignals(sigset_t& signalSet) {
	int exitSignal;
	sigwait(&signalSet, &exitSignal);
}
