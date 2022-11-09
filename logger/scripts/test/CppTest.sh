#!/bin/bash

scrp_path=$(cd `dirname $0`; cd ..; pwd)
. ${scrp_path}/utils.sh

################################################################

echo_back "${build_path}/CppTest --log_fname ${proj_path}/log/temp/CppTest.txt"
