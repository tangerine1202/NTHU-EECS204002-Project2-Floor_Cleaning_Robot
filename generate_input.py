m = 1000
n = 1000
b = (m+n-2)*2
input_file_name = f'{m}x{n}.txt'

with open(input_file_name, 'w') as f:
    f.write(f'{m} {n} {b}')
    f.write('\n')
    f.write('R' + '0'*(n-1))
    f.write('\n')
    for i in range(m-1):
        f.write('0'*n)
        f.write('\n')
