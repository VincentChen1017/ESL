# ESL Homework_1 report / 107061272 陳力豪
> GitHub link : https://github.com/VincentChen1017/ESL/tree/main/Homework_1

### 1. Introduction of the Problem and my Solution
這次的作業是要實作Gaussian Blur的演算法，並且要求兩種不同的Pixel transfer版本。<br />

第一種是基於lab02-Sobel Filter的範例，單純利用FIFO Channels來做。<br />
在計算Convolution時，一次僅將「必要」的「一個」Pixel送入Filter內做運算。<br />

第二種是內部有Row-Buffer來暫存Row Pixels的版本。<br />
在計算Convolution前，會先一次讀入「三個Rows」的Pixels，並利用這三個存在Filter內（local）的buffer做運算。<br />


### 2. Implementation details
在Gaussian Blur中，Kernel的定義方法是從2D的高斯分佈來取得。將kernel中心定義為原點，並且選定好分佈的範圍後透過下列公式可以求得Kernel的值：G(x, y) = exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * pi * sigma * sigma)。<br />

不論是版本一或是版本二的運算都是3x3 convolution的運算，將pixel讀入filter後只要將3x3 convolution的點乘完成並且加總之後就可以得到一個pixel的輸出結果。<br />

要特別注意的是，由於我希望輸出圖片尺寸與輸入圖片相同，故我在做pixel讀取的時候，會將width以及height左右各多加一單位，並利用判斷式來判斷當前讀取的地方是否為圖片邊緣，
若是的話就會選擇將輸入filter的pixel data設成0，如此來實現zero-padding（如下圖）。<br />
![1646793854136@2x](https://user-images.githubusercontent.com/98183102/157362605-ce1733ae-cfac-46ab-a387-0a1a7f5c6a43.jpg)<br />


### 3. Additional features of your design and models
在Row-Buffer Based的實現中，要特別注意的一點是，由於我只開3個buffer來儲存3個Row的pixel data。所以說當前運算使用的data以及下一次新讀進的Row data都僅能在這3個Buffer內做操作。<br />

而我實現的辦法是：先定義Row[0]、Row[1]、Row[2]分別為由上往下的第一層～第三層，而只要當前3個Row的卷積運算完畢，則我會「依序」將Row[1]先shift給Row[0]，然後Row[2]shift給Row[1]，最後只要再將新讀進的Row data存放在R[2]就可以成功完成以下三個動作：1.保留舊的Row data 2.重新排列Row-Buffer 3.存入新的Row data。而重複上述操作就可以完成整張圖片的Convolution運算而不需要開設過多的Buffer或者是重複讀取多次相同的pixel data。

### 4. Experimental results
The original picture:

![the original](https://user-images.githubusercontent.com/98183102/157357297-e57a3973-75d3-42b7-ab13-8766c6e5d721.png)

The blur picture generated from GaussianBlur filter(版本一跟版本二的Kernel參數選用一樣，故成像一樣）

![blur](https://user-images.githubusercontent.com/98183102/157357656-6d226cec-3221-4456-8a1c-3b5a8d281f83.png)


### 5. Discussions and conclusions
The Simulation time and the Pixels transfer times(from tb to filter) of wFIFO version:<br />
![1646767716180@2x](https://user-images.githubusercontent.com/98183102/157358129-725ca57e-5517-4ecd-a40f-d85851bc9038.jpg)<br />


The Simulation time and the Pixels transfer times(from tb to filter) of Row-Based version:<br />
![1646767897587@2x](https://user-images.githubusercontent.com/98183102/157358079-38fee890-17c1-4b6f-b6e3-633f71b2cb7c.jpg)<br />

在第一種wFIFO版本的架構中，Filter在讀取資料的時候每次僅會讀取3x3 convolution內的其中一個pixel進來做點乘運算，<br />
所以當3x3 convolution stride=1 往右滑的時候，下一次3x3 convolution運算讀取都會「重複讀到」上一次3x3 convolution運算時就已經使用過的6個pixel data。<br />

而第二種Row-Buffer Based的架構中，由於Filter會一次讀進3個Row的pixel data並且「暫存」起來，當3x3 convolution往右滑時，<br />
所有的資料都已經存在Row-Buffer內，故「不會」有重複讀取pixel data的問題。<br />

由上圖的模擬結果來看，版本一在Pixel transfer times from testbench to filter的次數為「589824」次，而版本二則僅有「66564」次。<br />
從硬體的角度來解讀的話，版本二新增的Row-Buffer大幅的降低了Memory與CPU間的資料傳輸要求，這可以讓Power consumption的efficiency有大幅的改善。

而由於程式內部的wait(delay_time)目前僅僅只是用來模擬整個behavior的行為，不帶有實際上特別的意義(delay_time可以任意改動)，<br />
故我認為此處的Simulated time不像Pixel transfer times那樣有特殊的相對關係。

