import subprocess

commands = [
    "sudo apt-get install python3 python3-pip python3-setuptools python3-wheel ninja-build",
    "python3 -m venv env",
    "pip install -r requirements.txt",
    "sudo bash -c \"$(wget -O - https://apt.llvm.org/llvm.sh)\"",
    "sudo rm /usr/bin/c++",
    "sudo ln -s /usr/bin/clang++-18 /usr/bin/c++",
    "sudo rm /usr/bin/clang++",
    "sudo ln -s /usr/bin/clang++-18 /usr/bin/clang++",
    "conan install . -of=buildDir --build=missing -pr:a=./config/debug_profile",
]

for cmd in commands:
    print(cmd)
    subprocess.run(cmd, shell=True, check=True)
