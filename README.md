# ROS2 导航培训

每周作业都交到这个仓库。第一次 Fork 一份到自己的账号，以后一直用这份副本。

## 第一周 · 话题通信

在自己的工作空间里创建一个 `ament_cmake` 功能包，用 C++ 写一个发布者和一个订阅者。发布者发消息，订阅者每收到一条，就在终端显示原文和对应回复，例如“你好 → 你好呀”。内容自己定。用一个 launch 文件同时启动两个节点。

**周末交三样：** 功能包源码（含 launch 文件）、一张能看到消息和回复的终端截图、一份写明编译和启动命令的 README。

**参考视频：** [安装 ROS2](https://www.bilibili.com/video/BV1Jz421B7Ey/)；[节点与功能包](https://www.bilibili.com/video/BV1FH4y1c7Px/)（合集 2.1.2、2.3、2.4）；[话题通信](https://www.bilibili.com/video/BV1D3WXeSELP/)（3.1、3.2.1、3.2.2）；[launch 启动](https://www.bilibili.com/video/BV1hdtLeZE9y/)（4.6.1）。

## 第一次怎么交

1. **复制仓库。** 在本页右上角点 **Fork → Create fork**。进入自己账号下的副本，点绿色 **Code → HTTPS**，复制地址。
2. **下载到电脑。** 在 Ubuntu 终端输入 `git clone`，加一个空格，粘贴刚复制的地址并回车。接着输入 `cd ros2-nav` 进入仓库。如果自己的副本用了别的名字，换成实际下载的目录名。
3. **放作业。** 在仓库最外层按“入学年份后两位＋专业简称＋姓名”建个人文件夹。下面以张三为例。把自己工作空间 src 下的整个功能包复制进来，保留 package.xml、CMakeLists.txt、源码和 launch 文件。build、install、log 留在电脑上。

   ~~~text
   26电科张三/
   └── 导航培训1/
       ├── chat_demo/      ← 你的功能包，名字自己定
       ├── 运行截图.png
       └── README.md       ← 你自己的启动说明
   ~~~

4. **上传到自己的副本。** 回到仓库目录的终端，把下面的“26电科张三”换成自己的文件夹名，逐行执行。

   ~~~bash
   git add "26电科张三/导航培训1/"
   git status
   git commit -m "导航培训1 - 26电科张三"
   git push
   ~~~

5. **交给负责人。** 回到自己副本的网页，点 **Contribute → Open pull request** 或 **Compare & pull request**。目标选 **nohehe/ros2-nav 的 main**，来源选自己刚上传作业的分支。标题写“导航培训1 - 你的个人文件夹名”，填好模板，点 **Create pull request**。在[原仓库的 PR 列表](https://github.com/nohehe/ros2-nav/pulls)看到自己的标题和编号，就交好了。

## 补交和后续作业

- **改本周作业：** 在原来的目录修改，再执行第 4 步。只要原 PR 还开着，推送到同一分支就会自动更新。
- **交下周作业：** 在自己的文件夹下新建“导航培训2”，和“导航培训1”并排放。沿用同一个副本和分支，把第 4 步命令中的“导航培训1”改成“导航培训2”。原 PR 还开着就继续用，已合并或关闭就按第 5 步新提一个。
- **找不到自己的提交：** 先看自己副本的网页有没有作业。若文件已上传，再到原仓库查看 PR。点进 PR 的 **Files changed**，能看到这次交了哪些文件。
