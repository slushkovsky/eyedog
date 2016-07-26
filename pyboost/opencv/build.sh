echo "Build pyboost_opencv..."
rm -rf build && mkdir build && cd build  
cmake .. &>/dev/null
make -j3 
mv *.so ../../
cd .. && rm -rf build 
sudo ln -s ../../libpyboost_opencv.so /usr/local/lib/libpyboost_opencv.so
echo "Done"
