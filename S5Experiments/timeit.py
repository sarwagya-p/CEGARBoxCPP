import os
import sys
import subprocess
import pandas as pd
import numpy as np

def cegar_cmd(filename):
    subprocess.call(
        f"sed '1d' {filename} | sed '2d' | sed 's/r//g' | sed 's/-/=/g' | sed 's/false/$false/g' | sed 's/true/$true/g' > cegar_file.tmp", 
        shell=True
        )
    
    return ["../main", "-tb4", "-f", "cegar_file.tmp"]

def cheetah_cmd(filename):
    return ["./S5Cheetah", filename]

def s52sat_cmd(filename):
    return ["./S52SAT", filename, "-diamondDegree", "-caching"]


def check_out_CEGAR(process):
    if process.decode("utf-8") == "Satisfiable\n":
        return "SAT"
    else:
        return "UNSAT"
    
def check_out_Cheetah(process):
    if process.decode("utf-8").split("\n")[-3] == "s SATISFIABLE":
        return "SAT"
    else:
        return "UNSAT"
    
def check_out_S52SAT(process):
    if process.decode("utf-8").split("\n")[-8] == "s SATISFIABLE":
        return "SAT"
    else:
        return "UNSAT"
    
solvers = ["CEGAR", "Cheetah", "S52SAT"]

solvers_cmd = {
    "CEGAR": cegar_cmd,
    "Cheetah": cheetah_cmd,
    "S52SAT": s52sat_cmd
}
    
solvers_out_check = {
    "CEGAR": check_out_CEGAR,
    "Cheetah": check_out_Cheetah,
    "S52SAT": check_out_S52SAT
}

def run(cmd, timeout, checkOutput):
    try:
        process = subprocess.check_output(cmd, timeout=timeout)

        print("OK")
        return checkOutput(process)

    except subprocess.CalledProcessError as e:
        print(e.output)
        print(f"ERROR in execution")
        exit(1)
        # return "ERROR"
        
    except subprocess.TimeoutExpired:
        print(f"TIMEOUT")
        return "TIMEOUT"

def compare_solve_count(dir, timeouts, out_dir, correct_ans = "UNKNOWN"):
    file_list = os.listdir(path=path)
    file_list.sort()
    num_files = len(file_list)

    results = pd.DataFrame({solver: -np.ones(num_files) for solver in solvers}, dtype=int)
    results["File"] = file_list
    results.set_index("File", inplace=True)
    times = pd.DataFrame({solver: -np.ones(num_files) for solver in solvers}, dtype=float)
    times["File"] = file_list
    times.set_index("File", inplace=True)

    for timeout in timeouts:
        for file in file_list:
            print(f"\nRunning {file}...")

            for solver in solvers:
                print(f"Running {solver}...", end=" ")
                
                if results.loc[file, solver] != -1:
                    print("Solved before")
                    continue

                cmd = solvers_cmd[solver](f"{dir}{file}")
                out = run(cmd, timeout, solvers_out_check[solver])

                if out == "TIMEOUT":
                    continue

                if correct_ans != "UNKNOWN" and out != correct_ans:
                    print(f"ERROR: {solver} gave wrong answer")
                    exit(1)

                results.loc[file, solver] = 1 if out == "SAT" else 0
                times.loc[file, solver] = timeout
        
        print("Statistics for timeout: ", timeout)
        for solver in solvers:
            print(f"{solver}: Solved {(results[solver] != -1).sum()} out of {num_files} instances")

        results.to_csv(f"{out_dir}/results.csv")
        times.to_csv(f"{out_dir}/times.csv")   


if __name__ == "__main__":
    timeouts = [0.25, 0.5, 1, 2, 4]

    name = sys.argv[1]

    path = f"./benchmarks/{name}/"
    out_dir = f"./results/{name}/"

    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    compare_solve_count(path, timeouts, out_dir, correct_ans="SAT")