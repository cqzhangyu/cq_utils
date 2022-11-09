#!/bin/bash

scrp_path=$(cd `dirname $0`; cd ..; pwd)
. ${scrp_path}/utils.sh

################################################################

echo_back "${build_path}/CTest --size 123 --log_level INFO"
