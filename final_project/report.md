# ESL Final_Project - Space Transform / 107061272 陳力豪
> GitHub link : https://github.com/VincentChen1017/ESL/tree/main/final_project

### 1. Introduction of the Problem and my Solution
  原先我要實作的是，在RGB space以及lab space間的顏色轉換，並且透過reference_image的統計資訊來對source_image進行color correction。不過在實作過程中會使用到log以及浮點數的操作，而在實作過程中我遇到了一些困難且沒辦法有效的解決，故我將我的題目簡化成單純的Space transform。希望透過Single Core以及Multi Cores(3-Cores)的硬體架構來比較同時處理多通道的資料以及一次處理一個通道的資料在硬體面積以及所費時間上究竟差了多少。

### 2. Implementation details(Hardware & System Architecture)



### 3. Experimental results



### 4. Discussions and conclusions
&emsp;&emsp; 這次的Project中，我學會了挑選特定的演算法並且進行驗證還有電路合成的方法。不過在高階合成的結果有時候會與預期的結果不太符合（例如：PIPELINE後面積反而變小），我想這與高階合成內部的演算法優化有關。我想之後應該要學習準確的「分析合成結果」才是，能更準確得掌握合成出來的電路才能實作出與預期相符且容易除錯的電路。


