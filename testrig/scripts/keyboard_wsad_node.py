#!/usr/bin/env python

## keyboard node to control the robot with WSAD
## publishes an action ID that is then taken by motor_controller_node
## printed feedback is a bit weird
import rospy
from std_msgs.msg import Int64
from pynput import keyboard

class KeyboardWsadNode:
    def __init__(self):
        # node init
        rospy.init_node('keyboard_wsad_node', anonymous=True)

        # action msg. 0=idle, 1=forward, 2=left, 3=right, 4=backward
        self.action = 0

        # publisher obj
        self.pub = rospy.Publisher('motor_action', Int64, queue_size=1)

        # loop rate
        rate = rospy.Rate(10) # 10hz

        # kb listener
        with keyboard.Listener(on_press=lambda key: self.on_press(key), on_release=lambda key: self.on_release(key)) as listener:
            listener.join()

    def on_press(self, key):
        try:
            print('alphanumeric key {0} pressed'.format(key.char))
            if key.char == 'w':
                self.action = 1 # forward
            elif key.char == 'a':
                self.action = 2 # left
            elif key.char == 'd':
                self.action = 3 # right
            elif key.char == 's':
                self.action = 4 # back
            elif key.char == 'q' or key == keyboard.Key.esc:
                exit()

            self.publish()
        except AttributeError:
            pass

    def on_release(self,key):
        try:
            # maybe just merge all these? meh
            print('{0} released'.format(key))
            if key.char == 'w':
                self.action = 0
            elif key.char == 'a':
                self.action = 0
            elif key.char == 'd':
                self.action = 0
            elif key.char == 's':
                self.action = 0

            self.publish()
        except AttributeError:
            pass

    def publish(self):
        action_msg = Int64()
        action_msg.data = self.action
        self.pub.publish(action_msg)

if __name__ == '__main__':
    try:
        kn = KeyboardWsadNode()
    except rospy.ROSInterruptException:
        pass
