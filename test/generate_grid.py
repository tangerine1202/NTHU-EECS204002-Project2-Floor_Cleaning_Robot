import math

# **odd** number only for grid test case
m = 99
n = 99
b = (m+n-2)*2
input_file_name = f'{m}x{n}_grid'

with open(input_file_name, 'w') as f:
    f.write(f'{m} {n} {b}')
    f.write('\n')
    f.write('R' + '01'*(math.floor((n-1)/2)))
    f.write('\n')
    for i in range(math.floor((m-1)/2)):
        f.write('0'*n)
        f.write('\n')
        f.write('1' + '01'*(math.floor((n-1)/2)))
        f.write('\n')
