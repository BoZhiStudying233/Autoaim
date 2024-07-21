#! /bin/bash
# gnome-terminal -e 'bash -c "/home/nuc/sh/watchdog.sh" '

echo "nuc" | sudo -S chmod 777 /dev/bus/usb/001/*
echo "nuc" | sudo -s chmod 777 /dev/bus/usb/002/*
echo "nuc" | sudo -s chmod 777 /dev/bus/usb/003/*
echo "nuc" | sudo -s chmod 777 /dev/bus/usb/004/*
gnome-terminal -e 'bash -c "source /opt/ros/foxy/setup.bash;source /opt/intel/openvino_2022.3.0/setupvars.sh;source $(pwd)/../install/setup.bash;cd $(pwd)/..;ros2 launch rmos_bringup normal_aim.launch.py " '

sleep 6

while true
do

cam_pid=$(ps -ef | grep "daheng_camera" | grep -v grep | awk '{print $2}')
detector_pid=$(ps -ef | grep "basic_detector" | grep -v grep | awk '{print $2}')
processer_pid=$(ps -ef | grep "processer" | grep -v grep | awk '{print $2}')
comm_pid=$(ps -ef | grep "usb_comm" | grep -v grep | awk '{print $2}')

if [ $cam_pid -ne 0 ] && [ $detector_pid -ne 0 ] && [ $processer_pid -ne 0 ] && [ $comm_pid -ne 0]
then                 #如果程序PID在，则程序在运行
        echo `date`
        echo "all_pid is exitable!"

else
	echo `date`
        echo "cam_pid isnot exitable!"
        {
        	echo "nuc" | sudo -S kill -9 $detector_pid
        } || 
        {
        	echo "$detector_pid isnot find!"
        }
        {
        	echo "nuc" | sudo -S kill -9 $processer_pid
        } || 
        {
        	echo "$processer_pid isnot find!"
        }
        {
        	echo "nuc" | sudo -S kill -9 $comm_pid
        } || 
        {
        	echo "$comm_pid isnot find!"
        }
        {
        	echo "nuc" | sudo -S kill -9 $cam_pid
        } || 
        {
        	echo "$cam_pid isnot find!"
        }
        echo "nuc" | sudo -S chmod 777 /dev/bus/usb/001/*
        echo "nuc" | sudo -s chmod 777 /dev/bus/usb/002/*
        echo "nuc" | sudo -s chmod 777 /dev/bus/usb/003/*
        echo "nuc" | sudo -s chmod 777 /dev/bus/usb/004/*
	gnome-terminal -e 'bash -c "source /opt/ros/foxy/setup.bash;source /opt/intel/openvino_2022.3.0/setupvars.sh;source $(pwd)/../install/setup.bash;cd $(pwd)/..;ros2 launch rmos_bringup normal_aim.launch.py " '

fi

sleep 6

done

exit 0
