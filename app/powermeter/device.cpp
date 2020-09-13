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

void Device::ReadChannels() {

	float voltage_scale;
	std::ifstream file;

	file.open(channels_[0].voltage.scale.first, std::ifstream::in);
	file >> voltage_scale;
	file.close();

	for(std::vector<Channel>::iterator it = channels_.begin(); it != channels_.end(); ++it) {
		float value;
		float raw, mean_raw, scale;


		file.open(it->current.raw.first, std::ifstream::in);
		file >> raw;
		it->current.raw.second = raw;
		file.close();

		file.open(it->current.mean_raw.first, std::ifstream::in);
		file >> mean_raw;
		it->current.mean_raw.second = mean_raw;
		file.close();

		file.open(it->current.scale.first, std::ifstream::in);
		file >> scale;
		it->current.scale.second = scale;
		it->current.scaled_value = raw * scale; 
		file.close();

		file.open(it->voltage.raw.first, std::ifstream::in);
		file >> raw;
		it->voltage.raw.second = raw;
		file.close();

		file.open(it->voltage.mean_raw.first, std::ifstream::in);
		file >> mean_raw;
		it->voltage.mean_raw.second = mean_raw;
		file.close();

		it->voltage.scale.second = voltage_scale;
		it->voltage.scaled_value = raw * voltage_scale;

		file.open(it->power.raw.first, std::ifstream::in);
		file >> raw;
		it->power.raw.second =  raw;
		file.close();

		file.open(it->power.scale.first, std::ifstream::in);
		file >> scale;
		it->power.scale.second = scale;
		it->power.scaled_value = raw * scale;
		file.close();

		file.open(it->energy.mean_raw.first, std::ifstream::in);
		file >> mean_raw;
		it->energy.mean_raw.second = mean_raw;
		file.close();

		file.open(it->energy.scale.first, std::ifstream::in);
		file >> scale;
		it->energy.scale.second = scale;
		it->energy.scaled_value = mean_raw * scale;
		file.close();
	}
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
	const auto LABEL_WIDTH = 20;
	const auto VALUE_WIDTH = 15;
	const auto PRECISION_WIDTH = 5;

	std::ostringstream current_raw;
	std::ostringstream current_mean_raw;
	std::ostringstream current_scale;
	std::ostringstream current_scaled;
	std::ostringstream voltage_raw;
	std::ostringstream voltage_mean_raw;
	std::ostringstream voltage_scale;
	std::ostringstream voltage_scaled;
	std::ostringstream power_raw;
	std::ostringstream power_scale;
	std::ostringstream power_scaled;
	std::ostringstream energy_mean_raw;
	std::ostringstream energy_scale;
	std::ostringstream energy_scaled;

	current_raw << std::setw(LABEL_WIDTH) << std::left << "Current raw" << " : " << std::setprecision(4) << std::right;
	current_mean_raw << std::setw(LABEL_WIDTH) << std::left << "Current mean raw" << " : " << std::setprecision(4) << std::right; 
	current_scale << std::setw(LABEL_WIDTH) << std::left << "Current scale" << " : " << std::setprecision(4) << std::right; 
	current_scaled << std::setw(LABEL_WIDTH) << std::left << "Scaled current" << " : " << std::setprecision(4) << std::right; 

	voltage_raw << std::setw(LABEL_WIDTH) << std::left << "Voltage raw" << " : " << std::setprecision(4) << std::right; 
	voltage_mean_raw << std::setw(LABEL_WIDTH) << std::left << "Voltage mean raw" << " : " << std::setprecision(4) << std::right; 
	voltage_scale << std::setw(LABEL_WIDTH) << std::left << "Voltage scale" << " : " << std::setprecision(4) << std::right; 
	voltage_scaled << std::setw(LABEL_WIDTH) << std::left << "Scaled voltage" << " : " << std::setprecision(4) << std::right; 

	power_raw << std::setw(LABEL_WIDTH) << std::left << "Power raw" << " : " << std::setprecision(4) << std::right; 
	power_scale << std::setw(LABEL_WIDTH) << std::left << "Power scale" << " : " << std::setprecision(4) << std::right; 
	power_scaled << std::setw(LABEL_WIDTH) << std::left << "Scaled power" << " : " << std::setprecision(4) << std::right; 

	energy_mean_raw << std::setw(LABEL_WIDTH) << std::left << "Energy mean raw" << " : " << std::setprecision(4) << std::right; 
	energy_scale << std::setw(LABEL_WIDTH) << std::left << "Energy scale" << " : " << std::setprecision(4) << std::right; 
	energy_scaled << std::setw(LABEL_WIDTH) << std::left << "Scaled energy" << " : " << std::setprecision(4) << std::right; 

	std::string header(90, '-');

	header.replace(0, 20, "Device " + std::to_string(device_number_) + " channels ");
	header.replace(26, 8, "  ch 0  ");
	header.replace(42, 8, "  ch 1  ");
	header.replace(58, 8, "  ch 2  ");
	header.replace(74, 8, "  ch 3  ");

	std::cout << "\n" << header << std::endl;

	for(int i = 0; i < channels_.size(); ++i) {
		current_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].current.raw.second << " ";
		current_mean_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].current.mean_raw.second << " ";
		current_scale << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].current.scale.second  << " ";
		current_scaled << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].current.scaled_value << " ";
		voltage_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].voltage.raw.second << " ";
		voltage_mean_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].voltage.mean_raw.second << " ";
		voltage_scale << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].voltage.scale.second << " ";
		voltage_scaled << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].voltage.scaled_value << " ";
		power_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].power.raw.second << " ";
		power_scale << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].power.scale.second << " ";
		power_scaled << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].power.scaled_value << " ";
		energy_mean_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].energy.mean_raw.second << " ";
		energy_scale << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].energy.scale.second << " ";
		energy_scaled << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].energy.scaled_value << " ";
	}

	std::cout << current_raw.str() << std::endl << current_mean_raw.str() << std::endl << \
		current_scale.str() << std::endl << current_scaled.str() << std::endl << \
		voltage_raw.str() << std::endl << voltage_mean_raw.str() << std::endl << \
		voltage_scale.str() << std::endl << voltage_scaled.str() << std::endl << \

		power_raw.str() << std::endl << power_scale.str() << std::endl << \
		power_scaled.str() << std::endl << energy_mean_raw.str() << std::endl << \
		energy_scale.str() << std::endl <<  energy_scaled.str() << std::endl; 

}

