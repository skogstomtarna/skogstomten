import rospy
from sensor_msgs.msg import LaserScan

def callback(msg):
	#for i in range(360):
	#	if (msg.ranges[i]<0.3):
	#		print(i-180)	
	#datapoints = [0..359]
	direction_identifier = 0
	entry1 = 0
	entry2 = 0
	entry3 = 0
	entry4 = 0
	for i in range(360):
		#direction_identifier = 0		
		if (i<11 or i>344):
			if (msg.ranges[i]<2.1):
				print("Stop Alert! Obstacle in front at angle" )
				#print(i-180)
				break
		if (msg.ranges[i]<1):
			if(i<90):
							
				#print("Notification. RIGHT FRONT")
				if (entry1 == 0):
					direction_identifier = 1
					entry1 = 1
			elif(i<180):
				#print("Notification. RIGHT BACK")
				#break
				if (entry2 == 0):	
					direction_identifier = direction_identifier+2
					entry2 = 1
			elif(i<270):
				#print("Notification. LEFT BACK")
				#break
				if (entry3 == 0):
					direction_identifier = direction_identifier+4
					entry3 = 1
			else:
				#print("Notification. LEFT FRONT")
				#break
				if (entry4 == 0):
					direction_identifier =  direction_identifier+8
					entry4 = 1
	#print(direction_identifier)	
	switcher = {
		0: "DONT MOVE",
		1: "LEFT FRONT",
		2: "LEFT BACK",
		3: "LEFT(Front and back)",
		4: "RIGHT BACK",
		5: "LEFT FRONT + RIGHT BACK",
		6: "BACK (Both sides)",
		7: "LEFT FRONT and (Both sides)BACK ",
		8: "RIGHT FRONT",
		9: "FRONT(Left and Right)",
		10: "RIGHT FRONT + LEFT BACK",
		11: "RIGHT FRONT + LEFT (Front and back)",
		12: "RIGHT (Front and Back)",
		13: "LEFT FRONT + RIGHT (Front and Back)",
		14: "LEFT BACK + RIGHT (Front and Back)",
		15: "ALL AROUND!"
		}
    	print switcher.get(direction_identifier, "All clear")
		#	else:
		#		print("Proceed forward!")
		#else:
		#	if (msg.ranges[i]<1):
		#		print("Surrounding object Alert! At")
		#		print(i-180)
		#	else: 
		#		print("Safe around")
		


rospy.init_node('listener')
sub=rospy.Subscriber('/scan', LaserScan, callback)
# spin() simply keeps python from exiting until this node is stopped
rospy.spin()
