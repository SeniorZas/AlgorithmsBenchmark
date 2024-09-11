from random import shuffle

size = 10000
nums = list(range(1, size//2+1))
nums2 = list(range(size//2+1,size+1))
shuffle(nums2)
nums = nums+nums2

nombre = "datasetMitadOrdenado10000.txt"
file = open(nombre, "w")
for num in nums:
    file.write(str(num)+ " ")
file.close()