cmake_minimum_required(VERSION 3.0)
add_library(RacheFile STATIC
        FileDocument.h
        FileDocument.cpp
        SubFile.h
        SubFile.cpp
        FileItemGroup.h
        FileItemGroup.cpp
        FileItemObject.h
        FileItemObject.cpp
        DocumentSearcher.h
        DocumentSearcher.cpp
        ResourceSet.h
        ResourceSet.cpp
        RByteSet/RBytes.h
        RByteSet/RBytes.cpp
        RByteSet/RByteSet.h
        RByteSet/RByteSet.cpp
        RByteSet/RByteSetFile.h
        RByteSet/RByteSetFile.cpp
        )