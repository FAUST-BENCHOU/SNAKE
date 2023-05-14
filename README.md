# SNAKE游戏介绍
这是一个从一维到四维（四维是瞎想的）的流程类贪吃蛇游戏，颠覆了以往对贪吃蛇的认知，想要表达的是对于自身所处维度和阶级的思考。
## 游玩时间：
6mins
## 涉及到的部分技术：
**OPENGL ， Easyx ， opencv** ， Windows API ， 一点点的文件操作 ， 一点点的多线程和Messagebox  
>在这个项目中用了许多已经开源了的代码，我将把它们放到文章最后，尊重源码出处
## 涉及到的部分主题：
规则怪谈 ， 怪核 ， 梦核 
### （本人技术还不够成熟，所以可能会有一些小瑕疵和想不到的bug......后面还会改进的）
## 如何开始
如果你真的有兴趣要亲手上手这个游戏的话，首先你需要在vs配置几个库，我将给出相关教程的链接，  
首先是glut和freeglut(https://blog.csdn.net/Huzy_ccli/article/details/91044530)     
其次需要的是opencv(https://blog.csdn.net/mars_xiaolei/article/details/78759041)   
再安装下easyx(https://blog.csdn.net/m0_57037805/article/details/127101970)  
然后由于我的项目源文件和头文件太多了到后面给我整晕了（好吧是我太菜了抱歉），所以可能需要你在链接器的命令行中加入"/FORCE:MULTIPLE"!
(https://blog.csdn.net/Xiuhua2017/article/details/109728879) 如果你不想继续使用，使用完后删除就可以了  
接下来就可以开始游戏了，请注意，游戏时长为6mins，没到这个时间前请不要随意关闭窗口，希望你能配合配合我，游戏还不成熟......
## 游戏截图
![15K`FA~KN(N3WZB$Q6_2 WV](https://github.com/FAUST-BENCHOU/SNAKE/assets/126341483/0a512fdc-b515-48a6-8302-3e2af38a7db1)

![M34%J XX@95O5T }~SHGV7B](https://github.com/FAUST-BENCHOU/SNAKE/assets/126341483/199086f6-d905-4a24-8f00-e7f91d2613e7)

![(`(TDGPZI7MP MW19DY{2ZH](https://github.com/FAUST-BENCHOU/SNAKE/assets/126341483/995d97d1-dc1a-46ed-b3ec-e4084b8edb71)

![YJRB06YB J7W%WY`G9PH%CS](https://github.com/FAUST-BENCHOU/SNAKE/assets/126341483/17d4dcb6-cffb-443e-8efa-3eba6dd075d9)
![CP77BZ3I$K$2HA300PNAUO3](https://github.com/FAUST-BENCHOU/SNAKE/assets/126341483/5cb389ba-5b34-4728-adfd-c8317d307a71)
![Q XJ0 N~F)N$DD~Q XMZT3H](https://github.com/FAUST-BENCHOU/SNAKE/assets/126341483/7b393d71-246c-44ec-a06a-bb11b3199aab)

### 注意：以下内容将含有剧透内容，请谨慎观看
-------------------------------------------
-------------------------------------------
-------------------------------------------
## 游戏流程：
从开始游戏的**一维**开始，是最为简易的界面和幼稚的语言，你只能够操作一个方向键来控制贪吃蛇进行移动，也只有一个方向来前进，在吃完有限的食物后，我会强制性让游戏结束，
好像让整个进程结束了，实则是推着你的进入了**二维**的贪吃蛇的界面，界面语言略微成熟，一开始的界面是正常的，但是同样在吃完3个食物后我会将蛇的速度调到极快，目的就是让你
无法通关，在死亡后我会希望你按下y再次开始游戏，事实上你也只能按y，按下别的键会反复报错，按下y后进入一个乱码的界面，乱七八糟的语法结构，进入游戏后全屏乱码，右边的游戏
变为了规则怪谈，得分很大，但是当你在乱码中吃完3个食物后依然会到极快的速度，也就是为了让你死亡，之后我会再次让你重新开始，同样是只能输入y，这时下一个界面我会加入
色彩元素，在随机位置随机颜色打印随机字符，表示即将进入有颜色和层次的**二点五维** ，这个地方没有加速，因为食物已经很难找到了，找到也没用，我压根就没设置赢的
条件，你所能做的只能是自杀或者占满整个界面然后撞墙死，然后会满屏的红字警告，进入模拟的蓝屏故障画面，鼠标会在屏幕乱飞，随后进入二点五维 ，画面可爱，但是在吃完两个食物后，食物会越来越多，但是你一个也吃不到，在天使和魔鬼之间反复变换，最后只能是结束游戏，在这之后，我将会在屏幕上放出几张剧烈的人类表情的图片，意味着你将要进入**三维** ，随后打开你的摄像头，让你自己看看你自己因为这几张图片而带来的微妙的表情变换，也象征着维度壁的打破，我从电脑屏幕的维度窥探到了你的维度的世界，随后写入记事本给出进入三维的警告，利用The Curse of Babylon里的诗句来构造出了满是蛇的没有出口的神殿的场景，并把你拉了进去，给你80s的时间在这个怪核空间探索，并且放出相关的bgm,时间到后又会将你以同样的形式拉入满是蛇的**四维**空间中（这个是我瞎想的，凑合着看），10s后退出，给你放出之前诗句里的一段话“你在里面，抑或是在外面”，询问我们之间的维度转换，随后给出一个假的小彩蛋，要求你在15s内输入特定内容，不然就删除你的c盘内容，事实上什么也不会发生，只是为了给你以一种紧张感，“现在，我们是谁在玩谁”，至此，游戏结束  
## 我想通过游戏表达的（个人观点，不喜勿喷）：
在我眼里我们每个人都处于自己的一个小的维度上，我们限于我们自身总是只能在自己的维度上看待事物和问题，在这个有限的维度上为了去到更高的维度而不断的为自己同一个维度的事物所斗争（如游戏中的食物等），但事实上是，当你进入到另一个更高的维度时，这个维度仍然会有事物需要你去斗争，以此循环，但事实上，每个维度都有比其更高的维度，都会有更高级的形式对其进行控制，就像当你开始游戏的那一刻起，表面是你在玩这个游戏，事实上我也通过这个游戏对你进行了一系列控制，我也在进入四维的话中写了“In what dimension do you reside?”以及“In what class do you belong?”，那我们在当今世界处于哪个维度，哪个阶级，我们又应该如何看待我们所处的层次，是我们所需要思考的。
## 全流程视频（to do）
#### 一些源码出处：
二维原型（https://zhuanlan.zhihu.com/p/126175267 ）   
二点五维原型（https://codebus.cn/taor/rt-snake ）  
蓝屏原型（https://blog.csdn.net/jinboxin_/article/details/124178915 ）  
Messagebox（https://download.csdn.net/download/uuuhhjjk/87696438?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-download-2%7Edefault%7ECTRLIST%7EAntiPaid-1-87696438-blog-105110236.235%5Ev32%5Epc_relevant_default_base3&depth_1-utm_source=distribute.pc_relevant.none-task-download-2%7Edefault%7ECTRLIST%7EAntiPaid-1-87696438-blog-105110236.235%5Ev32%5Epc_relevant_default_base3 ）  
结尾原型（https://codebus.cn/yangw/word-art-freeze ）  


