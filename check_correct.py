import sys

final_p = open('1.path','r') # read 'final.path' from working directory
floor = open(sys.argv[1],'r')

initial_val = floor.readline()
inval_list = initial_val.split(' ')

rows = int(inval_list[0])
cols = int(inval_list[1])
battery = int(inval_list[2])

flag = False
step_count = 0
mat = [[]]
# x = floor.read()
# print(type(x))
for r in range(rows):
    mat.append([])
    # print("")

for r in range(rows):
    for c in range(cols):
        mat[r].append(floor.read(1))
    floor.read(1)

#print matrix
# for r in range(rows):
#     for c in range(cols):
#         print(mat[r][c],end="")
#     # print(mat[r],end=)
#     print("")

t = int(final_p.readline())
start = (final_p.readline()).split(" ")
for i in range(t):
    temp = (final_p.readline()).split(" ")
    print(f'temp[0]={temp[0]},temp[1]={temp[1]}')
    mat[int(temp[0])][int(temp[1])] = 1
    if temp == start:
        step_count = 0
    else :
        step_count +=1
    print(f'battery={battery},step={step_count}')                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    if step_count > battery:
        print("Battery not enough!")
        flag = True
        break
    # else :
    #     print("gogo")

# for r in range(rows):
#     for c in range(cols):
#         print(mat[r][c],end="")
#     # print(mat[r],end=)
#     print("")

for r in range(rows):
    for c in range(cols):
        if mat[r][c] == '0':
            print("FAIL")
            flag = True
            break
if flag:
    print("Unexpected error")
else:
    print("SUCCESS")

final_p.close()
floor.close()
