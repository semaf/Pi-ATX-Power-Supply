/* Device.cpp
 * August 2020
 */

#include "device.h"
#include <fstream>
#include <iostream>
#include <iomanip>

Device::Device(int device_number, int read_interval): device_number_(device_number), read_interval_(read_interval), running_(false) {
    channels_.resize(NR_OF_CHANNELS);
    GenerateFileNames();
    StartThread();
}
Device::~Device() {
    StopThread();
}
void Device::StartThread() {
    if(running_.load(std::memory_order_acquire)) {
        StopThread();
    }
    running_.store(true, std::memory_order_release);
    worker_thread_ = std::thread([this]()

    {
        while(running_.load(std::memory_order_acquire)) {
            ReadChannels();
            printChannels();
            std::this_thread::sleep_for(std::chrono::milliseconds(read_interval_));
        }
    });
}
void Device::StopThread() {
    running_.store(false, std::memory_order_release);
    if(worker_thread_.joinable())
        worker_thread_.join();
}
std::vector<Channel> Device::ReadChannels() {

    float voltage_scale;
    std::ifstream file;

    file.open(channels_[0].voltage.scale.first, std::ifstream::in);
    file >> voltage_scale;
    file.close();

    for(std::vector<Channel>::iterator it = channels_.begin(); it != channels_.end(); ++it) {
        float value;


        file.open(it->current.raw.first, std::ifstream::in);
        file >> value;
        it->current.raw.second = value;
        file.close();

        file.open(it->current.mean_raw.first, std::ifstream::in);
        file >> value;
        it->current.mean_raw.second = value;
        file.close();

        file.open(it->current.scale.first, std::ifstream::in);
        file >> value;
        it->current.scale.second = value;
        file.close();

        file.open(it->voltage.raw.first, std::ifstream::in);
        file >> value;
        it->voltage.raw.second = value;
        file.close();

        file.open(it->voltage.mean_raw.first, std::ifstream::in);
        file >> value;
        it->voltage.mean_raw.second = value;
        file.close();

	it->voltage.scale.second = voltage_scale;
       
       	file.open(it->power.raw.first, std::ifstream::in);
        file >> value;
        it->power.raw.second = value;
        file.close();

        file.open(it->power.scale.first, std::ifstream::in);
        file >> value;
        it->power.scale.second = value;
        file.close();

        file.open(it->energy.mean_raw.first, std::ifstream::in);
        file >> value;
        it->energy.mean_raw.second = value;
        file.close();

        file.open(it->energy.scale.first, std::ifstream::in);
        file >> value;
        it->energy.scale.second = value;
        file.close();
    }
    return std::vector<Channel>();
}

std::string Device::GenerateCommonFileName(int i, std::string filename) {

    return std::string(DEVICE_PATH) + std::to_string(device_number_) + "/" + filename + std::to_string(i);
}

void Device::GenerateFileNames() {
    for(int i = 0; i < NR_OF_CHANNELS; ++i) {

        std::string common_file_name_current = GenerateCommonFileName(i, FILE_NAME_CURRENT);

        channels_[i].current.raw = std::make_pair(common_file_name_current + "_raw", 0);
        channels_[i].current.mean_raw = std::make_pair(common_file_name_current + "_mean_raw", 0);
        channels_[i].current.scale = std::make_pair(common_file_name_current + "_scale", 0);


        std::string common_file_name_voltage = GenerateCommonFileName(i, FILE_NAME_VOLTAGE);

        channels_[i].voltage.raw = std::make_pair(common_file_name_voltage + "_raw", 0);
        channels_[i].voltage.mean_raw = std::make_pair(common_file_name_voltage + "_mean_raw", 0);

        // delete last character, because scale for voltage is the same for all channels
        common_file_name_voltage.pop_back();
        channels_[i].voltage.scale = std::make_pair(common_file_name_voltage + "_scale", 0);

        std::string common_file_name_power = GenerateCommonFileName(i, FILE_NAME_POWER);

        channels_[i].power.raw = std::make_pair(common_file_name_power + "_raw", 0);
        channels_[i].power.scale = std::make_pair(common_file_name_power + "_scale", 0);

        std::string common_file_name_energy = GenerateCommonFileName(i, FILE_NAME_ENERGY);

        channels_[i].energy.mean_raw = std::make_pair(common_file_name_energy + "_mean_raw", 0);
        channels_[i].energy.scale = std::make_pair(common_file_name_energy + "_scale", 0);

    }
}

void Device::printChannels() {

    std::cout << "\n----- Channels data ---- device " << device_number_ << "\n";

    for(int i = 0; i < channels_.size(); ++i) {
	    std::cout << "\n------ Channel ------ " << i << std::endl;
        std::cout << std::setw(55) << std::left << channels_[i].current.raw.first << " : "      << channels_[i].current.raw.second << std::endl;
        std::cout << std::setw(55) << std::left << channels_[i].current.mean_raw.first << " : " << channels_[i].current.mean_raw.second << std::endl;
        std::cout << std::setw(55) << std::left << channels_[i].current.scale.first << " : "    << channels_[i].current.scale.second  << std::endl;
        std::cout << std::setw(55) << std::left << channels_[i].voltage.raw.first << " : "      << channels_[i].voltage.raw.second << std::endl;
        std::cout << std::setw(55) << std::left << channels_[i].voltage.mean_raw.first << " : " << channels_[i].voltage.mean_raw.second << std::endl;
        std::cout << std::setw(55) << std::left << channels_[i].voltage.scale.first << " : "    << channels_[i].voltage.scale.second << std::endl;
        std::cout << std::setw(55) << std::left << channels_[i].power.raw.first << " : "        << channels_[i].power.raw.second << std::endl;
        std::cout << std::setw(55) << std::left << channels_[i].power.scale.first << " : "      << channels_[i].power.scale.second << std::endl;
        std::cout << std::setw(55) << std::left << channels_[i].energy.mean_raw.first << " : "  << channels_[i].energy.mean_raw.second << std::endl;
        std::cout << std::setw(55) << std::left << channels_[i].energy.scale.first << " : "     << channels_[i].energy.scale.second << std::endl;
    }
}

