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
    subprocess.call(f"sed '1d' {filename} | sed '2d' | sed 's/r//g' > cegar_file.tmp", shell=True)
    return "cegar_file.tmp"

def compare_solve_count(dir, timeout):
    dir_list = os.listdir(path=dir)
    dir_list.sort()

    solved_count = [0,0]

    CEGAR_cmd = ["../main", "-tb4", "-f"]
    Cheetah_cmd = ["./S5Cheetah"]

    for file in dir_list:
        print(f"\nRunning {file}...")

        print("CEGAR:", end=" ")
        if run(CEGAR_cmd, prepare_file(dir + file), timeout, checkOutput_CEGAR):
            solved_count[0] += 1

        print("Cheetah:", end=" ")
        if run(Cheetah_cmd, dir + file, timeout, checkOutput_Cheetah):
            solved_count[1] += 1

    print(f"CEGAR solved {solved_count[0]}, Cheetah solved {solved_count[1]} out of {len(dir_list)} for timout {timeout}")
    return solved_count


def run_benchmark(timeouts, path):
    counts = []

    for timeout in timeouts:
        counts.append(compare_solve_count(path, timeout))

    return counts

def plot(timeouts, counts, total):
    import matplotlib.pyplot as plt
    import matplotlib.ticker

    fig, ax = plt.subplots()
    ax.plot(timeouts, [x[0] for x in counts], label="CEGAR", c = "#B931FC")
    ax.plot(timeouts, [x[1] for x in counts], label="Cheetah", c = "#5CD2E6")

    ax.plot(timeouts, [total for x in timeouts], label="Total", c = "#FF0000", linestyle="--")
    ax.set_xlabel("Given Time (in s, log scale)")

    ax.set_xscale("log", base=2)
    ax.set_xticks([0.25, 0.5, 1, 2, 4])
    ax.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())

    ax.set_ylabel("Number of instances solved")
    ax.legend()
    ax.grid(linestyle="--", color="grey", linewidth=0.5)

    plt.savefig("plots/QS5.png")
    plt.show()

if __name__ == "__main__":
    timeouts = [0.25, 0.5, 1, 2, 4, 8]

    path = "./data/3CNF_mutate-sat/"

    counts = run_benchmark(timeouts, path)

    with open("result/3CNF_mutate.txt", "w") as file:
        file.write(str(counts))

    # counts = eval(open("data/3CNF_mutate.txt", "r").read())

    plot(timeouts, counts, len(os.listdir(path=path)))