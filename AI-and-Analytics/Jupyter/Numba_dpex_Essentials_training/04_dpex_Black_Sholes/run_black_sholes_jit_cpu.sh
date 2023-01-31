#!/bin/bash
source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1
/bin/echo "##" $(whoami) is compiling AI numba-dpex essentials Module4 --  Black scholes - 1 of 4 black_sholes_jit_cpu.py
python lab/black_sholes_jit_cpu.py --steps 5 --size 1024 --repeat 5 --json result_gpu.json
