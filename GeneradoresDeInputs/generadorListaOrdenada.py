size = 100000
nums = list(range(1, size+1))

nombre = "datasetOrdenado100000.txt"
file = open(nombre, "w")
for num in nums:
    file.write(str(num)+ " ")
file.close()