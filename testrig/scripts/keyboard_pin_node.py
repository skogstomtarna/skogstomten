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
        print("Set motor pins directly.")
        print("Format: EN1 DIR1 EN2 DIR2 EN3 DIR3 EN4 DIR4")
        command = ''
        while 1:
            command = raw_input('# ')
            if command == 'quit' or command == 'exit' or command == 'q':
                print("Exiting. Thank you for your service.")
                break
            input_error = 0
            command = command.replace(' ','')

            com_len = len(command)
            if com_len!=8:
                input_error = 1

            digs = [0]*8
            for i in range(com_len):
                if command[i]!='0' and command[i]!='1':
                    input_error = 2
                    break
                else:
                    digs[i] = int(command[i])
            if input_error==0:
                # if no errors, unpack. Format: enables 1-4, dirs 1-4
                self.pins = digs[0]*128 + digs[2]*64 + digs[4]*32 + digs[6]*16 + digs[1]*8 + digs[3]*4 + digs[5]*2 + digs[7]
                print('Success. Sending value: {0}\n'.format(self.pins))
                self.publish()
            elif input_error==1:
                print('Wrong argument length.\n')
            elif input_error==2:
                print('Only 1s and 0s allowed.\n')
            else:
                print("Unknown error.\n")
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
