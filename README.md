# Chain Normalization
#### Efficient Spatial-Temporal Normalization in Surface of Active Events

**Prerequisite**
1. Install the following packages
```
sudo apt-get install cmake gcc g++ libeigen3-dev
```
2. Install the [OpenCV](https://docs.opencv.org/3.4.6/d7/d9f/tutorial_linux_install.html) 


**How to build?**
```
git clone https://github.com/eleboss/chain.git
cd ./chain
mkdir build
cd ./build
cmake ../
make
```
**How to use?**
1. Download the txt files of event data from [DAVIS240C Dataset](http://rpg.ifi.uzh.ch/davis_data.html).
2. Unzip it.
3. Find the file **event.txt**, and change the file path in c++ code to it.
4. Repeat the build process.
5. Use the following to run
```
./asy
./chain
```
BTW, you can use the python scripts for visualization.

**Prerequisite**
```
pip install mayavi
```
Then
```
cd ./scripts
python mayavi_matc.py
```

#### Results
![demo](./images/demo.png)