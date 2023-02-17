sudo apt update;
sudo apt install -y libgl-dev libvulkan-dev;
qt-cmake /app -G Ninja -B /app/build;
echo "Build start"
cmake --build /app/build;
echo "Build finish"
/app/build/bin/$(find /app/build/bin/ -maxdepth 1 -type f -executable | sed 's/.*\///g')