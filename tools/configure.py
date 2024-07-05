import argparse
import subprocess

parser = argparse.ArgumentParser(description='Run a series of commands for setting up the environment.')
parser.add_argument('build_dir', type=str, help='Directory for build output')

args = parser.parse_args()
build_dir = args.build_dir


commands = [
    "sudo apt-get install python3 python3-pip python3-setuptools python3-wheel ninja-build",
    "python3 -m venv env",
    "pip install -r requirements.txt",
    "sudo bash -c \"$(wget -O - https://apt.llvm.org/llvm.sh)\"",
    "sudo rm /usr/bin/clang++",
    "sudo ln -s /usr/bin/clang++-18 /usr/bin/clang++",
    f"conan install . -of={build_dir} --build=missing -pr:a=./config/debug_profile",
]

for cmd in commands:
    print(cmd)
    subprocess.run(cmd, shell=True, check=True)
