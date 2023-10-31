import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('outdir', type=str)
args = parser.parse_args()


outdir = args.outdir

def extract_result(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        min_fread = lines[-2].split()[-1]
        average_fread = lines[-2].split()[-5]
        max_fread = lines[-2].split()[-3]

        return[float(average_fread), float(max_fread)]


print("results in range (1,100) averageing over 10 sols")
for tasks in range(1, 100):
    acc_average = 0
    acc_max = 0
    numresults = 0

    for i in range(1, 11):
        filename = str(outdir) + str(tasks) + "task-run" + str(i) + ".txt"
        if os.path.exists(filename):
            [average, maximum] = extract_result(filename)
            acc_average += average
            acc_max += maximum
            numresults += 1

    if (numresults != 0):
        acc_average = acc_average / numresults
        acc_max = acc_max / numresults

        print(tasks, acc_average, acc_max)
