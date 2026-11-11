# ROS2 导航培训

本仓库通过 **Fork + Pull Request（PR）** 收作业。后面两次任务也交到这里，放在自己目录下对应的“导航培训2”“导航培训3”文件夹中。

## 一、本周作业

在自己的工作空间里创建一个 `ament_cmake` 功能包，用 C++ 写一个发布者和一个订阅者。发布者发消息，订阅者每收到一条，就在终端显示原文和对应回复，例如“你好 → 你好呀”。内容自己定。用一个 launch 文件同时启动两个节点。

周末提交功能包源码（含 launch 文件）、一张能看到消息和回复的终端截图，以及一份写明编译和启动命令的 README。

**参考学习视频**

- [在 Ubuntu 中安装 ROS2](https://www.bilibili.com/video/BV1Jz421B7Ey/)
- [编写第一个 C++ 节点](https://www.bilibili.com/video/BV1FH4y1c7Px/)。打开后在右侧合集依次看 2.1.2、2.3、2.4（节点、功能包与工作空间），3.1、3.2.1、3.2.2（消息收发），4.6.1（launch 启动）。

## 二、作业放哪里

每个人在仓库最外层建一个自己的文件夹，按“入学年份后两位＋专业简称＋姓名”命名，例如“26电科张三”。本次作业放在里面的“导航培训1”文件夹。

~~~text
26电科张三/
└── 导航培训1/
    ├── chat_demo/         ← 自己写的功能包，名字自己定
    │   ├── package.xml
    │   ├── CMakeLists.txt
    │   ├── src/           ← C++ 源码
    │   └── launch/        ← 启动文件
    ├── 运行截图.png
    └── README.md          ← 自己的编译与启动说明
~~~

从自己 ROS2 工作空间的 src 里复制整个功能包。有头文件就连同 include 文件夹一起复制。build、install、log 是生成文件，留在自己的电脑上。个人说明写在自己的目录里，仓库最外层的 README 是大家共用的。

## 三、怎么提交

### 第 1 步 · Fork 本仓库

登录 GitHub，打开[本仓库](https://github.com/nohehe/ros2-nav)，点右上角 **Fork → Create fork**。

完成后，页面左上角应显示“你的用户名 / ros2-nav”。这是你自己账号下的副本。

### 第 2 步 · 把自己的副本下载到电脑

在自己的副本页面点绿色 **Code → HTTPS**，复制地址。

打开 Ubuntu 终端，输入 `git clone`、一个空格，再粘贴地址，回车执行。下载完成后输入：

~~~bash
cd ros2-nav
~~~

此时终端已经进入提交仓库。后面的 git 命令都在这个目录里执行。若下载时改过仓库名，cd 后面用实际目录名。

### 第 3 步 · 放入自己的作业

按第二节的目录结构建好文件夹，复制功能包、截图和启动说明。可以输入 `xdg-open .` 打开当前文件夹，再用鼠标操作。

“26电科张三”换成自己的信息，“chat_demo”换成自己的功能包名。

### 第 4 步 · 上传到自己的副本

回到终端，把下面的“26电科张三”换成自己的文件夹名，逐行执行：

~~~bash
git add "26电科张三/导航培训1/"
git status
git commit -m "导航培训1 - 26电科张三"
git push
~~~

执行 git status 后，检查准备上传的文件，确认源码、截图和说明都在自己的目录下，再继续执行后两行。

### 第 5 步 · 发 PR，交给负责人

回到自己副本的网页，点 **Contribute → Open pull request**，或 **Compare & pull request**。

核对两边选项：

- **目标（base）** 选 nohehe/ros2-nav，分支 main。
- **来源（head）** 选自己的副本，以及实际上传作业的分支。按本文步骤操作就是 main。

标题写“导航培训1 - 你的个人文件夹名”，按模板填上作业目录、启动说明和截图位置，再点 **Create pull request**。

### 第 6 步 · 确认交成功

打开[原仓库的 Pull requests 列表](https://github.com/nohehe/ros2-nav/pulls)，能看到自己的标题和 PR 编号就交好了。点进 PR 的 **Files changed**，检查里面是否有自己的源码、截图和说明。

**只做 git push，文件会上传到自己的副本。完成创建 PR 这一步，负责人才能在原仓库里收到作业。**

## 四、遇到问题

- 找不到 PR 按钮时，在原仓库点 **Pull requests → New pull request → compare across forks**，按第 5 步选择目标和来源。
- push 报 403 时，运行 `git remote -v`，确认 origin 地址指向自己的副本，同时确认登录的是自己的账号。
- git commit 提示 **Please tell me who you are** 时，先执行 `git config --global user.name "你的名字"` 和 `git config --global user.email "你的邮箱"`，再重新提交。
- 其他问题可以在群里问，附上操作步骤和报错截图。
