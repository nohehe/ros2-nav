ros版本：   ros2 humble
功能包位置：  src/node_cpp
文件夹目录：
├── 25机器人潘鑫润
│   └── 导航培训1
│       ├── README.md
│       ├── src
│       │   └── node_cpp
│       │       ├── CMakeLists.txt
│       │       ├── launch          ← 启动文件
│       │       ├── LICENSE
│       │       ├── package.xml
│       │       └── src             ← C++ 源码
│       └── Ubuntu-22.04-2026-11-15-19-21-52.png         ← 执行文件后的截图
└── README.md


#编译命令： 
        cd 导航培训1
        colcon build
        source install/setup.bash
        ros2 launch node_cpp demo.launch.py