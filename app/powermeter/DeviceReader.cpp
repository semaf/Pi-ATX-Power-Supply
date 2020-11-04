/* DeviceReader.cpp
 * August 2020
 */

#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "DeviceReader.hpp"

Device::Device(int deviceNumber, const int nrOfChannels): device_number_(deviceNumber), jsonChannels_(Json::objectValue) { 

	channels_.resize(nrOfChannels); 
	
	jsonChannels_[NAME_CURRENT] = Json::Value(Json::arrayValue);
	jsonChannels_[NAME_VOLTAGE] = Json::Value(Json::arrayValue);
	jsonChannels_[NAME_POWER] = Json::Value(Json::arrayValue);
	jsonChannels_[NAME_ENERGY] = Json::Value(Json::arrayValue);
	
	GenerateFileNames();
}

void Device::ReadChannels(Json::Value& jsonValue) {

	float voltage_scale;
	std::ifstream file;

	file.open(channels_[0].voltage.scale.file_path, std::ifstream::in);
	file >> voltage_scale;
	file.close();
	Json::Value jsonChannels_;

	Json::Value current_array = Json::Value(Json::arrayValue);
	Json::Value voltage_array = Json::Value(Json::arrayValue);
	Json::Value power_array = Json::Value(Json::arrayValue);
	Json::Value energy_array = Json::Value(Json::arrayValue);
	
	for(std::vector<Channel>::iterator it = channels_.begin(); it != channels_.end(); ++it) {
		float value;
		float raw, mean_raw, scale;

		Json::Value current;
		Json::Value voltage;
		Json::Value power;
		Json::Value energy;

		file.open(it->current.raw.file_path, std::ifstream::in);
		file >> raw;
		it->current.raw.value = raw;
		file.close();
		
		current[it->current.raw.name] = raw;

		file.open(it->current.mean_raw.file_path, std::ifstream::in);
		file >> mean_raw;
		it->current.mean_raw.value = mean_raw;
		file.close();
		
		current[it->current.mean_raw.name] = mean_raw;		
		
		file.open(it->current.scale.file_path, std::ifstream::in);
		file >> scale;
		it->current.scale.value = scale;
		it->current.scaled_value = raw * scale; 
		file.close();

		current[it->current.scale.name] = scale;
		current[NAME_SCALED_VALUE] = it->current.scaled_value;
		
	    current_array.append(current);	
		
		file.open(it->voltage.raw.file_path, std::ifstream::in);
		file >> raw;
		it->voltage.raw.value = raw;
		file.close();
		
		voltage[it->voltage.raw.name] = raw;

		file.open(it->voltage.mean_raw.file_path, std::ifstream::in);
		file >> mean_raw;
		it->voltage.mean_raw.value = mean_raw;
		file.close();
		
		voltage[it->voltage.mean_raw.name] = mean_raw;

		it->voltage.scale.value = voltage_scale;
		it->voltage.scaled_value = raw * voltage_scale;

		voltage[it->voltage.scale.name] = voltage_scale;
		voltage[NAME_SCALED_VALUE] = it->voltage.scaled_value;
		
	    voltage_array.append(voltage);	
		
		file.open(it->power.raw.file_path, std::ifstream::in);
		file >> raw;
		it->power.raw.value =  raw;
		file.close();
		
		power[it->power.raw.name] = raw;

		file.open(it->power.scale.file_path, std::ifstream::in);
		file >> scale;
		it->power.scale.value = scale;
		it->power.scaled_value = raw * scale;
		file.close();
		
		power[it->power.scale.name] = scale;
		power[NAME_SCALED_VALUE] = it->power.scaled_value;
		
        power_array.append(power);

		file.open(it->energy.mean_raw.file_path, std::ifstream::in);
		file >> mean_raw;
		it->energy.mean_raw.value = mean_raw;
		file.close();
		
		energy[it->energy.mean_raw.name] = mean_raw;

		file.open(it->energy.scale.file_path, std::ifstream::in);
		file >> scale;
		it->energy.scale.value = scale;
		it->energy.scaled_value = mean_raw * scale;
		file.close();
		
		energy[it->energy.scale.name] = scale;
		energy[NAME_SCALED_VALUE] = it->energy.scaled_value;

		energy_array.append(energy);
		
	}
	
	jsonValue[NAME_CURRENT] = current_array;
	jsonValue[NAME_VOLTAGE] = voltage_array;
	jsonValue[NAME_POWER] = power_array;
	jsonValue[NAME_ENERGY] = energy_array;
}



std::string Device::GenerateCommonFileName(int channelNumber, std::string filename) {

	return std::string(DEVICE_PATH) + std::to_string(device_number_) + "/in_" + filename + std::to_string(channelNumber) + "_";
}

