# Qt_Wzq
* Linux Qt
* 基于Linux 和 Qt 的联机五子棋游戏。创建-加入房间的玩法，支持游戏房间内聊天。
* 服务端：基于Linux 环境 Socket网络编程，采用Epoll模型。数据库使用MySql存储用户信息。
* 客户端：基于Linux 环境 使用 Qt 框架编写游戏界面，采用多线程编程，主窗体向服务端发送信息，新建线程用来阻塞循环监听接收服务端信息。
## 登录界面
![](samples/d186c5e4a82eda50d5096ec14592e89d.JPG)
## 主窗体
![](samples/2dc3d247c73c4d93e2862a655aba9c36.JPG)
## 游戏窗体，等待对方加入
![](samples/e4f35c5e492248063c550b8af290e9e3.JPG)
## 进行游戏
![](samples/b2905721eff0b0652446ddd31a50c675.JPG)
## 游戏结束
![](samples/58dcc0795c0370aac1e62bba01efc8fb.JPG)
