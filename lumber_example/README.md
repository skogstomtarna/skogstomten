# lumber_example
Three nodes: lumberjack, lumbermill and lumbershop that show simple publisher/subscriber mechanics.
# running
Assuming you got ROS installed and this package is correctly placed in your ```catkin_ws```, run the following:
```bash
cd ~/catkin_ws
catkin_make
roslaunch lumber_example lumber_example.launch
```
1. Lumberjack will "cut" trees and publish logs on the ```lumber_to_mill``` topic.
2. Lumbermill will receive logs from that topic, cut them to planks and publish them on the ```planks_to_shop``` topic.
3. Lumbershop will receive planks from that topic and sell them or whatever.

You can look directly at any topic in the terminal with
```bash
rostopic echo topic_name
```
and you can list all available topics with
```bash
rostopic list
```
