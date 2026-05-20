#pragma once
#include <iostream>
#include <thread>
#include <chrono>

class FileChunk {
public:
    int file_id;
    size_t chunk_id;
    size_t size;
    void download() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
};