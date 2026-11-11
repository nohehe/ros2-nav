# ROS2 导航培训 1 · 作业提交

本周作业通过 Fork + Pull Request（PR）提交。Fork 会把仓库复制到你自己的账号，PR 用来把本次作业交给负责人查看。

## 导航培训 1 · 本周作业

在自己的工作空间里创建一个 ament_cmake 功能包，用 C++ 写一个发布者和一个订阅者。发布者发消息，订阅者每收到一条，就在终端显示原文和对应回复，例如“你好 → 你好呀”。内容自己定。用一个 launch 文件同时启动两个节点。

提交功能包源码（含 launch 文件）、一张能看到消息和回复的终端截图，以及一份写有编译和启动命令的 README。学习入口见[鱼香 ROS 视频合集](https://www.bilibili.com/video/BV1Jz421B7Ey/)。依次看 2.1.2、2.3、2.4、3.1、3.2.1、3.2.2、4.6.1。

## 第一次怎么交

1. 在本仓库右上角点 **Fork → Create fork**。进入自己账号下的副本，点 **Code** 复制 HTTPS 地址。
2. 在终端执行以下命令。把第一行的地址换成刚复制的地址。

   ```bash
   git clone https://github.com/你的用户名/ros2-navigation-training.git
   cd ros2-navigation-training
   git switch -c submit-week1
   ```

3. 在仓库里建自己的目录。例子中的“我的用户名”换成自己的 GitHub 用户名。本周材料放在 `submissions/我的用户名/导航培训1/`。功能包应包含 package.xml、CMakeLists.txt、源码和 launch 文件。
4. 放好本周作业后执行以下命令，把目录换成自己的实际路径。

   ```bash
   git add "submissions/我的用户名/导航培训1/"
   git commit -m "导航培训1 - 我的用户名"
   git push -u origin submit-week1
   ```

5. 回到 GitHub，点 **Compare & pull request**。目标选培训原仓库的 **main**，来源选自己副本的 **submit-week1**。标题写“导航培训1 - 我的用户名”，按模板填写后点 **Create pull request**。看到 PR 编号就交好了。

收到修改意见时，在同一分支修改并再次提交、推送，原 PR 会自动更新。
