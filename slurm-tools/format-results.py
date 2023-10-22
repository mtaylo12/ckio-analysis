import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('outdir', type=str)
args = parser.parse_args()


outdir = args.outdir

def extract_row(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        average_total = lines[-6].split()[-1]
        max_total = lines[-5].split()[-1]
        average_fread = lines[-3].split()[-1]
        max_fread = lines[-2].split()[-1]

        return [average_total, max_total, average_fread, max_fread]


all_data = [["avg(total)", "max(total)", "avg(fread)", "max(fread)"]]
for node in [1, 2, 4, 8, 16, 32, 48, 64, 72, 80, 88, 96, 112]:
    filename = "output/" + outdir + "/" + str(node) + ".txt"
    if os.path.exists(filename):
        row = extract_row(filename)
        row.insert(0, node)
        all_data.append(row)

for row in all_data:
    print(' '.join(str(i) for i in row))
