import os
import subprocess
import time

path = "./S5Experiments/QS5_CEGARBox/"
dir_list = os.listdir(path=path)

dir_list.sort()

total_time_s = 0
num_solved = 0

for file in dir_list:
    print(f"Running file: {file}")
    pipe = subprocess.Popen(f"time ./main -f {path}{file} -tb4", shell=True, executable='/bin/bash', stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    time_output = pipe.communicate()[1].decode("utf-8")
    print(time_output)
    wall_time = time_output.split()[1].split("m")
    s = wall_time[1][:-2]
    print(f"Time taken: {s}s")
    total_time_s += float(s)
    num_solved +=1

    print(f"Num solved: {num_solved}; Total time: {total_time_s}")

    if (total_time_s > 10):
        break

print(total_time_s)
print(num_solved)