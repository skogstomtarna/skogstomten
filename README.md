# skogstomten
ROS platform for the XT28-forwarder test rig

## Setup
Check out this link: http://wiki.ros.org/kinetic/Installation
I say we run ROS Kinetic, it's the most stable and I already have it installed. :^) There are two newer distributions, Lunar and Melodic, and to be honest I'm not sure what even is the difference. But hey, kinetic is stable and recommended.
If you're running a more exotic Linux distro (like me with Mint Sonya), you might not be getting Kinetic by default. I had to update my ```/etc/apt/sources.list.d/ros-latest.list``` and change ```sonya main``` to ```xenial main``` at the end, then update my ```apt```. It worked, and nothing has exploded. Yet.

Assuming ROS Kinetic installed for you in a standard way and you are running some flavor of Linux:
```bash
cd ~/catkin_ws/src
git clone https://github.com/skogtomtarna/skogtomten.git
catkin_make
```
iApple(tm)(c)(r) and Windows: I have no idea.

## Getting started with ROS
A good deal of info can be found here: https://anywiki.csc.kth.se/DD2425/doku.php?id=start (ignore the dead certificate warning, you can skip it with advanced options)
You should be able to find a robot lab there to get some bearings on how ROS works. But in an extreme short:
1. ROS is a platform to easily communicate between controllers ("nodes").
2. You have a main computer running the ```roscore``` and then some nodes that connect to each other using topics.
3. A topic is pretty much a message queue, and the message type can be more or less anything - from a laughably simple Bool to complex arrays.
4. A node can publish to topics (sending out a message) and subscribe to topics (reading a message). Usually subscribing to and publishing to the same topic is discouraged. However many nodes can be subscribing to or publishing to the same topic, and a node can be subscribed or publishing to however many topics. Physical constainsts like the physical size of the universe notwithstanding.
5. Code is split into packages, each package needs to have ```CMakeLists.txt``` and ```package.xml``` in them. Then you can add ```/src```, ```/include```, ```/scripts``` and whatnot. ```skogtomten``` package is a metapackage - pretty much a "everything here is part of skogstomten metapackage", it has no executable code of its own. ```lumber_example``` is a simple example to show how subscribers and publishers work. I threw it together quickly and cannot even try to compile at the moment, there might be bugs.
6. Code can be written in Python and C++ (and regular old C as well, of course). Use whichever you like most. Please comment your code - better to write too much than too little.


## Branching
I am not a git expert, and I don't expect you to be either. But I think branching is the way to go here, don't commit to master willy-nilly. :-P
