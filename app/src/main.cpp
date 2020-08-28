#include <getopt.h>
#include <csignal>
#include <unistd.h>
#include <iostream>
#include "callbacktimer.h"
#include "device.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))


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
	Device d2(1, 1000);
	// Listen to ctrl+c and assert
	sigset_t signalSet;
	registerSignals(signalSet);
	waitForSignals(signalSet);
	std::cout << "\n\nExiting...\n\n" << std::endl;
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
