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
	time.sleep(0.5)
	write_msg(ser, '1')
	print "test"


ser.close()