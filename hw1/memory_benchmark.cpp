#include <iostream>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;

int ITERS = 10;
int n_from = 12;
int n_to = 28;

int main() {
  std::ofstream file;
  file.open("results.csv", std::ios_base::app);

  for (int N=n_from; N <= n_to; N++) {
    int size = pow(2,N) / sizeof(int);   // convert INT type to BYTE

    int *buffer = new int[size];

    default_random_engine gen(size);
    uniform_int_distribution<int> uniform_dist(0, size-1);

    int *rand_indices = new int[size];

    for (int i=0; i < size; i++) {
      rand_indices[i] = uniform_dist(gen);
    }

    double elapsed_time = INT_MAX;
    for (int i=0; i < ITERS; i++) {

      auto t1 = chrono::high_resolution_clock::now();
      volatile int x;
      for (int j=0; j < size; j++) {
        x = buffer[rand_indices[j]];   // reading a byte from buffer at random access
      }
      auto t2 = chrono::high_resolution_clock::now();
      auto t = (chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count()) / double(size);

      if (t < elapsed_time) elapsed_time = t;
    }
    file << size / 1024 << ", " << elapsed_time << std::endl;
    free(buffer);
    free(rand_indices);
  }
  file.close();
  // plot memory benchmarks with gnuplot
  system("gnuplot -p -e \"; set logscale x 2; set terminal qt; set title 'Benchmarking the memory hierarchy'; set xlabel 'Buffer size (kB)'; set ylabel 'Mean latency per memory read (ns)'; plot 'results.csv' w lp; \"");
  return 0;
}
