#include <getopt.h>
#include <csignal>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <thread>
#include "device.h"
#include "MosquittoHub.hpp"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
const std::string SERVER_ADDRESS   { "localhost" };
const std::string CLIENT_ID        { "test_id" };
const int port = 1883;
const std::string TOPIC { "/hello/" };
const std::string MESSAGE { "{USB1:{V:5,I=1}}" };


std::once_flag MosquittoCpp::MosquittoCpp::init_once_;
std::atomic_bool run(true);

static void usage(int argc, char *argv[]);
static void parse_options(int argc, char *argv[]);
void waitForSignals(sigset_t& signalSet);
void registerSignals(sigset_t& signalSet);
void signalHandler(int sig);

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
	mhub.connect(SERVER_ADDRESS.c_str(), port);
	std::cout << "connected..." << std::endl;

	std::thread thread_mqtthub([&](){
	while(run.load(std::memory_order_acquire))
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
		while(run.load(std::memory_order_acquire))
			{
				mhub.publish(TOPIC, MESSAGE);
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				std::cout << "TH ID " << std::this_thread::get_id() << " run: " << run.load() << std::endl;

			}
		});

	sigset_t signalSet;
	registerSignals(signalSet);
	waitForSignals(signalSet);
	std::cout << "\n\nExiting...\n\n" << std::endl;
	
	mhub.disconnect();

	std::cout << "\nDisconnected...\n\n" << std::endl;

	if(thread_mqtthub.joinable()) {
		
		thread_mqtthub.join();
	}

	if(thread_device_read.joinable()) {
	std::cout << "\nbefore join...\n\n" << std::endl;
		thread_device_read.join();
	std::cout << "\nafter join...\n\n" << std::endl;
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


void signalHandler(int sig) { run.store(false, std::memory_order_release); 
	std::cout << "signal run: " << run << std::endl;}

void registerSignals(sigset_t& signalSet) {
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
