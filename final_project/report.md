# ESL Final_Project - Space Transform / 107061272 陳力豪
> GitHub link : https://github.com/VincentChen1017/ESL/tree/main/final_project

### 1. Introduction of the Problem and my Solution
  原先我要實作的是，在RGB space以及lab space間的顏色轉換，並且透過reference_image的統計資訊來對source_image進行color correction。不過在實作過程中會使用到log以及浮點數的操作，而在實作過程中我遇到了一些困難且沒辦法有效的解決，故我將我的題目簡化成單純的Space transform。希望透過Single Core以及Multi Cores(3-Cores)的硬體架構來比較同時處理多通道的資料以及一次處理一個通道的資料在硬體面積以及所費時間上究竟差了多少。

### 2. Implementation details(Hardware & System Architecture)

* Input Image: lena, Size: 256*256.

* RISCV-VP System Architecture:
![1654449735610@2x](https://user-images.githubusercontent.com/98183102/172063138-1d02ffe6-409e-496b-85cb-5ae42281708e.jpg)
此為 Multicores 架構的riscv platform架構圖。總共會有3個Core來驅動3顆獨立的PE分別對3個channel做運算。

* HLS Architecture:
![1654450797328@2x](https://user-images.githubusercontent.com/98183102/172063189-65e5c76e-e80c-4377-a74e-a9d001fa3438.jpg)
以為HLS-Module parallel的架構圖。3個獨立的Module會同時對3個Channel做運算。



### 3. Experimental results

* RISCV-VP single core:
![1654452323618@2x](https://user-images.githubusercontent.com/98183102/172064177-b1ed2681-f8fb-4144-bcf8-372c863be45c.jpg)

* RISCV-VP multicore core:
![1654452335443@2x](https://user-images.githubusercontent.com/98183102/172064185-a5ec9271-a610-4467-bf25-b2c57ef8a7c2.jpg)

* HLS without module parallel:
![1654452024486@2x](https://user-images.githubusercontent.com/98183102/172064277-cd8f4b66-1e68-4739-89a5-a426590a7862.jpg)

* HLS with module parallel:
![1654452035517@2x](https://user-images.githubusercontent.com/98183102/172064299-99fc7b26-ecc5-40b6-a85b-cb822f234adc.jpg)

* HLS Comparison Table   
![1654452274391@2x](https://user-images.githubusercontent.com/98183102/172064360-121bd7fb-4d10-4b08-92dc-e6e1bbbeebb7.jpg)



### 4. Discussions and conclusions
從RISCV-VP singlecore的模擬結果來看，當使用DMA時，整個模擬所需要的時間以及CPU所執行的instruction確實比沒有使用DMA時來的短少，這也符合我們使用DMA來避免CPU load/store 的指令佔據其運算效能的目的。<br/>
不過Multicore的模擬結果似乎就有點奇怪，當我使用3個core來平行運算的時候，不論是否使用DMA，整體的模擬時間以及使用的instruction數目都較singlecore來的多，甚至使用DMA的效能還比沒有使用DMA的效能還要差。我想這個原因跟我在做Space transform需要頻繁的從Memory讀取pixels的資料有關，當我讀取資料的次數非常頻繁且我Core的數目拉高到3顆的時候，雖然運算的部分會有平行的效果沒錯，但是由於我架構內的DMA僅有一組，所以三顆獨立的Core在做DMA的使用時，其實是需要互相錯開互相等待的。所以我推論在這邊整個模擬的bottleneck已經不在是運算的部分，而是問題出在沒能有效的讓3顆Core或的資料才會導致Multicore的效能較差的情況出現。






