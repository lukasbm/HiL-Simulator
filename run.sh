#!/bin/bash
export LD_LIBRARY_PATH="$(realpath ../../../lib)::$(realpath ../../../workspace/hil_performance_model/AD_HiL_Ethernet/src):$(realpath ../../../workspace/hil_performance_model/inet4/src):"
export OMNETPP_IMAGE_PATH="$(realpath ../../../images)"

cd simulations/Testproject_for_scheduling
../../../../../bin/opp_run_dbg \
    -m -u Qtenv -c General \
    -n ..:../../src:../../../inet4/src:../../../inet4/examples:../../../inet4/tutorials:../../../inet4/showcases \
    --image-path=../../../inet4/images -l ../../src/AD_HiL_Ethernet \
    -l ../../../inet4/src/INET one_easy_stream.ini
