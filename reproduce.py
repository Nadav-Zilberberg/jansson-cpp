#!/usr/bin/env python3

import os
import subprocess
import sys

def run_command(cmd, cwd=None):
    """Run a command and return its output"""
    try:
        result = subprocess.run(cmd, shell=True, cwd=cwd, capture_output=True, text=True)
        return result.returncode, result.stdout, result.stderr
    except Exception as e:
        return -1, "", str(e)

def main():
    print("=== Current README.rst content ===")
    with open("README.rst", "r") as f:
        print(f.read())
    
    print("\n=== Checking if test coverage commands work ===")
    
    # Check if we can run the test coverage commands
    commands = [
        "cd tests && mkdir -p build && cd build",
        "cd tests/build && cmake ..",
        "cd tests/build && make",
        "cd tests/build && ./jansson_tests",
        "cd tests/build && lcov --ignore-errors mismatch --directory . --capture --output-file coverage.info",
        "cd tests/build && genhtml coverage.info --output-directory coverage_html"
    ]
    
    for cmd in commands:
        print(f"\n--- Running: {cmd} ---")
        ret, stdout, stderr = run_command(cmd)
        if ret != 0:
            print(f"Error: {stderr}")
            return False
        else:
            print("Success!")
    
    return True

if __name__ == "__main__":
    sys.exit(0 if main() else 1)
