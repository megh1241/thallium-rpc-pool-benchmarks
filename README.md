# thallium-rpc-pool-benchmarks

## Organization
client.cpp: Runs client processes that choose a provider id at random and invokes rpc on it.
server.cpp: Run providers
server_provider.cpp: Provider class implementation

## Building
spack env activate .
spack install
cmake .
make

## Running
To run on polaris: qsub ./run_job.qsub
