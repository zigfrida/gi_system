cmake_minimum_required(VERSION 3.23)
project(gi_system)

set(CMAKE_CXX_STANDARD 17)

add_executable(gi_system GIS.cpp CommandProcessor.cpp CommandProcessor.h World.cpp World.h GISRecord.h HashTable.cpp HashTable.h BufferPool.h BufferPool.cpp GISRecord.cpp Logger.h Logger.cpp PRQuadtree.h PRQuadtree.cpp CoordinateIndex.h CoordinateIndex.cpp NameIndex.h NameIndex.cpp PRQuadtreeNode.h)
