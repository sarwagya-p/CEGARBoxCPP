import os
import sys
import subprocess
import pandas as pd
import numpy as np
from run_file import solvers, solvers_cmd, solvers_out_check, run, print_output

def compare_solve_count(file_dir, timeouts, out_dir, curr_solvers):
    file_list = os.listdir(path=path)
    file_list.sort()
    num_files = len(file_list)

    results = pd.DataFrame({solver: -np.ones(num_files) for solver in solvers}, dtype=int)
    results["File"] = file_list
    results.set_index("File", inplace=True)
    times = pd.DataFrame({solver: -np.ones(num_files) for solver in solvers}, dtype=float)
    times["File"] = file_list
    times.set_index("File", inplace=True)

    run_files(file_list, file_dir, timeouts, out_dir, results, times, curr_solvers)


def run_files(file_list, file_dir, timeout, out_dir, results, times, curr_solvers, write_df = True):
    for iter, file in enumerate(file_list):
        print(f"\nRunning {file}...")

        for solver in curr_solvers:
            print(f"Running {solver}...")
            
            if results.loc[file, solver] != -1:
                print("Solved before")
                continue

            cmd = solvers_cmd[solver](os.path.join(file_dir, file))
            out = run(cmd, timeout, solvers_out_check[solver])

            print_output(out)
            if out == "TIMEOUT" or out[0] == "ERROR":
                continue

            results.loc[file, solver] = 1 if out[0] == "SAT" else 0
            times.loc[file, solver] = out[1]

        if write_df and iter % 10 == 0:
            results.to_csv(f"{out_dir}/results.csv")
            times.to_csv(f"{out_dir}/times.csv")  

    return times, results 


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(len(sys.argv))
        print("Usage: python3 run_benchmark.py <benchmark_name> optional: <timeout> <solver1> <solver2> ... <solverN>")
        sys.exit(1)

    if len(sys.argv) > 2:
        timeouts = int(sys.argv[2])
    else:
        timeouts = 8
    
    if len(sys.argv) > 3:
        if sys.argv[3] == "all":
            curr_solvers = solvers
        else:
            curr_solvers = sys.argv[3:]
    else:
        curr_solvers = solvers

    name = sys.argv[1]

    path = f"../benchmarks/{name}/"
    out_dir = f"../results/{name}/"

    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    compare_solve_count(path, timeouts, out_dir, curr_solvers)