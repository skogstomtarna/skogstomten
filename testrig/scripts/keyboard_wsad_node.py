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

        self.x = 0 # left = -1, right = +1
        self.y = 0 # backward = +1, forward = -1

        # actions
        self.action = 5 # 5 = idle

        # publisher obj
        self.pub = rospy.Publisher('motor_action', Int64, queue_size=1)

        # loop rate
        rate = rospy.Rate(10) # 10hz

        # kb listener setup
        with keyboard.Listener(on_press=lambda key: self.on_press(key), on_release=lambda key: self.on_release(key)) as listener:
            listener.join()

    def on_press(self, key):
        try:
            print('')

            if key.char == 'q' or key == keyboard.Key.esc:
                exit()
            elif key.char == 'w':
                self.y = 1
                print('y = {0}'.format(self.y))
            elif key.char == 's':
                self.y = -1
                print('y = {0}'.format(self.y))
            elif key.char == 'a':
                self.x = -1
                print('x = {0}'.format(self.x))
            elif key.char == 'd':
                self.x = 1
                print('x = {0}'.format(self.x))

            self.action_recalc()
            self.publish()

        except AttributeError:
            pass

    def on_release(self,key):
        try:
            print('')

            if key.char == 'w' or key.char == 's':
                self.y = 0
            elif key.char == 'a' or key.char == 'd':
                self.x = 0

            self.action_recalc()
            self.publish()
        except AttributeError:
            pass

    def action_recalc(self):
        # actions - imagine a keypad:
        # 7 8 9     FL F FR
        # 4 5 6  =  L  I  R
        # 1 2 3     BL B BR
        # F = forward, B = backward, L = left, R = right, I = idle
        self.action = self.x + 3*self.y + 4
        print('action: {0}'.format(self.action))
    def publish(self):
        # action_msg is of type Int64
        action_msg = Int64()
        # pack action
        action_msg.data = self.action
        # pub
        self.pub.publish(action_msg)

if __name__ == '__main__':
    try:
        kn = KeyboardWsadNode()
    except rospy.ROSInterruptException:
        pass
