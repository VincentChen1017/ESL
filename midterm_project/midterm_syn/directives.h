#ifndef _DIRECTIVES_H_
#define _DIRECTIVES_H_

// Define a group of macros for each configuration
#ifdef UNROLL
#   define LOOP_UNROLL_L1  HLS_UNROLL_LOOP( ON, "l1"  )
#   define LOOP_UNROLL_L2  HLS_UNROLL_LOOP( ON, "l2"  )
#   define LOOP_UNROLL_L3  HLS_UNROLL_LOOP( ON, "l3"  )
#   define LOOP_UNROLL_L4  HLS_UNROLL_LOOP( ON, "l4"  )
#   define LOOP_UNROLL_L5  HLS_UNROLL_LOOP( ON, "l5"  )
#   define LOOP_UNROLL_OUTPUT  HLS_UNROLL_LOOP( ON, "output"  )
#   define LOOP_PIPELINE
#   define LOOP_LATENCY
#endif

#ifdef PIPELINE
#   define LOOP_PIPELINE  HLS_PIPELINE_LOOP( SOFT_STALL, 1, "pipeline"  )   
#   define LOOP_UNROLL_L1  
#   define LOOP_UNROLL_L2  
#   define LOOP_UNROLL_L3  
#   define LOOP_UNROLL_L4  
#   define LOOP_UNROLL_L5  
#   define LOOP_UNROLL_OUTPUT  
#   define LOOP_LATENCY
#endif

#ifdef LATENCY
#   define LOOP_LATENCY  HLS_CONSTRAIN_LATENCY(0,2,"latency")
#   define LOOP_UNROLL_L1  
#   define LOOP_UNROLL_L2  
#   define LOOP_UNROLL_L3 
#   define LOOP_UNROLL_L4 
#   define LOOP_UNROLL_L5  
#   define LOOP_UNROLL_OUTPUT  
#   define LOOP_PIPELINE
#endif

#ifdef UNROLL_PIPE
#   define LOOP_UNROLL_L1  HLS_UNROLL_LOOP( ON , "l1"  )
#   define LOOP_UNROLL_L2  HLS_UNROLL_LOOP( ON , "l2"  )
#   define LOOP_UNROLL_L3  HLS_UNROLL_LOOP( ON , "l3"  )
#   define LOOP_UNROLL_L4  HLS_UNROLL_LOOP( ON , "l4"  )
#   define LOOP_UNROLL_L5  HLS_UNROLL_LOOP( ON , "l5"  )
#   define LOOP_UNROLL_OUTPUT  HLS_UNROLL_LOOP( ON , "output"  )
#   define LOOP_PIPELINE  HLS_PIPELINE_LOOP( SOFT_STALL, 1, "pipeline"  )
#   define LOOP_LATENCY
#endif

#ifdef LATENCY_PIPE
#   define LOOP_UNROLL_L1  
#   define LOOP_UNROLL_L2  
#   define LOOP_UNROLL_L3 
#   define LOOP_UNROLL_L4 
#   define LOOP_UNROLL_L5  
#   define LOOP_UNROLL_OUTPUT 
#   define LOOP_PIPELINE  HLS_PIPELINE_LOOP( SOFT_STALL, 1, "pipeline"  )
#   define LOOP_LATENCY HLS_CONSTRAIN_LATENCY(0,5,"latency")
#endif



#ifndef LOOP_UNROLL
#   define LOOP_UNROLL_L1  
#   define LOOP_UNROLL_L2  
#   define LOOP_UNROLL_L3  
#   define LOOP_UNROLL_L4  
#   define LOOP_UNROLL_L5  
#   define LOOP_UNROLL_OUTPUT  
#endif
#ifndef LOOP_PIPELINE
#   define LOOP_PIPELINE
#endif
#ifndef LOOP_LATENCY
#   define LOOP_LATENCY
#endif


#endif // _DIRECTIVES_H_
