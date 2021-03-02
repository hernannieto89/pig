#!/bin/bash

echo "17" > /sys/class/gpio/export;
echo "27" > /sys/class/gpio/export;
echo "22" > /sys/class/gpio/export;
echo "10" > /sys/class/gpio/export;
echo "9" > /sys/class/gpio/export;
echo "out" > /sys/class/gpio/gpio17/direction;
echo "out" > /sys/class/gpio/gpio27/direction;
echo "out" > /sys/class/gpio/gpio22/direction;
echo "out" > /sys/class/gpio/gpio10/direction;
echo "out" > /sys/class/gpio/gpio9/direction;
echo "0" > /sys/class/gpio/gpio17/value;
echo "0" > /sys/class/gpio/gpio27/value;
echo "0" > /sys/class/gpio/gpio22/value;
echo "0" > /sys/class/gpio/gpio10/value;
echo "0" > /sys/class/gpio/gpio9/value;

bash -c "source /home/pi/.pyenv/versions/scheduler/bin/activate && python /home/pi/Desktop/pig.scheduler/main.py > /home/pi/Desktop/sch.log 2>&1";
bash -c "source /home/pi/.pyenv/versions/pig/bin/activate && cd /home/pi/Desktop/pig.station/ && flask run -p 5234 > /home/pi/Desktop/pig.log 2>&1";
bash -c "source /home/pi/.pyenv/versions/hub/bin/activate && cd /home/pi/Desktop/pig.hub/ && flask run -p 5001 -h 0.0.0.0 > /home/pi/Desktop/hub.log 2>&1";
http get localhost:5234/ping;