import os
import subprocess
from subprocess import TimeoutExpired
import time


def run_CEGAR(filename, timeout):
    cmd = ["../main", "-f", filename, "-tb4"]
    try:
        process = subprocess.check_output(cmd, 
                                timeout=timeout)
        
        print(f"Finished running file {filename} on CEGAR")

        if process.decode("utf-8") == "Satisfiable\n":
            return True
        else:
            # raise Exception(f"CEGAR returned unsatisfiable on file {filename}")
            print(f"CEGAR returned unsatisfiable on file {filename}")
        
    except subprocess.CalledProcessError as e:
        print(e.output)
        print(f"\n CEGAR return error on file {filename}")

        with open("../troublefiles.txt", "a") as file:
            file.write(filename + "\n")
        
    except TimeoutExpired:
        print(f"CEGAR timed out on file {filename}")
        return False
    
def run_Cheetah(filename, timeout):
    cmd = ["./S5Cheetah", filename]

    try:
        process = subprocess.check_output(cmd, 
                                timeout=timeout)
        
        print(f"Finished running file {filename} on Cheetah")

        output = process.decode("utf-8").split("\n")

        if output[-3] == "s SATISFIABLE":
            return True
        else:
            print(output)
            raise Exception(f"Cheetah returned unsatisfiable on file {filename}")
        
    except subprocess.CalledProcessError as e:
        print(e.output)
        print(f"\n Cheetah return error on file {filename}")
        
    except TimeoutExpired:
        print(f"Cheetah timed out on file {filename}")
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

        print("\n")

    print(f"CEGAR solved {CEGAR_solved}, Cheetah solved {Cheetah_solved} out of {len(dir_list)}")