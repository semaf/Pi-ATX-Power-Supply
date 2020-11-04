/*
 * DeviceReader.h
 * August 2020
 */

#ifndef DEVICE_READER_H
#define DEVICE_READER_H

#include <thread>
#include <atomic>
#include <vector>
#include <functional>
#include <utility>
#include <mutex>
#include <condition_variable>
#include <jsoncpp/json/json.h>
#include "MosquittoHub.hpp"

constexpr int NR_OF_CHANNELS = 4;
constexpr auto IIO_DEVICE_PATH = "/sys/bus/iio/devices/";
constexpr auto DEVICE_PATH = "/sys/bus/iio/devices/iio:device";
constexpr auto NAME_CURRENT = "current";
constexpr auto NAME_VOLTAGE = "voltage";
constexpr auto NAME_POWER = "power";
constexpr auto NAME_ENERGY = "energy";
constexpr auto FILE_NAME_SAMPLING_FREQ = "in_sampling_frequency";
constexpr auto NAME_RAW = "raw";
constexpr auto NAME_MEAN_RAW = "mean_raw";
constexpr auto NAME_SCALE = "scale";
constexpr auto NAME_SCALED_VALUE = "scaled_value";
constexpr auto TOPIC_USB_CURRENTS = "/USB/currents/";

struct Value {
	std::string file_path;
	std::string name;
	float value;
};

struct ChannelElement {
	Value raw;
	Value mean_raw;
	Value scale;
	float scaled_value;
};

struct Channel {
	ChannelElement current;
	ChannelElement voltage;
	ChannelElement power;
	ChannelElement energy;
};

class Device {

	public:
		Device(int deviceNumber, const int nrOfChannels); 
		~Device(){};
		std::vector<Channel> getChannels(){ return channels_; }
		void setChannels(std::vector<Channel>& channels){ channels_ = channels; }
		void setDeviceNumber(int device_number){device_number_=device_number;}
		int getDeviceNumber(){return device_number_;}
		void ReadChannels(Json::Value& jsonValue);
		
	private:
		int device_number_;
		std::vector<Channel> channels_;
		Json::Value jsonChannels_;
		std::string GenerateCommonFileName(int i, std::string filename);
		void GenerateFileNames();
		void PrintChannels();
};

class DeviceReader
{
	public:
		DeviceReader(MosquittoHub &mhub, int readInterval);
		~DeviceReader();

	private:
		int read_interval_ = 1000;
		int number_of_devices_ = 0;

		std::vector<Device> devices_;
		std::function<void(const std::string&, const std::string&)> publishCallback;
		std::atomic<bool> running_;
		std::thread worker_thread_;
		bool isRunning() const;
		std::mutex condition_mutex_;
		std::condition_variable condition_var_;
		void StartThread();
		void StopThread();

		int FindNumberOfDevices(std::string path);
		void printChannels();
		void PublishChannels(Json::Value& channels);
};

#endif // DEVICE_READER_H
