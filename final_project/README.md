# Cross-compile Space transform to RISC-V VP platform

* First:    

  Remember to build the "acc-color_transfer" & "acc-color_transfer_single_core" platform of riscv-vp:  

  Add "subdirs(acc-color_transfer)" & "subdirs(acc-color_transfer_single_core)" to $EE6470/riscv-vp/vp/src/platform/CMakeLists.txt and run the following instructions:  

  $ cd $EE6470

  $ cd riscv-vp/vp/build  

  $ cmake ..  

  $ make install  

* Second:  

  Compile the software code by:  

  $ cd $EE6470

  $ cd riscv-vp/sw 

  $ cd color-transfer & cd color-transfer_single_core

  $ make  

  $ make sim
  
 # Run the HLS of the Multi cores and single core version

