#!/usr/bin/env python

## another keyboard node
## send pin settings directly to controller
## pretty much just a simplified publisher
import rospy
from std_msgs.msg import Int64
from pynput import keyboard

class KeyboardPinNode:
    def __init__(self):
        # node init
        rospy.init_node('keyboard_pin_node', anonymous=True)

        # action msg. 0=idle, 1=forward, 2=left, 3=right, 4=backward
        self.pins = 0

        # publisher obj
        self.pub = rospy.Publisher('pins', Int64, queue_size=1)

        # loop rate
        rate = rospy.Rate(10) # 10hz

    def run(self):
        command = ''
        while command != 'quit' and command != 'exit':
            command = raw_input('# set motors: motor1 enable dir, motor 2 enable dir etc \nexample: 10110011')
            digits = command.split('')

            input_error = 0
            if len(digits)!=8:
                print('Wrong argument length.')
                input_error = 1
            for i in range 8:
                if digits[i]!='0' and digits[i]!='1'
                    print('Wrong argument composition. Only 0s and 1s are allowed.')
                    input_error = 1
            if !input_error:
                # if no errors, unpack
                self.pins = digits[0]*128 + digits[2]*64 + digits[4]*32 + digits[6]*16 + digits[1]*8 + digits[3]*4 + digits[5]*2 + digits[7]
                self.publish()

    def publish(self):
        pins_msg = Int64()
        pins_msg.data = self.pins
        self.pub.publish(pins_msg)


if __name__ == '__main__':
    try:
        kb = KeyboardPinNode()
        kb.run()
    except rospy.ROSInterruptException:
        pass
