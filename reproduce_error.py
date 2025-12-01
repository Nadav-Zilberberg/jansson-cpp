#!/usr/bin/env python3

import subprocess
import sys

def run_command(cmd):
    print(f"Running: {cmd}")
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    print("STDOUT:", result.stdout)
    print("STDERR:", result.stderr)
    return result.returncode

# Try to compile the demo
ret = run_command("g++ -std=c++20 -I./src demo/json_demo.cpp -o json_demo")
if ret != 0:
    print("Compilation failed as expected")
    sys.exit(1)

# Try to run the demo
ret = run_command("./json_demo")
if ret != 0:
    print("Demo failed to run")
    sys.exit(1)

print("Demo ran successfully")
