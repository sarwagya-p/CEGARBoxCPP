from run_benchmark import *

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 run_file.py <solver> <file_path>")
        exit()

    solver = sys.argv[1]
    file_path = sys.argv[2]

    cmd = solvers_cmd[solver](file_path)
    out = run(cmd, None, solvers_out_check[solver])