void Device::GenerateFileNames() {
	for(int i = 0; i < NR_OF_CHANNELS; ++i) {

		std::string common_file_name_current = GenerateCommonFileName(i, NAME_CURRENT);

		channels_[i].current.raw.file_path = common_file_name_current + NAME_RAW;
		channels_[i].current.mean_raw.file_path = common_file_name_current + NAME_MEAN_RAW;
		channels_[i].current.scale.file_path = common_file_name_current + NAME_SCALE;

		channels_[i].current.raw.name = NAME_RAW;;
		channels_[i].current.mean_raw.name = NAME_MEAN_RAW;
		channels_[i].current.scale.name = NAME_SCALE;

		std::string common_file_name_voltage = GenerateCommonFileName(i, NAME_VOLTAGE);

		channels_[i].voltage.raw.file_path = common_file_name_voltage + NAME_RAW;
		channels_[i].voltage.mean_raw.file_path = common_file_name_voltage + NAME_MEAN_RAW;

		channels_[i].voltage.raw.name = NAME_RAW;
		channels_[i].voltage.mean_raw.name = NAME_MEAN_RAW;
		
		// delete last character, because scale for voltage is the same for all channels
		common_file_name_voltage.pop_back();
		channels_[i].voltage.scale.file_path = common_file_name_voltage + NAME_SCALE;
		channels_[i].voltage.scale.name = NAME_SCALE;

		std::string common_file_name_power = GenerateCommonFileName(i, NAME_POWER);

		channels_[i].power.raw.file_path = common_file_name_power + NAME_RAW;
		channels_[i].power.scale.file_path = common_file_name_power + NAME_SCALE;

		channels_[i].power.raw.name = NAME_RAW;
		channels_[i].power.scale.name = NAME_SCALE;

		std::string common_file_name_energy = GenerateCommonFileName(i, NAME_ENERGY);

		channels_[i].energy.mean_raw.file_path = common_file_name_energy + NAME_MEAN_RAW;
		channels_[i].energy.scale.file_path = common_file_name_energy + NAME_SCALE;

		channels_[i].energy.mean_raw.name = NAME_MEAN_RAW;
		channels_[i].energy.scale.name = NAME_SCALE;
	}
}

void Device::PrintChannels() {
	int device_number_;
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

	header.replace(0, 20, "DeviceReader " + std::to_string(device_number_) + " channels ");
	header.replace(26, 8, "  ch 0  ");
	header.replace(42, 8, "  ch 1  ");
	header.replace(58, 8, "  ch 2  ");
	header.replace(74, 8, "  ch 3  ");

	std::cout << "\n" << header << std::endl;

	for(int i = 0; i < channels_.size(); ++i) {
		current_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].current.raw.value << " ";
		current_mean_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].current.mean_raw.value << " ";
		current_scale << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].current.scale.value  << " ";
		current_scaled << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].current.scaled_value << " ";
		voltage_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].voltage.raw.value << " ";
		voltage_mean_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].voltage.mean_raw.value << " ";
		voltage_scale << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].voltage.scale.value << " ";
		voltage_scaled << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].voltage.scaled_value << " ";
		power_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].power.raw.value << " ";
		power_scale << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].power.scale.value << " ";
		power_scaled << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].power.scaled_value << " ";
		energy_mean_raw << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].energy.mean_raw.value << " ";
		energy_scale << std::setw(VALUE_WIDTH) << std::setprecision(PRECISION_WIDTH) << std::right << std::fixed << channels_[i].energy.scale.value << " ";
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

DeviceReader::DeviceReader(MosquittoHub& mhub, int read_interval): publishCallback(std::bind(&MosquittoHub::publish, &mhub, std::placeholders::_1, std::placeholders::_2)), read_interval_(read_interval), running_(false) {
	
	std::string iio_path(IIO_DEVICE_PATH);
	number_of_devices_ = FindNumberOfDevices(std::string(IIO_DEVICE_PATH)); 
	
	if(number_of_devices_ == 0) {
		throw std::runtime_error("No device detected!");
	}	

	for(int i = 0; i < number_of_devices_; ++i) {
		devices_.emplace_back(i, NR_OF_CHANNELS);
	}

	StartThread();
}

DeviceReader::~DeviceReader() {
	StopThread();
}

void DeviceReader::StartThread() {
	
	static Json::Value jsonChannels;
	
	std::function<void()> threadFunction = [&]() {
	
		while(running_.load(std::memory_order_acquire)) {
				
			jsonChannels[NAME_CURRENT] = Json::arrayValue; 
			jsonChannels[NAME_VOLTAGE] = Json::arrayValue; 
			jsonChannels[NAME_POWER] = Json::arrayValue; 
			jsonChannels[NAME_ENERGY] = Json::arrayValue; 

			for(int i = 0; i < devices_.size(); ++i) {
				Json::Value jsonValue;
				devices_.at(i).ReadChannels(jsonValue);
				
				for(Json::Value::ArrayIndex j = 0; j != jsonValue.size(); ++j) {
					jsonChannels[NAME_CURRENT][NR_OF_CHANNELS * i + j] = jsonValue[NAME_CURRENT][j];
					jsonChannels[NAME_VOLTAGE][NR_OF_CHANNELS * i + j] = jsonValue[NAME_VOLTAGE][j];
					jsonChannels[NAME_POWER][NR_OF_CHANNELS * i + j] = jsonValue[NAME_POWER][j];
					jsonChannels[NAME_ENERGY][NR_OF_CHANNELS * i + j] = jsonValue[NAME_ENERGY][j];
				}
			}
			//std::cout << jsonChannels.toStyledString() << std::endl;
			Json::FastWriter fastWriter;
			std::string jsonOutput = fastWriter.write(jsonChannels);
				
			publishCallback(TOPIC_USB_CURRENTS, jsonOutput);

			std::this_thread::sleep_for(std::chrono::milliseconds(read_interval_));
		}
	};

	running_.store(true);
			
	worker_thread_ = std::thread(threadFunction);
}

void DeviceReader::StopThread() {
	running_.store(false, std::memory_order_release);
	if(worker_thread_.joinable())
		worker_thread_.join();
}

// Device files are symlinks. Count symlinks to find number of devices.
int DeviceReader::FindNumberOfDevices(std::string dir_path)
{
	boost::filesystem::path fs_path(dir_path);

	if(!boost::filesystem::exists(fs_path) || boost::filesystem::is_empty(fs_path))
		return 0;
    
	return std::count_if(boost::filesystem::directory_iterator(fs_path), boost::filesystem::directory_iterator(), 
		[&](const boost::filesystem::path& p) { return boost::filesystem::is_symlink(p); });
}


