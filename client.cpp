#include <iostream>
#include <thallium/serialization/stl/string.hpp>
#include <thallium.hpp>
#include <assert.h>
#include <fstream>
#include<unistd.h>
#include "mpi.h"
#include<iostream>
#include <cstdlib>
#include <random>
#include "server_provider.cpp"
#include <chrono>
using namespace std;
using namespace std::chrono;
namespace tl = thallium;

int read_addresses_from_file(std::vector<std::string>& server_addresses, int num_clients){
	for (int i=0; i<num_clients; ++i){
		std::string filename = std::string("server_addr") + std::to_string(i) ;
		std::string addr;
		std::ifstream infile(filename.c_str());
		if(!infile.is_open()) {
			std::cerr << "Couldn't open file" << std::endl;
			return -1;
		}
		infile >> addr;
		server_addresses.push_back(addr);
		infile.close();
	}
	return 1;
}

void print_array(std::vector<int> array){
	for (auto & ele: array){
		std::cout<<ele<<"\n";
		fflush(stdout);
	}
}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	int mpi_rank, mpi_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
	std::random_device rd; 
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> distr(0, mpi_size/2-1);
	
	std::vector<std::string> server_addresses;
	read_addresses_from_file(server_addresses, mpi_size);
	
	tl::engine myEngine("verbs;ofi_rxm", THALLIUM_CLIENT_MODE);
	tl::remote_procedure sum = myEngine.define("sum");
	tl::remote_procedure prod = myEngine.define("prod");
	int num_rpc_invocations = 100;
	std::vector<int> rpc_timings;
	std::vector<int> all_rpc_timings(num_rpc_invocations * mpi_size);

	std::vector<tl::provider_handle> provider_handles;
	for(int i=0; i<mpi_size; ++i)
		provider_handles.emplace_back(myEngine.lookup(server_addresses[i]), i);

	for(int i=0; i<num_rpc_invocations; ++i){
		//choose provider at random to invoke rpc on	
		int provider_id = distr(gen);
		auto time_start = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
		if (i%2) 
			prod.on(provider_handles[provider_id])(12,9);	
		else 
			sum.on(provider_handles[provider_id])(3,8);	
		auto time_end = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
		rpc_timings.push_back(time_end - time_start);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if (mpi_rank == 0){
		for(int i=0; i<mpi_size; ++i)
			myEngine.shutdown_remote_engine(provider_handles[i]);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	
	MPI_Gather(
		rpc_timings.data(),
		num_rpc_invocations,
		MPI_INT,
		all_rpc_timings.data(),
		num_rpc_invocations,
		MPI_INT,
		0,
		MPI_COMM_WORLD
	);	

	MPI_Barrier(MPI_COMM_WORLD);
	if (mpi_rank == 0)
		print_array(all_rpc_timings);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
        return 0;
}
