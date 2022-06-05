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
此為HLS-Module parallel的架構圖。3個獨立的Module會同時對3個Channel做運算。



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
一：<br/>
從RISCV-VP singlecore的模擬結果來看，當使用DMA時，整個模擬所需要的時間以及CPU所執行的instruction確實比沒有使用DMA時來的短少，這也符合我們使用DMA來避免CPU load/store 的指令佔據其運算效能的目的。<br/>
不過Multicore的模擬結果似乎就有點奇怪，當我使用3個core來平行運算的時候，不論是否使用DMA，整體的模擬時間以及使用的instruction數目都較singlecore來的多，甚至使用DMA的效能還比沒有使用DMA的效能還要差。我想這個原因跟我在做Space transform需要頻繁的從Memory讀取pixels的資料有關，當我讀取資料的次數非常頻繁且我Core的數目拉高到3顆的時候，雖然運算的部分會有平行的效果沒錯，但是由於我架構內的DMA僅有一組，所以三顆獨立的Core在做DMA的使用時，其實是需要互相錯開互相等待的。所以我推論在這邊整個模擬的bottleneck已經不在是運算的部分，而是問題出在沒能有效的讓3顆Core獲得資料才會導致Multicore的效能較差的情況出現。<br/><br/>

二：<br/>
若不考慮整個System的模擬時間，僅考慮PE運算的時間的話，從HLS的模擬結果來看似乎就合理很多。<br/>
不論是BASIC或者是DPA的版本，當我拿3個channel要Serial運算的版本當Baseline來跟Module parallel的版本來比較時可以發現，當我把平行度拉成3倍時，我們運算速度確實有達到3倍甚至3倍以上的加速效果。而單看BASIC的兩種版本的話，面積增長的幅度確實也是有2.75倍將近3倍左右。故運算效能與面積變化幅度似乎都符合我們的預期（拉高平行度：效率變高、3份硬體導致面積變大）。<br/>
而DPA的兩種版本在面積增長的部分就僅有1.42倍，我深入去看合成結果發現，在DPA的版本中由於dataflow優化的關係，stratus會將一些MUX或者是adder、Div等元件做更換，所以才會導致Serial版本以及parallel 3 module版本的單一PE的面積不一樣，也才會有最終總面積倍增不是3倍的情況發生。<br/>
而由於我每顆PE內部都有使用一顆除法器（由於HLS乘以浮點數似乎會有些問題，故我先將係數放大後再除回來），而從上方表格可以看到在BASIC的版本中除法器就佔了超過一半的面積。而在DPA版本中由於Stratus優化的關係，我發現他把除法器的cell置換成另一款較小的除法器（不過其他像是MUX的cell，有些被換成比較大顆的版本），而我想也是因為在DPA優化過程中會更換cell的原因，才使DPA版本的面積變化僅有1.42倍，而非3倍之高。










