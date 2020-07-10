!/bin/bash
source /opt/ros/kinetic/setup.bash

roslaunch tank_bot tank_bot.launch

echo "launched tankbot" > startup.txt
