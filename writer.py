import sys
import argparse

GB = 1024*1024*1024
MB = 1024 * 1024 
filename = sys.argv[1]

filesize = int(float(sys.argv[2]) * GB)

# Create 4KB of data
data = bytearray.fromhex('abcd') * 2048

print(f'Writing file {filename} of size {filesize / (1024*1024*1024)}GB with data {data[0:2]}')
with open(filename, 'wb') as f:
    for _ in range(filesize // len(data)):
        f.write(data)

print("Done writing!")