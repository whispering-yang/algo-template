import subprocess as sp
from pathlib import Path

G, R, C = "\033[1;32m", "\033[1;31m", "\033[0m"

def run(cmd, data=None, t=5):
    print(">", cmd)
    try:
        proc = sp.run(cmd.split(), input=data, timeout=t, capture_output=True, text=True)
    except sp.TimeoutExpired as e:
        print(f"{R}ERROR: Time Limit Exceed{C}")
        exit(1)

    if proc.returncode != 0:
        print(f"{R}ERROR: Runtime Error exit={proc.returncode}{C}")
        print(proc.stdout)
        print(proc.stderr)
        exit(1)
    return proc.stdout

sol, std, gen = "sol", "brute", "gen"

for name in (sol, std, gen):
    run(f"g++ --std=gnu++20 -O2 {name}.cpp -o {name} -DONLINE_JUDGE")

i = 0
while True:
    i += 1

    data = run(f"./{gen}")
    Path("in").write_text(data)

    out1 = run(f"./{sol}", data).rstrip()
    out2 = run(f"./{std}", data).rstrip()

    if out1 != out2:
        print(f"{R}WA on testcase {i}{C}")
        print("AC:", out2)
        print("WA:", out1)
        break

    print(f"{G}AC on testcase {i}{C}")
