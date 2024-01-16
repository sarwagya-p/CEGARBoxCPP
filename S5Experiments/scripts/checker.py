import pandas as pd
import sys

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 checker.py <benchmark_name>")
        exit()

    bench = sys.argv[1]

    results_df = pd.read_csv(f"../results/{bench}/results.csv")

    results_df["correct"] = (
                            (results_df["CEGAR"] == results_df["Cheetah"])
                            | (results_df["CEGAR"] == results_df["S52SAT"])
                            | results_df["CEGAR"] == -1
                            | ((results_df["Cheetah"] == -1) & (results_df["S52SAT"] == -1))
                            )
    
    incorrect_files = results_df[~results_df["correct"]]["File"].tolist()

    if len(incorrect_files) == 0:
        print("All files are correct!")
    else:
        print("Incorrect files:")
        print("\n".join(incorrect_files))