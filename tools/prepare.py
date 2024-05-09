# python3 -m venv ./env/
# pip3 install --user meson
# sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
# clang-format -style=google -dump-config > .clang-format

# pip install conan
# https://docs.conan.io/2/reference/config_files/profiles.html
# conan install . --output-folder=buildDir --build=missing --profile:build=config/myprofile --profile:host=myprofile
# meson setup --native-file buildDir/conan_meson_native.ini  buildDir .
# meson compile -C buildDir
# meson test -C buildDir
