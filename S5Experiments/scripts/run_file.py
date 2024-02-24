import subprocess
import sys

def cegar_cmd(filename):
    with open(filename, "r") as f:
        inp = f.read()

    inp = inp.split("\n")[1]
    inp = inp.replace("r1", "").replace("-", "=").replace("false", "$false").replace("true", "$true")
    
    with open("cegar_file.tmp", "w") as f:
        f.write(inp)

    return ["../../main", "-tb4", "-f", "cegar_file.tmp"]

def cheetah_cmd(filename):
    with open(filename, "r") as f:
        inp = f.read()

    inp = inp.replace("p0", "p99999").replace("v", "p")

    with open("cheetah_file.tmp", "w") as f:
        f.write(inp)

    return ["./S5Cheetah", "cheetah_file.tmp"]

def s52sat_cmd(filename):
    with open(filename, "r") as f:
        inp = f.read()

    inp = inp.replace("p0", "p99999").replace("v", "p")

    with open("s52sat_file.tmp", "w") as f:
        f.write(inp)

    return ["./S52SAT", "s52sat_file.tmp", "-diamondDegree", "-caching"]

def lck_cmd(filename):
    with open(filename, "r") as f:
        inp = f.read()

    inp = "1:" + inp.split("\n")[1]
    inp = inp.replace("r1", "E").replace("-", "=").replace("false", "False").replace("true", "True")
    
    print(inp)
    return f'echo "{inp}"| ../lckS5Prover/lck graph '

def ksp_cmp(filename):
    with open(filename, "r") as f:
        file_input = f.read()

    file_input = file_input.split("\n")[1]
    file_input = file_input.replace("[r1]", " box ").replace("<r1>", " dia ")
    print(file_input)
    return ["../ksp-0.1.6/ksp", "-c", "../ksp-0.1.6/conf.files/cade-28/S5_euc1_euc2_ref.conf", "-c", "../ksp-0.1.6/conf.files/cade-28/cord_ple_ires_K.conf", "-f", file_input]

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
    
def check_out_LCK(process):
    if process.decode("utf-8").split("\n")[-3].split()[-1] == "satisfiable":
        return "SAT"
    else:
        return "UNSAT"
    
def check_out_KSP(process):
    if process.decode("utf-8").split("\n")[-2] == " Satisfiable.":
        return "SAT"
    elif process.decode("utf-8").split("\n")[-2] == " Unsatisfiable.":
        return "UNSAT"
    
    else:
        return "ERROR"
    
solvers = ["CEGAR", "Cheetah", "S52SAT", "LCK"]

solvers_cmd = {
    "CEGAR": cegar_cmd,
    "Cheetah": cheetah_cmd,
    "S52SAT": s52sat_cmd,
    "LCK": lck_cmd,
    "KSP": ksp_cmp
}
    
solvers_out_check = {
    "CEGAR": check_out_CEGAR,
    "Cheetah": check_out_Cheetah,
    "S52SAT": check_out_S52SAT,
    "LCK": check_out_LCK,
    "KSP": check_out_KSP
}

def run(cmd, timeout, checkOutput, use_shell = False):
    try:
        process = subprocess.check_output(cmd, timeout=timeout, shell=use_shell)

        print(f"OK: {checkOutput(process)}")
        return checkOutput(process)

    except subprocess.CalledProcessError as e:
        print(e.output)
        print(f"ERROR in execution")
        # exit(1)
        return "ERROR"
        
    except subprocess.TimeoutExpired:
        print(f"TIMEOUT")
        return "TIMEOUT"

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 run_file.py <solver> <file_path>")
        exit()

    solver = sys.argv[1]
    file_path = sys.argv[2]

    cmd = solvers_cmd[solver](file_path)
    out = run(cmd, None, solvers_out_check[solver], use_shell = (solver == "LCK"))