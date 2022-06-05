# ESL Final_Project - Space Transform / 107061272 陳力豪
> GitHub link : https://github.com/VincentChen1017/ESL/tree/main/final_project

### 1. Introduction of the Problem and my Solution
  原先我要實作的是，在RGB space以及lab space間的顏色轉換，並且透過reference_image的統計資訊來對source_image進行color correction。不過在實作過程中會使用到log以及浮點數的操作，而在實作過程中我遇到了一些困難且沒辦法有效的解決，故我將我的題目簡化成單純的Space transform。希望透過Single Core以及Multi Cores(3-Cores)的硬體架構來比較同時處理多通道的資料以及一次處理一個通道的資料在硬體面積以及所費時間上究竟差了多少。

### 2. Implementation details(Hardware & System Architecture)

* RISCV-VP System Architecture:
![1654449735610@2x](https://user-images.githubusercontent.com/98183102/172063138-1d02ffe6-409e-496b-85cb-5ae42281708e.jpg)


* HLS Architecture:
![1654450797328@2x](https://user-images.githubusercontent.com/98183102/172063189-65e5c76e-e80c-4377-a74e-a9d001fa3438.jpg)




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



