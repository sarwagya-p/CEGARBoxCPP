import os
import subprocess
from subprocess import TimeoutExpired
import time


def run_CEGAR(filename, timeout):
    cmd = ["../main", "-f", filename, "-tb4"]
    try:
        process = subprocess.run(cmd, 
                                stdout = subprocess.PIPE,
                                stderr = subprocess.PIPE,
                                timeout=timeout)
        
        if process.returncode == 0:
            print(f"Finished running file {filename} on CEGAR")

            if process.stdout.decode("utf-8") == "Satisfiable\n":
                return True
            else:
                raise Exception(f"CEGAR returned unsatisfiable on file {filename}")
            
        else:
            print(f"CEGAR returned error code {process.returncode}")
            return False
        
    except TimeoutExpired:
        print("Timeout")
        return False
    
def run_Cheetah(filename, timeout):
    cmd = ["./S5Cheetah", filename]

    try:
        process = subprocess.run(cmd, 
                                stdout = subprocess.PIPE,
                                stderr = subprocess.PIPE,
                                timeout=timeout)
        
        if process.returncode == 0:
            print(f"Finished running file {filename} on Cheetah")

            output = process.stdout.decode("utf-8").split("\n")

            if output[-3] == "s SATISFIABLE":
                return True
            else:
                print(output)
                raise Exception(f"Cheetah returned unsatisfiable on file {filename}")
            
        else:
            print(f"Cheetah returned error code {process.returncode}")
            return False
        
    except TimeoutExpired:
        print("Timeout")
        return False

if __name__ == "__main__":
    path = "./QS5/"
    cegar_path = "./QS5_CEGARBox/"
    dir_list = os.listdir(path=path)

    dir_list.sort()
    CEGAR_solved = 0
    Cheetah_solved = 0

    for file in dir_list:
        if run_CEGAR(cegar_path + file, 2):
            CEGAR_solved += 1

        if run_Cheetah(path + file, 2):
            Cheetah_solved += 1

    print(f"CEGAR solved {CEGAR_solved}, Cheetah solved {Cheetah_solved} out of {len(dir_list)}")