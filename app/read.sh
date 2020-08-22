#!/bin/bash


DEVICE_PATH=/sys/bus/iio/devices/iio:device1/
CMD=$(cat ${DEVICE_PATH}in_current0_raw)

export DEVICE_PATH
watch -n 1 'printf "%-20s: %12.5f\n\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n\n\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n%-20s: %12.5f\n" \
	   "in_voltage_scale" "$(cat "${DEVICE_PATH}in_voltage_scale")" \
	   "in_current0_raw" "$(cat "${DEVICE_PATH}in_current0_raw")" \
	   "in_current0_mean_raw" "$(cat "${DEVICE_PATH}in_current0_mean_raw")" \
	   "in_current0_scale" "$(cat "${DEVICE_PATH}in_current0_scale")" \
	   "in_voltage0_raw" "$(cat "${DEVICE_PATH}in_voltage0_raw")" \
	   "in_voltage0_mean_raw" "$(cat "${DEVICE_PATH}in_voltage0_mean_raw")" \
	   "in_power0_raw" "$(cat "${DEVICE_PATH}in_power0_raw")" \
	   "in_power0_scale" "$(cat "${DEVICE_PATH}in_power0_scale")" \
	   "in_energy0_mean_raw" "$(cat "${DEVICE_PATH}in_energy0_mean_raw")" \
	   "in_energy0_scale" "$(cat "${DEVICE_PATH}in_energy0_scale")" \
	   "in_current1_raw" "$(cat "${DEVICE_PATH}in_current1_raw")" \
	   "in_current1_mean_raw" "$(cat "${DEVICE_PATH}in_current1_mean_raw")" \
	   "in_current1_scale" "$(cat "${DEVICE_PATH}in_current1_scale")" \
	   "in_voltage1_raw" "$(cat "${DEVICE_PATH}in_voltage1_raw")" \
	   "in_voltage1_mean_raw" "$(cat "${DEVICE_PATH}in_voltage1_mean_raw")" \
	   "in_power1_raw" "$(cat "${DEVICE_PATH}in_power1_raw")" \
	   "in_power1_scale" "$(cat "${DEVICE_PATH}in_power1_scale")" \
	   "in_energy1_mean_raw" "$(cat "${DEVICE_PATH}in_energy1_mean_raw")" \
	   "in_energy1_scale" "$(cat "${DEVICE_PATH}in_energy1_scale")" \
	   '

#while true; do
#	printf '%-30s: %i\r' "in_current0_raw" ${CMD}
#	sleep 1
#done
#&& echo -n \"\nin_current0_mean_raw:\" && cat ${DEVICE_PATH}in_current0_mean_raw && echo -n \"in_current0_scale:\" && cat ${DEVICE_PATH}in_current0_scale"
