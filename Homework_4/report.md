# ESL Homework_4 report / 107061272 陳力豪
> GitHub link : https://github.com/VincentChen1017/ESL/tree/main/Homework_4

### 1. Introduction of the Problem and my Solution
在這次的作業中我們將先前已經完成的GaussianBlur TLM model稍作修改後，將其嵌入RISC-V的Virtual Platform上面做整個系統的模擬。

### 2. Implementation details
將既有的GaussianBlur TLM model與risc-v的source code放置在一起後，建立屬於GaussianBlur的virtual plarform。<br />
並修改原先的testbench，使其能夠使用risc-vp內的DMA-function來傳接資料。<br />
要注意的是現在整個系統上有13個module，所以在bus的initiator端必須有13個ports才行。<br />

### 3. Experimental results
![圖片 1](https://user-images.githubusercontent.com/98183102/168950112-50819bd2-6cf5-402b-a416-bc94652b4907.jpg)<br />

### 4. Discussions and conclusions
由上述的結果可以明顯的發現，使用DMA來搬運資料可以大幅的減少整個模擬的時間以及CPU core所執行的instruction次數。<br />
這個結果確實符合預期，因為假設不使用DMA來「直接」獲得資料的話，當Accelator需要使用某筆資料時就需要跟CPU溝通，讓CPU針對內部的register file做出「load/store」的動作。<br />
而透過DMA讓Accelator直接跟memory溝通的話，則可以省下中間CPU處理繁瑣load/store的多餘程序。故使用DMA可以大幅的提升運算效率。



