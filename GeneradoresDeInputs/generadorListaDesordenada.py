from random import shuffle

size = 10000
nums = list(range(1, size+1))
shuffle(nums)

nombre = "datasetDesordenado10000.txt"
file = open(nombre, "w")
for num in nums:
    file.write(str(num)+ " ")
file.close()