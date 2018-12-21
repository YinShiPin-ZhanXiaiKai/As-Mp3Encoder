## 说明
### 来源：
该项目主要是修改自：https://github.com/zhanxiaokai/Android-Mp3Encoder  
非常感谢晓凯老师！
### 目的：
原项目是Eclipse工程，现在改由AndroidStudio工程.  

### 改动
1 《音视频开发进阶指南》2.3.4 使用LAME编码MP3文件，用例是编码一个左右声道的pcm文件。 
  我找到的是一个采样率 ：16000 固定值。 编码：16bit 位深的单声道文件。  
  所以在src/main/jin/mp3_encoder/libmp3_encoder/mp3_encoder.cpp加入了void Convert()方法，用于
  编码单通道的问题。同时MainActivity.java修改采样率，比特率等参数

2. compileSdkVersion targetSdkVersion minSdkVersion进行了改动。
3. 引入了apiLeve23以上的动态权限
4. 其中定然有很多不足之处，请大家斧正！