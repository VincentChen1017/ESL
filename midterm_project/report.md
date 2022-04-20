# ESL Midterm_Project - MergeSort / 107061272 陳力豪
> GitHub link : https://github.com/VincentChen1017/ESL/tree/main/midterm_project

### 1. Introduction of the Problem and my Solution
&emsp;&emsp; 在這次的Project中，我挑選的題目是「MergeSort」。我會先透過SystemC TLM進行演算法的驗證，之後再將架構改為可以讓Stratus進行合成的版本。並寫會進行「BASIC」、「UNROLL」還有「PIPELINE」三種不同datapath的分析。<br />
&emsp;&emsp; 相較於Software實作MergeSort時需要不斷的呼叫recursive function，Hardware實作時僅需不斷重複「Compare」以及「Combine」即可。故MergeSort是非常有利於硬體實作的<br />
在Testbench中，我會依序產生5組unsorted的array，每一組array有8個元素，且範圍落在[1,255]。接著利用以下架構來實現硬體上的MergeSort。<br />
![MergeSort](https://user-images.githubusercontent.com/98183102/164136216-82827b2c-8bcc-4cfc-a034-60c920e145e8.jpg)<br />

### 2. Implementation details

- UNROLL:
根據上圖的架構，我的演算法中會有很多組迴圈，而內部包含了很多的Compare以及if-else branch。故我對每一個類似的迴圈進行"Completely ON"的UNROLL，試圖讓更多的運算同時進行。<br />
![1650388650562@2x](https://user-images.githubusercontent.com/98183102/164137580-c927e24e-2ddd-4d26-aefa-ca76cbbf02c7.jpg)<br />


- PIPELINE:
接著我在Input protocol以及body of sorting body的中間插入interval=1的PIPLINE指令，希望可以讓整個Sorting的過程利用pipeline的方式來提升latency。<br />
![1650388594873@2x](https://user-images.githubusercontent.com/98183102/164139418-9dec2e26-2fe5-40ed-9cd9-4c324b5f5e78.jpg)<br />

### 3. Experimental results
![mid](https://user-images.githubusercontent.com/98183102/164140129-8bd1ce1b-4461-470e-afb3-b6e6f2823b9a.jpg)<br />
&emsp;&emsp; 如果以BASIC版本當作base line的話，可以發現在UNROLL的時候，基於把所有迴圈內的指令都展開來，故平行運算的硬體元件理應增加。可以發現在latency的部分確實有改善，不過也因為硬體元件的增加而使面積上升。<br />
&emsp;&emsp; 在PIPELINE的部分，我們預期的是整個sorting的latency能夠大幅被改善，而就結果來看確實也達到目的（Latency 26 -> 8）。不過直得注意的是，照常來說當pipeline stage切得越多，同時運算的stage越多，而所需要的「不可共用」硬體元件就會越多，所以面積應該要大幅上升才對，不過這邊卻減少了快要4倍左右。

&emsp;&emsp; 而下圖是我將PIPELINE的效應模擬到TLM上的結果，由於PIPELINE會提升for迴圈內累加運算的throughput。故我在TLM read-socket前加了280 cycle的delay來模擬經過PIPELINE後的throughput，可以看到TLM模擬PIPELINE的Simulation time與上方HLS跑出來的Simulation time非常接近。<br />
![1650381267761@2x](https://user-images.githubusercontent.com/98183102/164046833-c5b598b4-715f-4e7a-b869-392359aca3f9.jpg)<br />
![1650381207132@2x](https://user-images.githubusercontent.com/98183102/164046850-f623e79d-ad69-499e-a022-05d769fc3215.jpg)<br />


### 4. Discussions and conclusions
&emsp;&emsp; 這次的作業讓我學會了使用HLS來進行電路的合成，由於HLS在Datapath上的便利性，可以直接使用各種Directive即可達到各種datapath處理的效果。不過由於內部隱含著很多最佳化的演算法，故最後合成出來的結果可能與我們想像的大相徑庭，或者是根本就很難去想像電路是如何被優化的。故在使用HLS時我認為必須要非常小心，要仔細的評估目前所下的各種Directive才是。


