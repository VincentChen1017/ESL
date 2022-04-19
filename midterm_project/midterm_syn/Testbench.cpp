#include <cstdio>
#include <cstdlib>
using namespace std;

#include "Testbench.h"
#include "def.h"

// use a queue to hold the time that each sample was sent
// for latency calculations
#include <queue>
static std::queue<sc_time> time_queue;
/////////////////////////////////////////

Testbench::Testbench(sc_module_name n): sc_module(n)
{
  SC_THREAD(feed_unsorted_array);
  sensitive << i_clk.pos();
  dont_initialize();
  SC_THREAD(fetch_sorted_array);
  sensitive << i_clk.pos();
  dont_initialize();
}

Testbench::~Testbench() {
	cout << "Total run time = " << total_run_time << endl;
}

void Testbench::feed_unsorted_array()
{

#ifndef NATIVE_SYSTEMC
	o_usort0.reset();
	o_usort1.reset();
	o_usort2.reset();
	o_usort3.reset();
	o_usort4.reset();
	o_usort5.reset();
	o_usort6.reset();
	o_usort7.reset();
#endif
	o_rst.write(false);
	wait(5);
	o_rst.write(true);
	wait(1);
	total_start_time = sc_time_stamp();

  int min = 1;
  int max = 255;
  srand(time(NULL));
  for (int n = 0; n < 5; n++) // sort 5 array
  {
    for (int i = 0; i < N; i++)
      unsorted_array[i] = rand() % (max - min + 1) + min;
    /*
        unsorted_array[0] = 25;
        unsorted_array[1] = 43;
        unsorted_array[2] = 27;
        unsorted_array[3] = 64;
        unsorted_array[4] = 79;
        unsorted_array[5] = 5;
        unsorted_array[6] = 13;
        unsorted_array[7] = 69;
    */

#ifndef NATIVE_SYSTEMC
					o_usort0.put(unsorted_array[0]);
					o_usort1.put(unsorted_array[1]);
					o_usort2.put(unsorted_array[2]);
					o_usort3.put(unsorted_array[3]);
					o_usort4.put(unsorted_array[4]);
					o_usort5.put(unsorted_array[5]);
					o_usort6.put(unsorted_array[6]);
					o_usort7.put(unsorted_array[7]);
#else
					o_usort0.write(unsorted_array[0]);
					o_usort1.write(unsorted_array[1]);
					o_usort2.write(unsorted_array[2]);
					o_usort3.write(unsorted_array[3]);
					o_usort4.write(unsorted_array[4]);
					o_usort5.write(unsorted_array[5]);
					o_usort6.write(unsorted_array[6]);
					o_usort7.write(unsorted_array[7]);
#endif
		wait();
    time_queue.push( sc_time_stamp() );
    cout<<"Time Start: "<< sc_time_stamp() <<endl; 
   //std::cout << "The unsorted data"<< " " << n << " :" << endl;
    //for (int i = 0; i < N; i++)
      //std::cout << unsorted_array[i] << " ";
    //std::cout << endl;
	}
}


void Testbench::fetch_sorted_array() {
  unsigned int x, y; // for loop counter
#ifndef NATIVE_SYSTEMC
	i_sort0.reset();
	i_sort1.reset();
	i_sort2.reset();
	i_sort3.reset();
	i_sort4.reset();
	i_sort5.reset();
	i_sort6.reset();
	i_sort7.reset();
#endif
	wait(5);
	wait(1);
	for(int i=0; i<5; i++)
	{
		sorted_array[0] = i_sort0.get();
		sorted_array[1] = i_sort1.get();
		sorted_array[2] = i_sort2.get();
		sorted_array[3] = i_sort3.get();
		sorted_array[4] = i_sort4.get();
		sorted_array[5] = i_sort5.get();
		sorted_array[6] = i_sort6.get();
		sorted_array[7] = i_sort7.get();

		sc_time sent_time( time_queue.front() );
    time_queue.pop();
    unsigned long latency = clock_cycle( sc_time_stamp() - sent_time );

		// show the sorted data
		std::cout << "The sorted data:"<< " " << i << " :" << endl;
		for (int i = 0; i < N; i++)
		  std::cout << sorted_array[i] << " ";
		std::cout << endl;
		std::cout<<"Latcncy is "<<latency<<endl;
                cout<<"Time End: "<< sc_time_stamp() <<endl;
		std::cout <<"\n"<< endl;
  
	}

	total_run_time = sc_time_stamp() - total_start_time;
  sc_stop();
}

int Testbench::clock_cycle( sc_time time )
{
    sc_clock * clk_p = DCAST < sc_clock * >( i_clk.get_interface() );
    sc_time clock_period = clk_p->period(); // get period from the sc_clock object.
    return ( int )( time / clock_period );
}



