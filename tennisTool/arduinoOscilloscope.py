from collections import deque
from time import sleep
import matplotlib.pyplot as plt
import serial

ser = serial.Serial("/dev/tty.usbmodem11301",9600)

num_points = 100  # Number of coordinate points displayed on the screen simultaneously

x_values = deque(maxlen=num_points)  # Use a fixed-size deque to store x coordinates
y_values = deque(maxlen=num_points)  # Use a fixed-size deque to store y coordinates

plt.ion()  # Turn on interactive mode

x = None
y = None

while True:
    getval = ser.readline()
    getval_str = getval.decode()  # Convert bytes object to string
    getval_clean = getval_str.strip()  # Remove leading and trailing newline characters and spaces from the string

    # Check if the data contains 'x' or 'y'
    if getval_clean and 'x' == getval_clean[0]:
        x = float(getval_clean[1:])  # Update x coordinate
    elif 'y' == getval_clean[0]:
        y = float(getval_clean[1:])  # Update y coordinate
    else:
        print(getval_clean)

    # If both x and y have values, add them to the lists and plot the graph
    if x is not None and y is not None:
        x_values.append(x)  # Add the new x coordinate to the x_values list
        y_values.append(y)  # Add the new y coordinate to the y_values list

        print(x, y)

        # Plot the graph
        plt.clf()  # Clear the old graph
        plt.plot(x_values, y_values)
        plt.xlim(0, 255)  # Set the range of the x-axis
        plt.ylim(0, 255)  # Set the range of the y-axis
        plt.pause(0.00000000000000000000001)  # Pause for a moment to update the graph
