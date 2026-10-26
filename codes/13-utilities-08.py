import subprocess
from pathlib import Path


def run(cmd, data=None, timeout=5):
    try:
        return subprocess.run(cmd, input=data, capture_output=True,
                              check=True, timeout=timeout).stdout
    except subprocess.CalledProcessError as e:
        raise SystemExit(f"ERROR: {cmd} exit={e.returncode}\n"
                         + e.stderr.decode(errors="replace"))
    except (OSError, subprocess.TimeoutExpired) as e:
        raise SystemExit(f"ERROR: {cmd}: {e}")


# 在包含 gen.cpp、my.cpp、std.cpp 的目录运行；单次运行限时 5 秒。
for name in ("gen", "my", "std"):
    run(["g++", "-std=c++17", "-O2", f"{name}.cpp", "-o", f"{name}.exe"],
        timeout=None)

i = 0
while True:
    i += 1
    print(f"Test {i}", flush=True)
    data = run(["./gen.exe"])
    Path("in").write_bytes(data)  # 出错时保留本次输入。
    actual = run(["./my.exe"], data)
    expected = run(["./std.exe"], data)
    if actual.split() != expected.split():
        Path("my_out.txt").write_bytes(actual)
        Path("std_out.txt").write_bytes(expected)
        print(f"WA on test {i}")
        break
    print("AC", flush=True)
