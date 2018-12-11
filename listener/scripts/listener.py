import rospy # Include this in the package dependency as well
from sensor_msgs.msg import LaserScan #Used to obtain the /scan data
from std_msgs.msg import Int32 #Used for publishing 1/0 to the decision node

def callback(msg):
	#for i in range(360):
	#	if (msg.ranges[i]<0.3):
	#		print(i-180)	
	#datapoints = [0..359]
	direction_identifier = 0  #Variable used to keep track of the binary encoding of object precsence in the 4 zones (Front right, front left, back right and back left
	entry1 = 0 #Variables to limit entry into cases for accuracy of the method
	entry2 = 0 #Variables to limit entry into cases for accuracy of the method
	entry3 = 0 #Variables to limit entry into cases for accuracy of the method
	entry4 = 0 #Variables to limit entry into cases for accuracy of the method
	faulty_proceed = 0 #Ensures avoiding faulty publish of "PROCEED FORWARD" to the decision node when there is even single point obstacle in front
	for i in range(360): #Skimming through all datapoints
		#direction_identifier = 0		
		if(i>=11 and i<25) or (i>324 and i<=344): # The two range beside the front facing beams
			if (msg.ranges[i]<1.1): ## Correspond to 0.5m from front of test rig, checking for obstacles in range
				lidar_pub.publish(1) #Publish 1 to decision node if obstacle found
				print("Stop Alert! Obstacle in front" )
				#print(i-180)
				break		
		if (i<11 or i>344):
			if (msg.ranges[i]<2.7): #Corresponds to 2m from front of test rig
				lidar_pub.publish(1) #Publish 1 to decision node if object is right in front
				print("Stop Alert! Obstacle in front at angle" ) 
				#print(i-180)
				break
			else:
				faulty_proceed = faulty_proceed+1  #Check to make sure faulty 0 is not published to decision node
				if (faulty_proceed == 26): ## Based on angle in front for STOP
					lidar_pub.publish(0)
		if (msg.ranges[i]<2.7): # Looking for obstacles everywhere around the vehicle for notification
			if(i<90): #360 degrees divided into 4 quadrants
							
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
	switcher = {             #Based on the decision identifiers values decide where the obstacles are
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
		


rospy.init_node('listener') # Initialising the ROS node 
sub=rospy.Subscriber('/scan', LaserScan, callback) # Subscribing to the scan topic published 
lidar_pub=rospy.Publisher('lidar_front_warning',Int32, queue_size = 1)  #Publishing data to decision node
# spin() simply keeps python from exiting until this node is stopped
rospy.spin()
