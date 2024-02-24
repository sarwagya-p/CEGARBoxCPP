import os
import sys
import subprocess
import pandas as pd
import numpy as np
from run_file import solvers, solvers_cmd, solvers_out_check, run

def compare_solve_count(file_dir, timeouts, out_dir, correct_ans = "UNKNOWN"):
    file_list = os.listdir(path=path)
    file_list.sort()
    num_files = len(file_list)

    results = pd.DataFrame({solver: -np.ones(num_files) for solver in solvers}, dtype=int)
    results["File"] = file_list
    results.set_index("File", inplace=True)
    times = pd.DataFrame({solver: -np.ones(num_files) for solver in solvers}, dtype=float)
    times["File"] = file_list
    times.set_index("File", inplace=True)

    run_files(file_list, file_dir, timeouts, out_dir, results, times, correct_ans)


def run_files(file_list, file_dir, timeouts, out_dir, results, times, correct_ans = "UNKNOWN", write_df = True):
    for timeout in timeouts:
        for file in file_list:
            print(f"\nRunning {file}...")

            for solver in solvers:
                print(f"Running {solver}...", end=" ")
                
                if results.loc[file, solver] != -1:
                    print("Solved before")
                    continue

                cmd = solvers_cmd[solver](os.path.join(file_dir, file))
                out = run(cmd, timeout, solvers_out_check[solver])

                if out == "TIMEOUT" or out == "ERROR":
                    continue

                if correct_ans != "UNKNOWN" and out != correct_ans:
                    print(f"ERROR: {solver} gave wrong answer")
                    continue
                    # exit(1)

                results.loc[file, solver] = 1 if out == "SAT" else 0
                times.loc[file, solver] = timeout
        
        print("Statistics for timeout: ", timeout)
        for solver in solvers:
            print(f"{solver}: Solved {(results[solver] != -1).sum()} out of {len(file_list)} instances")

        if write_df:
            results.to_csv(f"{out_dir}/results.csv")
            times.to_csv(f"{out_dir}/times.csv")  

    return times, results 


if __name__ == "__main__":
    timeouts = [0.25, 0.5, 1, 2, 4, 8]

    name = sys.argv[1]

    path = f"../benchmarks/{name}/"
    out_dir = f"../results/{name}/"

    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    compare_solve_count(path, timeouts, out_dir, correct_ans=sys.argv[2])