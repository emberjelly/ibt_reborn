import serial
import time

baudrate = 9600
device = "/dev/ttyUSB0"

ser = serial.Serial(device, baudrate)


def write_msg(ser, message_str):
	ser.write(message_str)
	ser.flush()

def read_msg(ser):
	if ser.inWaiting():
		return ser.readline()



for i in range(20):

	print "test"
	
	for i in range(4):
		write_msg(ser, str(i + 1).encode())
		time.sleep(0.5/4)
		for j in range(3):
			write_msg(ser, 's'.encode())
			time.sleep(0.5/4)





ser.close()