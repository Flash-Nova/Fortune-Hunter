from os import *
from random import *

MAP_WIDTH = 60
MAP_HEIGHT = 15
sensor_amount = 200
ring_left = 10
ring1 = []
ring2 = []
ring3 = []

def generate_rand_pos():
	global ring1, ring2, ring3
	ring1 = tuple([randint(0, 59), randint(0, 14)])
	ring2 = tuple([randint(0, 59), randint(0, 14)])
	while ring1 == ring2:
		ring2 = tuple([randint(0, 59), randint(0, 14)])
	ring3 = tuple([randint(0, 59), randint(0, 14)])
	while ring1 == ring3 or ring2 == ring3:
		ring3 = tuple([randint(0, 59), randint(0, 14)])

def create_map(row, column):
	temp = []
	for i in range(0, row):
		subtemp = []
		for j in range(0, column):
			subtemp.append(-1)
		temp.append(subtemp)
	return temp

def print_map(temp):
	row = len(temp)
	column = len(temp[0])

	#system("cls")
	print("", end = "\n")

	print(" " * 4, end = "")
	for i in range(10, column, 10):
		print(" " * 9 + str(i // 10), end = "")
	print("", end = "\n")

	print(" " * 3, end = "")
	for i in range(0, column, 1):
		print(i % 10, end = "")
	print("", end = "\n")

	for i in range(0, row):
		print(str(i).rjust(2), end = " ")
		for each in temp[i]:
			if each == -1:
				print("-", end = "")
			elif each == 10:
				print("X", end = "")
			else:
				print(each, end = "")

		print(" " + str(i), end = "\n")

	print(" " * 3, end = "")
	for i in range(0, column, 1):
		print(i % 10, end = "")
	print("", end = "\n")

	print(" " * 4, end = "")
	for i in range(10, column, 10):
		print(" " * 9 + str(i // 10), end = "")
	print("", end = "\n")

	#system("pause")

def dist(loc1, loc2):
	return abs(loc1[0] - loc2[0]) + abs(loc1[1] - loc2[1])

def min_dist(sensor, ring1, ring2, ring3):
	result = min(dist(sensor, ring1), dist(sensor, ring2), dist(sensor, ring3))
	if result >= 9:
		return 9
	return result

def imform():
	print("You have " + str(sensor_amount) + " sensors left and " + str(ring_left) + " gold rings to find.\nEnter an x (0-59) and y (0-14) for placing the sensor:", end = " ")

def input_recheck():
	global x, y
	while x > 59 or x < 0 or y > 14 or y < 0:
		system("cls")
		print_map(beach_map)
		print("Invalid row and/or column.")
		imform()
		x, y = [int(temp) for temp in input().split(" ")]

beach_map = create_map(MAP_HEIGHT, MAP_WIDTH)
print_map(beach_map)
generate_rand_pos()

while ring_left > 0 and sensor_amount > 0:
	imform()
	x, y = [int(temp) for temp in input().split(" ")]
	input_recheck()

	system("cls")

	sensor = tuple([x, y])
	sensor_amount -= 1
	length = min_dist(sensor, ring1, ring2, ring3)

	#print(x, y, sensor, length)

	if length == 0:
		beach_map[y][x] = 10
		print_map(beach_map)
		print("You found a gold ring!")
		ring_left -= 1
	else:
		beach_map[y][x] = length
		print_map(beach_map)

print("Running out of sensors . . .")
system("pause")