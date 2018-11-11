#!/usr/bin/env python

import rospy
from std_msgs.msg import Int8
from pynput import keyboard


class KeyboardNode:
    def __init__(self):
        # action msg. 0=idle, 1=forward, 2=left, 3=right, 4=backward
        self.action = 0
        # publisher obj
        self.pub = rospy.Publisher('motor_action', Int8, queue_size=1)
        rospy.init_node('keyboard_node', anonymous=True)
        rate = rospy.Rate(10) # 10hz

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
        action_msg = Int8()
        action_msg.data = self.action
        self.pub.publish(action_msg)

if __name__ == '__main__':
    try:
        kn = KeyboardNode()
    except rospy.ROSInterruptException:
        pass
