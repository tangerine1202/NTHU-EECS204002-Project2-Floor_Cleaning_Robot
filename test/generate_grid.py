import math

m = 100
n = 100
b = (m+n-2)*2
input_file_name = f'{m}x{n}_grid.txt'

with open(input_file_name, 'w') as f:
    f.write(f'{m} {n} {b}')
    f.write('\n')
    f.write('R' + '0'*(n-1))
    f.write('\n')
    for i in range(math.floor((m-1)/2)):
        f.write('0' + '10'*(math.floor((n-1)/2)))
        f.write('\n')
        f.write('0'*n)
        f.write('\n')
