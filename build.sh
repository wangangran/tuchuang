#!/bin/bash
set -x
root_dir=$(pwd)

# build yalantinglibs
build_yalantinglibs() {
  cd $root_dir/thirdPart/yalantinglibs
  rm build -rf
  mkdir build && cd build
  cmake ..
  cmake --build . --config debug -j8
  cmake --install . --prefix $root_dir
}

build_glog() {
  cd $root_dir/thirdPart/glog
  cmake -S . -B build -G "Unix Makefiles"
  cmake --build build
  cmake --build build --target install
}

build_yalantinglibs
build_glog
