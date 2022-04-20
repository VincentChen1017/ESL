# ESL Midterm_Project - MergeSort / 107061272 陳力豪
> GitHub link : https://github.com/VincentChen1017/ESL/tree/main/midterm_project

### 1. Introduction of the Problem and my Solution
&emsp;&emsp; 在這次的Project中，我挑選的題目是「MergeSort」。我會先透過SystemC TLM進行演算法的驗證，之後再將架構改為可以讓Stratus進行合成的版本。並寫會進行「BASIC」、「UNROLL」還有「PIPELINE」三種不同datapath的分析。

### 2. Implementation details
&emsp;&emsp; 在進行HLS之前，我們必須注意要將in/out port修改成可以合成以及可以跑Stratus的port。例如System.h檔中的sc_fifo必須改成cynw_p2p以及GaussFilter.h檔中的sc_fifo_in必須改成scynw_p2p<>::in ......。

- UNROLL:
由於Filter必須進行3x3的卷積，故會有一個雙迴圈，首先我對他們兩個迴圈進行complete Unroll。
![1650383272784@2x](https://user-images.githubusercontent.com/98183102/164043841-cbad7965-3e62-4a9e-a3af-aac045e560e1.jpg)<br />

- PIPELINE:
接著每個迴圈內部其實會進行數值的運算並且累加，故我利用PIPELINE的方式試著提升他們的throughput。
![1650383425090@2x](https://user-images.githubusercontent.com/98183102/164044311-01aeb56f-b3d6-4f8f-b227-d380fed5a56c.jpg)<br />

- UNROLL,PIPELINE:
最後我同時將全部的LOOP做UNROLL並且對它的累加運算做PIPELINE。試著觀察效能變化。
![1650383509548@2x](https://user-images.githubusercontent.com/98183102/164044604-462f5c5d-d2a8-4b5b-a497-db1203e30895.jpg)<br />


### 3. Experimental results
![hw3](https://user-images.githubusercontent.com/98183102/164047370-facbac12-bd5e-47dd-ac5d-8bada37457db.jpg)<br />
&emsp;&emsp; 當我們使用BASIC版本的時候，他合成的面積確實是最小的，但是由於datapath沒有進行優化或者是平行調整的關係，Simulation times相較其他三者來著長非常多。<br />
&emsp;&emsp; 而將雙迴圈整個UNROLL開後可以發現由於迴圈內部的運算全部平行處理的關係，Simulation times確實有大幅的下降，不過也因為平行運算所需要的硬體數量增加，故面積也上升。<br />
&emsp;&emsp; 而PIPELINE的部分則又使Simulation times再度下降。不過當Pipeline切的Stage越多，同時運算的Stage越多，則所需要的硬體數量又是倍數上升，故面積增加了超過2倍的大小。<br />
&emsp;&emsp; 我想在設計時，我們應該選擇要特別著重面積或者是timing來在PIPELINE以及UNROLL間做出不同的調整。

&emsp;&emsp; 而下圖是我將PIPELINE的效應模擬到TLM上的結果，由於PIPELINE會提升for迴圈內累加運算的throughput。故我在TLM read-socket前加了280 cycle的delay來模擬經過PIPELINE後的throughput，可以看到TLM模擬PIPELINE的Simulation time與上方HLS跑出來的Simulation time非常接近。<br />
![1650381267761@2x](https://user-images.githubusercontent.com/98183102/164046833-c5b598b4-715f-4e7a-b869-392359aca3f9.jpg)<br />
![1650381207132@2x](https://user-images.githubusercontent.com/98183102/164046850-f623e79d-ad69-499e-a022-05d769fc3215.jpg)<br />


### 4. Discussions and conclusions
&emsp;&emsp; 這次的作業讓我學會了使用HLS來進行電路的合成，由於HLS在Datapath上的便利性，可以直接使用各種Directive即可達到各種datapath處理的效果。不過由於內部隱含著很多最佳化的演算法，故最後合成出來的結果可能與我們想像的大相徑庭，或者是根本就很難去想像電路是如何被優化的。故在使用HLS時我認為必須要非常小心，要仔細的評估目前所下的各種Directive才是。


