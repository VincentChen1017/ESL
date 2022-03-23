# ESL Homework_2 report / 107061272 陳力豪
> GitHub link : https://github.com/VincentChen1017/ESL/tree/main/Homework_2

### 1. Introduction of the Problem and my Solution
&emsp;&emsp; 在lab3以及lab4中，我們首先學習如何使用TLM建立point-to-point的傳遞。也就是引入initiator還有target的觀念，將要傳輸的資料以transaction的形式打包後，透過socket來做data/pointer的傳輸。<br />

&emsp;&emsp; 接著我們進一步學會了在initiator(Testbench)還有target(Filter)中間插入一個Bus來當作Router，來處理單一Initiator對多個Target的情況。<br />
&emsp;&emsp; 而此次作業就是把Homework_1的架構整併成Testbench - Bus - Target的形式，利用Bus來處理Target端Filter/Memory的控制訊號。<br />


### 2. Implementation details
&emsp;&emsp; 在Gaussian Blur中，Kernel的定義方法是從2D的高斯分佈來取得。將kernel中心定義為原點，並且選定好分佈的範圍後透過下列公式可以求得Kernel的值：G(x, y) = exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * pi * sigma * sigma)。<br />

&emsp;&emsp; 如同lab3以及lab4所做的，我把應該傳輸的data/pointer/address先用socket-to-socket的方式傳遞給中間的Bus，接著再透過Bus中的decoder來決定目前傳送的transaction是要丟給Filter或者是Memory做操作。

#### Input part:
&emsp;&emsp; 由於我們的pixels資訊是R,G,B三個通道的量，所以我們會使用3個Buffer來暫存data，並且宣告有效的mask量為3<br />
(有效：value=0xff)，之後再將其透過「write_to_socket」的方式，打包成transaction後傳送給Target。（如下圖所示）<br />
![1648005607405@2x](https://user-images.githubusercontent.com/98183102/159618207-06416b1e-cb84-4e4b-af77-7ecdefd3562f.jpg)<br />

並且在Bus端會將傳入的Addres進行decode，藉此決定Bus(Bus端的initial socket)要將transcation傳到哪一個target的socket中(Filter or Memory)。<br />

#### Output part:
&emsp;&emsp;  要特別注意的是在lab3、lab4之中，Sobel的輸出是「灰階」，所以僅僅只需要將「相同」的結果送往R,G,B三通道所對應的data pointer就好。但是在此作業中，由於Gaussian Filter的輸出會是「彩色」的所以必須要將Filter三個channel的輸出都利用獨立的data_buffer存起來，再將其分別給到R,G,B三通道對應的data pointer中。（如下圖所示）<br />
![1648005662522@2x](https://user-images.githubusercontent.com/98183102/159618216-59c13f92-db47-4849-942f-9e3c90c06df8.jpg)<br />

### 3. Experimental results
The original picture:

![the original](https://user-images.githubusercontent.com/98183102/157357297-e57a3973-75d3-42b7-ab13-8766c6e5d721.png)

The blur picture generated from GaussianBlur filter

![hw2](https://user-images.githubusercontent.com/98183102/159618432-1437f837-87ad-446e-a5d4-42a93e21c451.png)<br />

### 5. Discussions and conclusions
&emsp;&emsp;  這次的作業將SystemC/TLM還有如何有效的做Memory的存取(DMI)給整合起來，儘管我還不能透徹的瞭解使用DMI來操作Memory還有使用一般transaction來傳遞資料在效益上到底有多大的差異，但是透過Lab3~Lab4+HW2的練習之後，我對Testbench(Initiator) > Bus、Router(Targer+Initiator) > Filter(Target)的連接有更近一步的認識！



