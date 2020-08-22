#!/bin/bash

gpio mode 22 out

if [ $1 = "on" ] 
then
	gpio write 22 1 && dtoverlay pac193x addr=0x10 && dtoverlay pac193x addr=0x1f
else
	dtoverlay -r && dtoverlay -r && gpio write 22 0
fi

