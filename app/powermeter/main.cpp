#include <getopt.h>
#include <csignal>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <thread>
#include "DeviceReader.hpp"
#include "MosquittoHub.hpp"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

constexpr auto SERVER_ADDRESS = "localhost";
constexpr auto CLIENT_ID = "test_id";
const int port = 1883;

std::once_flag MosquittoCpp::MosquittoCpp::init_once_;
std::atomic_bool run(true);

static void usage(int argc, char *argv[]);
static void parse_options(int argc, char *argv[]);
void waitForSignals(sigset_t& signalSet);
void registerSignals(sigset_t& signalSet);

int main(int argc, char **argv) {

	parse_options(argc, argv);
	
	try {

		MosquittoHub mhub(CLIENT_ID.c_str());
		std::string will_string{"will string"};
		mhub.will_set("/will/", will_string.length(), will_string.c_str());
		mhub.connect(SERVER_ADDRESS.c_str(), port);
	
		DeviceReader deviceReader(mhub, 1000);

		std::thread thread_mqtthub([&](){
			while(run.load(std::memory_order_acquire))
			{
				mhub.loop_forever(20);
				break;
				std::cout << "Error in loop" << std::endl;
			}
		});
		
		sigset_t signalSet;
		registerSignals(signalSet);
		waitForSignals(signalSet);
		run.store(false, std::memory_order_release); 
		std::cout << "\nExiting..." << std::endl;
		
		mhub.disconnect();

		std::cout << "Disconnected..." << std::endl;

		if(thread_mqtthub.joinable()) {
			thread_mqtthub.join();
		}

		} catch (std::runtime_error& re) {
				std::cout << re.what() << std::endl;
			exit(1);
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



void registerSignals(sigset_t& signalSet) {
	auto signalHandler = [](int sig) {};
	signal(SIGTERM, signalHandler);
	signal(SIGINT, signalHandler);

	sigemptyset(&signalSet);
	sigaddset(&signalSet, SIGINT);
	sigaddset(&signalSet, SIGTERM);
}
void waitForSignals(sigset_t& signalSet) {
	int exitSignal;
	sigwait(&signalSet, &exitSignal);
}
