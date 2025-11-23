import subprocess

# Get cmake version output
result = subprocess.run(['cmake', '--version'], capture_output=True, text=True)
cmake_output = result.stdout

# Write output to test.txt
with open('test.txt', 'w') as f:
    f.write(cmake_output)

print("Created test.txt with cmake version output")
