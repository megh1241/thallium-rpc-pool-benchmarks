# thallium-rpc-pool-benchmarks

## Organization
client.cpp: Runs client processes that choose a provider id at random and invokes rpc on it.<br>
server.cpp: Run providers.<br>
server_provider.cpp: Provider class implementation.<br>

## Building
spack env activate .<br>
spack install<br>
cmake .<br>
make<br>

## Running
To run on polaris: qsub ./run_job.qsub
