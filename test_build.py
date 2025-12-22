#!/usr/bin/env python3
import subprocess
import os
import shutil

def run_command(cmd, cwd=None):
    """Run a command and return the result"""
    print(f"Running: {cmd}")
    result = subprocess.run(cmd, shell=True, cwd=cwd, capture_output=True, text=True)
    print(f"Return code: {result.returncode}")
    if result.stdout:
        print(f"STDOUT:\n{result.stdout}")
    if result.stderr:
        print(f"STDERR:\n{result.stderr}")
    return result

def main():
    # Clean up any existing build directory
    if os.path.exists("build"):
        shutil.rmtree("build")
    
    # Create build directory
    os.makedirs("build")
    
    # Try to configure with CMake
    result = run_command("cmake ..", cwd="build")
    
    if result.returncode != 0:
        print("CMake configuration failed!")
        return False
    
    # Try to build
    result = run_command("make", cwd="build")
    
    if result.returncode != 0:
        print("Build failed!")
        return False
    
    print("Build successful!")
    return True

if __name__ == "__main__":
    main()
