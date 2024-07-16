#!/bin/bash
export LD_LIBRARY_PATH=../sdk/lib:$LD_LIBRARY_PATH
sudo chmod u+x ./nerecord_demo
./nerecord_demo \
    --appkey "6acf024e190215b685905444b6e57dd7" \
    --cname "5566" \
    --uid 2233 \
    --runtime 1
