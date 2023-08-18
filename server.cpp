#include <iostream>
#include <thallium/serialization/stl/string.hpp>
#include <thallium.hpp>
#include <assert.h>
#include <fstream>
#include<unistd.h>
#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include "server_provider.cpp"
#include <chrono>
using namespace std;
using namespace std::chrono;
namespace tl = thallium;

int write_addresses_to_file(std::string& addr, std::string& filename){
	std::ofstream outfile(filename.c_str());
	if(!outfile.is_open()) {
		std::cerr << "Couldn't open file" << std::endl;
		return -1;
	}
	outfile << addr << std::endl;
	outfile.close();
	return 1;
}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	int mpi_rank, mpi_size;
	int num_es=4;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
	//tl::engine engine("ofi+verbs", THALLIUM_SERVER_MODE, true, num_es);
	//TODO: uncomment to run baseline
	tl::engine engine("ofi+verbs", THALLIUM_SERVER_MODE);
	engine.enable_remote_shutdown();
	std::string addr = engine.self();
	std::string filename = std::string("server_addr") + std::to_string(mpi_rank) ;
	write_addresses_to_file(addr, filename);
	my_sum_provider* myProvider = my_sum_provider::create(engine, mpi_rank);
	engine.wait_for_finalize();
	MPI_Barrier(MPI_COMM_WORLD);
        MPI_Finalize();
	return 0;
}
