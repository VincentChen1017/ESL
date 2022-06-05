# ESL Final_Project - Space Transform / 107061272 陳力豪
> GitHub link : https://github.com/VincentChen1017/ESL/tree/main/final_project

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
- BASIC: <br />
![1650425354924@2x](https://user-images.githubusercontent.com/98183102/164144765-03373cf4-ac09-4c5e-b84c-e0e0b994c1b1.jpg)<br />

- UNROLL:<br />
![UNROLL](https://user-images.githubusercontent.com/98183102/164144814-2998ccd3-db21-4937-b9d7-560a66a9ce37.jpg)<br />

- PIPELINE:<br />
![1650425383172@2x](https://user-images.githubusercontent.com/98183102/164144839-edcc9ff8-4259-48a0-96e7-389adaa2e99f.jpg)<br />

![mid](https://user-images.githubusercontent.com/98183102/164140129-8bd1ce1b-4461-470e-afb3-b6e6f2823b9a.jpg)<br />
&emsp;&emsp; 如果以BASIC版本當作base line的話，可以發現在UNROLL的時候，基於把所有迴圈內的指令都展開來，故平行運算的硬體元件理應增加。可以發現在latency的部分確實有改善，不過也因為硬體元件的增加而使面積上升。<br />
&emsp;&emsp; 在PIPELINE的部分，我們預期的是整個sorting的latency能夠大幅被改善，而就結果來看確實也達到目的（Latency 26 -> 8）。不過直得注意的是，照常來說當pipeline stage切得越多，同時運算的stage越多，而所需要的「不可共用」硬體元件就會越多，所以面積應該要大幅上升才對，不過這邊卻減少了快要4倍左右。<br /><br />
&emsp;&emsp; 在我深入分析BASIC以及PIPELINE兩種版本的合成結果之後，發現兩者合成出的電路都有特定一種MUX使用的量特別多(兩種版本各自使用最多的MUX為不同種MUX)且電路面積有很大一部分都來自此種MUX。而經過該種特殊MUX的使用量以及其面積比較後得到下方的結果，可以發現使PIPELINE面積大幅下降的原因就在於合成時演算法對於MUX的選擇以及使用。<br />
![mux](https://user-images.githubusercontent.com/98183102/164143876-f3849a48-6087-48f7-9f37-aee0c8683d70.jpg)

&emsp;&emsp; 而下圖是我將PIPELINE的效應模擬到TLM上的結果。由於PIPELINE會提升Sorting的Latency，故我在TLM read-socket前加了8 cycle的delay來模擬經過PIPELINE後的Simulation times。可以看到TLM模擬PIPELINE的結果，在Sorting完5個array後總共需要大約400ns。 <br />
![1650425021074@2x](https://user-images.githubusercontent.com/98183102/164144364-c8a69f89-1f5b-46e8-8b57-8303a5e4f532.jpg) <br />
![tlm](https://user-images.githubusercontent.com/98183102/164144391-aa1e6f9f-8947-487c-a432-0cd8fb3562eb.jpg)<br />


### 4. Discussions and conclusions
&emsp;&emsp; 這次的Project中，我學會了挑選特定的演算法並且進行驗證還有電路合成的方法。不過在高階合成的結果有時候會與預期的結果不太符合（例如：PIPELINE後面積反而變小），我想這與高階合成內部的演算法優化有關。我想之後應該要學習準確的「分析合成結果」才是，能更準確得掌握合成出來的電路才能實作出與預期相符且容易除錯的電路。


