"""Run the actual stress script against tiny C++ programs, including failures."""
import subprocess
import sys
import tempfile
from pathlib import Path

script = Path(__file__).resolve().parents[1] / "codes/13-utilities-08.py"
output = '#include <cstdio>\nint main() { puts("42"); }'
crash = '#include <cstdio>\nint main() { fputs("boom", stderr); return 7; }'
hang = 'int main() { while (true) {} }'
# A persistent counter ends an otherwise successful comparison after two tests.
twice = '''#include <cstdio>
int main() {
    int n = 0;
    if (FILE *f = fopen("count", "r")) { fscanf(f, "%d", &n); fclose(f); }
    FILE *f = fopen("count", "w"); fprintf(f, "%d", n + 1); fclose(f);
    if (n == 2) { fputs("done", stderr); return 7; }
    puts("42");
}'''

cases = [
    ("WA", {}, "WA on test 1", 0),
    ("CE", {"my": "invalid C++"}, "exit=", 1),
    ("gen RE", {"gen": crash}, "boom", 1),
    ("my RE", {"my": crash}, "boom", 1),
    ("std RE", {"std": crash}, "boom", 1),
    ("timeout", {"my": hang}, "timed out", 1),
    ("AC then RE", {"gen": twice, "my": output}, "done", 1),
]
build = script.parents[1] / "build"
build.mkdir(exist_ok=True)
for label, overrides, message, exit_code in cases:
    with tempfile.TemporaryDirectory(prefix="stress test ", dir=build) as folder:
        root = Path(folder)
        sources = {"gen": output, "my": output.replace("42", "43"), "std": output}
        sources.update(overrides)
        for name, source in sources.items():
            (root / f"{name}.cpp").write_text(source, encoding="utf-8")
        result = subprocess.run([sys.executable, str(script)], cwd=root,
                                capture_output=True, timeout=30)
        log = (result.stdout + result.stderr).decode(errors="replace").replace("\r", "")
        assert result.returncode == exit_code, (label, log)
        assert message in log, (label, log)
        assert "Test 4" not in log, (label, log)
        if label not in {"CE", "AC then RE"}:
            assert "Test 2" not in log, (label, log)
        if label in {"my RE", "std RE", "timeout", "WA"}:
            assert (root / "in").read_bytes().strip() == b"42"
        if label == "WA":
            assert (root / "my_out.txt").read_bytes().strip() == b"43"
            assert (root / "std_out.txt").read_bytes().strip() == b"42"
        if label == "AC then RE":
            assert log.count("AC\n") == 2 and "Test 3" in log, log
        print(f"PASS: {label}")
