import os
import subprocess
from subprocess import TimeoutExpired
import time

def checkOutput_CEGAR(process):
    if process.decode("utf-8") == "Satisfiable\n":
        return True
    else:
        return False
    
def checkOutput_Cheetah(process):
    if process.decode("utf-8").split("\n")[-3] == "s SATISFIABLE":
        return True
    else:
        return False

def run(cmd, filename, timeout, checkOutput):
    try:
        process = subprocess.check_output(cmd + [filename], 
                                timeout=timeout)

        if checkOutput(process):
            print("SUCCESS")
            return True
        
        else:
            print("WRONG ANSWER")
            return False

    except subprocess.CalledProcessError as e:
        print(e.output)
        print(f"ERROR in execution")

        with open("../troublefiles.txt", "a") as file:
            file.write(filename + "\n")
        
    except TimeoutExpired:
        print(f"TIMEOUT")
        return False

def prepare_file(filename):
    subprocess.call(f"sed '1d' {filename} | sed '2d' | sed 's/r//g' | sed 's/-/=/g' > cegar_file.tmp", shell=True)
    return "cegar_file.tmp"

def compare_solve_count(dir, cegar_dir_list, cheetah_dir_list, timeout):
    solved_count = [0, 0]

    CEGAR_cmd = ["../main", "-tb4", "-f"]
    Cheetah_cmd = ["./S5Cheetah"]

    cegar_list_copy = cegar_dir_list.copy()
    cheetah_list_copy = cheetah_dir_list.copy()

    for file in cegar_list_copy:
        print(f"\nRunning {file}...")

        print("CEGAR:", end=" ")
        if run(CEGAR_cmd, prepare_file(dir + file), timeout, checkOutput_CEGAR):
            solved_count[0] += 1
            cegar_dir_list.remove(file)

    for file in cheetah_list_copy:
        print(f"\nRunning {file}...")

        print("Cheetah:", end=" ")
        if run(Cheetah_cmd, dir+file, timeout, checkOutput_Cheetah):
            solved_count[1] += 1
            cheetah_dir_list.remove(file)

    print(f"CEGAR solved {solved_count[0]}, Cheetah solved {solved_count[1]} for timout {timeout}")
    return solved_count


def run_benchmark(timeouts, path, filename):
    counts = []
    cegar_solved = 0
    cheetah_solved = 0

    cegar_dir_list = os.listdir(path=path)
    cegar_dir_list.sort()

    cheetah_dir_list = cegar_dir_list.copy()

    total = len(cegar_dir_list)

    for timeout in timeouts:
        count = compare_solve_count(path, cegar_dir_list, cheetah_dir_list, timeout)
        cegar_solved += count[0]
        cheetah_solved += count[1]

        counts.append([cegar_solved, cheetah_solved])

        with open(filename, "a+") as file:
            file.write(f"{timeout}: {counts} / {total}\n")
    return counts

def plot(timeouts, counts, total, name):
    import matplotlib.pyplot as plt
    import matplotlib.ticker

    fig, ax = plt.subplots()
    ax.plot(timeouts, [x[0] for x in counts], label="CEGAR", c = "#B931FC")
    ax.plot(timeouts, [x[1] for x in counts], label="Cheetah", c = "#5CD2E6")

    ax.plot(timeouts, [total for x in timeouts], label="Total", c = "#FF0000", linestyle="--")
    ax.set_xlabel("Given Time (in s, log scale)")

    ax.set_xscale("log", base=2)
    ax.set_xticks(timeouts)
    ax.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())

    ax.set_ylabel("Number of instances solved")
    ax.legend()
    ax.grid(linestyle="--", color="grey", linewidth=0.5)

    plt.savefig(f"plots/{name}.png")
    plt.show()

import numpy as np

if __name__ == "__main__":
    timeouts = [0.25, 0.5, 1, 2, 4]

    name = "QS5"

    path = f"./data/{name}/"
    filename = f"result/{name}.txt"

    # counts = run_benchmark(timeouts, path, filename)
    counts = np.array([[706, 165], [780, 376], [879, 541], [931, 654], [944, 749]])
    counts += np.array([[155, 105], [191, 123], [230, 145], [240, 167], [248, 200]])
    counts += np.array([[0, 0], [71, 0], [113, 0], [169, 7], [225, 55]])

    # counts = [[0, 0], [71, 0], [113, 0], [169, 7], [225, 55], [234, 66]]

    plot(timeouts, counts, 945+len(os.listdir(path))+240, "mix")