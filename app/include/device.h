/*
 * Device.h
 * August 2020
 */

#ifndef DEVICE_H_
#define DEVICE_H

#include <thread>
#include <functional>
#include <atomic>
#include <vector>
#include <utility>

constexpr int NR_OF_CHANNELS = 4;
constexpr auto DEVICE_PATH = "/sys/bus/iio/devices/iio:device";
constexpr auto FILE_NAME_CURRENT = "in_current";
constexpr auto FILE_NAME_VOLTAGE = "in_voltage";
constexpr auto FILE_NAME_POWER = "in_power";
constexpr auto FILE_NAME_ENERGY = "in_energy";
constexpr auto FILE_NAME_SAMPLING_FREQ = "in_sampling_frequency";


struct Value {
	std::pair<std::string, float> raw;
	std::pair<std::string, float> mean_raw;
	std::pair<std::string, float> scale;
	float scaled_value;
};

struct Channel {
	Value current;
	Value voltage;
	Value power;
	Value energy;
};


class Device
{
	public:
		Device(int device_number, int read_interval);

		void ReadChannels();
		~Device();

	private:
		int device_number_= 0;
		int read_interval_ = 1000;
		std::vector<Channel> channels_;

		std::atomic<bool> running_;
		std::thread worker_thread_;
		bool isRunning() const;

		void StartThread();
		void StopThread();

		void GenerateFileNames();
		std::string GenerateCommonFileName(int i, std::string filename);
		void printChannels();
};

#endif
