cmake_minimum_required(VERSION 3.6)
project(stocks)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    main.cpp
        stock.h
    StockProcessor.cpp
    StockProcessor.h)

add_executable(stocks ${SOURCE_FILES})