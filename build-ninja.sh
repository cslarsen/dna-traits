#!/bin/bash
rm -rf build
mkdir build
cd build; cmake .. -DCMAKE_BUILD_TYPE=Release -G 'Ninja' && ninja
