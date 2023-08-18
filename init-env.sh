#!/bin/bash

# Necessary for Bash shells
. /etc/profile

#export HDF5_USE_FILE_LOCKING=FALSE
libfabric_alcf() {
  # use shared recv context in RXM; should improve scalability
  export FI_OFI_RXM_USE_SRX=1
  # note: disable registration cache for verbs provider for now; see
  #       discussion in https://github.com/ofiwg/libfabric/issues/5244
  export FI_MR_CACHE_MAX_COUNT=0
}
source ~/.bashrc
module swap PrgEnv-nvhpc PrgEnv-gnu
source ${HOME}/git/spack-polaris/share/spack/setup-env.sh
export CRAYPE_LINK_TYPE=dynamic
export SPACK_USER_CONFIG_PATH="$HOME/.spack/$clustername"
export SPACK_USER_CACHE_PATH="$SPACK_USER_CONFIG_PATH"
spack env activate .
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/soft/libraries/cudnn/cudnn-11.4-linux-x64-v8.2.4.15/lib64"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/soft/compilers/cudatoolkit/cuda-11.2.2/lib64"
export LD_LIBRARY_PATH="$(pwd)/venv/lib/python3.8/site-packages/nvidia/cuda_runtime/lib:$LD_LIBRARY_PATH"

export http_proxy="http://proxy-01.pub.alcf.anl.gov:3128"
export https_proxy="http://proxy-01.pub.alcf.anl.gov:3128"
export ftp_proxy="http://proxy-01.pub.alcf.anl.gov:3128"
libfabric_alcf
