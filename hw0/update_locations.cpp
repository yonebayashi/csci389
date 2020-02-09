#include <iostream>
#include <random>
#include <cstdlib>
#include <chrono>
#include <fstream>

int NUM_OF_RUNS=10;

using data_t = int64_t;

// Create a list of 'size' floating point numbers in the range [-bound, bound]
data_t* generate_random_list(const int size, const double bound) {
	data_t* arr = new data_t[size];

  	std::default_random_engine e1(1);	// 1 is the seed number

	std::uniform_real_distribution<data_t> uniform_dist(-bound, bound);
	for (int i=0; i<size; i++) {
		arr[i] = uniform_dist(e1);
	}

	return arr;
}

// Update position by velocity, one time-step
void update_coords(const int size, data_t* xs, data_t* ys, data_t* zs, const data_t* vx, const data_t* vy, const data_t* vz) {
	for (int i=0; i< size; i++) {
		xs[i] = xs[i] + vx[i];
        ys[i] = ys[i] + vy[i];
        zs[i] = zs[i] + vz[i];
	}     
}

data_t getSum(const int size, const data_t* arr) {
	data_t total = 0;
	for (int i=0; i< size; i++) {
		total += arr[i];
	}
	return total;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Required arguments: vector_length(N) and iterations_num(M)";
		return 0;
	} 

	int size = atoi(argv[1]);
	int iters = atoi(argv[2]);

	auto xs = generate_random_list(size, 1000.);
	auto ys = generate_random_list(size, 1000.);
	auto zs = generate_random_list(size, 1000.);
	auto vx = generate_random_list(size, 1.);
	auto vy = generate_random_list(size, 1.);
	auto vz = generate_random_list(size, 1.);

	std::ofstream file;
	file.open("plot.csv", std::ios_base::app);

	double total_t = 0;
	for (int i=0; i < NUM_OF_RUNS; i++) {
		auto t1 = std::chrono::high_resolution_clock::now();
		for (int i=0; i < iters; i++) {
			update_coords(size, xs, ys, zs, vx, vy, vz);
		}
		auto t2 = std::chrono::high_resolution_clock::now();
		auto t = (std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count()) / double(1000000);
		total_t += 1000000 * t / (size * iters);
	}
	double mean_t = total_t / NUM_OF_RUNS;

	data_t chksum = getSum(size, xs) + getSum(size, ys) + getSum(size, zs);

	std::cout << "Mean time per coordinate: " << mean_t << "us" << std::endl;
	std::cout << "Final checksum is: " << chksum << std::endl; 

	file << size << ", " << iters << ", " << mean_t << std::endl;
	file.close();

	delete[] xs;
	delete[] ys;
	delete[] zs;
	delete[] vx;
	delete[] vy;
	delete[] vz;
	return 0;
}