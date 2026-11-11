# ROS2 导航培训 1｜第一周作业提交说明

这周练习让两个 ROS2 节点互相配合：一个发消息，一个收到消息后在终端显示回复。周末交三样东西：**功能包源码、一张终端截图、一份启动说明**。每人提一个 PR，负责人会在里面看作业、留反馈。

## 一、本周做什么

在自己的 ROS2 工作空间里创建一个 `ament_cmake` 功能包，用 C++ 写一个发布者和一个订阅者。消息和回复自己定。比如发布者发“你好”，订阅者收到后在终端显示“你好 → 你好呀”。最后写一个 launch 文件，让两个节点一起启动。截图要能看清收到的原文和对应回复。

## 二、文件放哪里

在提交仓库最外层建一个自己的文件夹，名字按 **入学年份后两位 + 专业简称 + 姓名** 连起来写。下面用“26电科张三”举例。打开你的文件夹，再建一层“导航培训1”，把三样材料放进去：

~~~text
26电科张三/
└── 导航培训1/
    ├── chat_demo/            ← 从自己的工作空间 src 里复制整个功能包
    │   ├── package.xml
    │   ├── CMakeLists.txt
    │   ├── src/              ← 发布者和订阅者的 C++ 源码
    │   └── launch/           ← 同时启动两个节点的文件
    ├── 运行截图.png
    └── README.md             ← 写上你实际用过的编译和启动命令
~~~

功能包名用你自己的。有头文件就连同 include 文件夹一起复制。只整理自己的个人目录，仓库首页的 README 是大家共用的。build、install、log 是编译时生成的目录，留在自己的电脑上即可。

## 三、第一次怎么交

**1. 在网页上 Fork 一份。** 登录 GitHub，打开[培训原仓库](https://github.com/nohehe/ros2-navigation-training)，点右上角 **Fork → Create fork**。新页面左上角应显示“你的 GitHub 用户名 / ros2-navigation-training”，这就是你自己的副本。

**2. 把自己的副本下载到电脑。** 在副本页面点绿色 **Code → HTTPS**，复制地址。打开 Ubuntu 终端，输入 `git clone`、一个空格，再粘贴地址并回车。下载好后进入目录：

~~~bash
cd ros2-navigation-training
~~~

**3. 放入作业。** 在刚下载的目录里，按上面的结构建好个人文件夹。可以在终端输入 `xdg-open .` 打开当前文件夹，再用鼠标复制功能包、截图和说明。

**4. 把文件传到自己的 GitHub 副本。** 回到刚才的终端，逐行执行。把下面两处“26电科张三”都换成自己的文件夹名。

~~~bash
git add "26电科张三/导航培训1/"
git status
git commit -m "导航培训1 - 26电科张三"
git push
~~~

执行到 `git status` 时，看一眼准备提交的文件，确认源码、截图和说明都在自己的目录下，再往下执行。

**5. 把作业交回培训原仓库。** 打开网页上自己的副本，点 **Contribute → Open pull request**，或者点 **Compare & pull request**。核对两边：目标（base）是 **nohehe/ros2-navigation-training 的 main**，来源（head）是**自己副本的 main**。标题写“导航培训1 - 你的个人文件夹名”，按模板填好说明，再点 **Create pull request**。

**6. 确认交成功。** 打开原仓库的 [Pull requests 列表](https://github.com/nohehe/ros2-navigation-training/pulls)，能看到自己的标题和编号（例如 #1）就交好了。点进自己的 PR，再点 **Files changed**，可以检查负责人实际能看到哪些文件。

## 四、第一次提交容易卡在哪

- **刚才已经 push 了，为什么列表里没有我？** push 只把文件传到了你自己的副本。继续做第 5 步，创建 PR 才算交到原仓库。
- **网页上找不到提 PR 的按钮？** 在原仓库点 **Pull requests → New pull request → compare across forks**，按第 5 步选目标和来源。如果之前按旧说明建了 submit-week1 分支，来源就选实际放作业的那个分支。
- **push 提示 403？** 在终端运行 `git remote -v`，看看 origin 地址里的用户名是不是你自己。如果指向原仓库，就从自己副本的 Code 按钮复制 HTTPS 地址，执行 `git remote set-url origin 你复制的地址` 后再 push，同时确认登录的是自己的账号。
- **负责人让我改一下，怎么补交？** 在原来那份代码、同一个分支上改，重复第 4 步。刷新原来的 PR 就能看到修改，每人保留一个 PR 即可。
