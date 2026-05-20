#pragma once
#include "FileChunk.hpp"
#include <vector>
#include <atomic>

class FileDownload {
public:
    int file_id;
    std::vector<FileChunk> chunks;
    std::atomic<int> downloaded_chunks{0};
    int total_chunks;

    FileDownload(int id, int num_chunks) : file_id(id), total_chunks(num_chunks) {
        for (int i = 0; i < num_chunks; ++i) {
            chunks.push_back({id, static_cast<size_t>(i), 1024});
        }
    }

    bool is_complete() {
        return downloaded_chunks.load() == total_chunks;
    }

    void mark_chunk_downloaded() {
        downloaded_chunks.fetch_add(1);
    }
};