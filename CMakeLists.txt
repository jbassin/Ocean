cmake_minimum_required(VERSION 3.10)
project(OceanLang)

set(CMAKE_CXX_STANDARD 11)

add_executable(OceanLang main.cpp oceanSyntacticTree.cpp oceanTransform.cpp oceanSyntacticTree.h oceanTransform.h)