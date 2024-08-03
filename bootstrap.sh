#!/bin/env bash

set -e

git clone https://github.com/microsoft/vcpkg.git
(cd vcpkg && ./bootstrap-vcpkg.sh)
cmake --preset debug
