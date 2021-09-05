# http://www.mikeburdis.com/wp/notes/plotting-serial-port-data-using-python-and-matplotlib/

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import random
import serial
import time

#initialize serial port
ser = serial.Serial()
ser.port = '/dev/ttyUSB0' #Arduino serial port
ser.baudrate = 115200
ser.timeout = 10000 #specify timeout when using readline()
ser.open()
if ser.is_open==True:
	print("\nAll right, serial port now open. Configuration:\n")
	print(ser, "\n") #print serial parameters
	
	# get control of the PSU
	ser.write("GetCtrl\n".encode())
	print("GetCtrl\n".encode())
	line=ser.readline() 
	ser.write("SetmA:450\n".encode())
	line=ser.readline() 
	ser.write("SetmV:00\n".encode())
	line=ser.readline() 
	time.sleep(1.5)
	

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = [] #store trials here (n)
ys = [] #store relative frequency here
rs = [] #for theoretical probability

mvSet = 100
# This function is called periodically from FuncAnimation
def animate(i, xs, ys):
    x = 0
    global mvSet

    if (mvSet < 5000) :
      #Aquire and parse data from serial port

      ser.write("GetmV\n".encode())
      line = ser.readline()     
      millivolt = int(line)
      
      ser.write("GetmA\n".encode())
      line = ser.readline()     
      milliamp = int(line)
      
      print(millivolt, milliamp, "\n")
      
      mvSet = mvSet + 250
      print(("SetmV:" + str(mvSet) + "\n").encode())
      ser.write(("SetmV:" + str(mvSet) + "\n").encode())
      line=ser.readline() 
      print(line)
      
      # Add x and y to lists
      xs.append(millivolt/1000)
      # ys.append(millivolt)
      rs.append(milliamp)

      # Draw x and y lists
      ax.clear()
      # ax.plot(xs, ys, label="mv")
      ax.plot(xs, rs, label="ma")

      # Format plot
      plt.xticks(rotation=45, ha='right')
      plt.subplots_adjust(bottom=0.30)
      plt.title('Sampled data')
      plt.legend()
      plt.axis([0.1, None, 0, 700]) #Use for arbitrary number of trials
      #plt.axis([1, 100, 0, 1.1]) #Use for 100 trial demo


ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=1500)
plt.show() 
