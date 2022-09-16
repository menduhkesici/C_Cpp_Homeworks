# python program to create a random input

import random

arr_time = 0

# change the range to change the number of input items
for x in range(1,3000):
	priority = random.randint(1,3)
	arr_time += random.randint(2,20) * 10
	exec_time = random.randint(4,20) * 10
	print(x, priority, arr_time, exec_time)
