#pragma once
#include "FileDownload.hpp"
#include <semaphore>
#include <mutex>
#include <queue>

class DownloadManager {
private:
    std::queue<FileChunk> queue;
    std::counting_semaphore<> active_downloads;
    std::counting_semaphore<> chunk_downloads;
    std::mutex mtx;
    std::atomic<int> completed_files{0};
    std::vector<FileDownload> tracked_files;

public:
    DownloadManager(int max_files, int max_chunks) : active_downloads(max_files), chunk_downloads(max_chunks) {}

    void add_file(FileDownload file) {
        std::lock_guard<std::mutex> lock(mtx);
        tracked_files.push_back(file);

        for (auto& chunk : tracked_files.back().chunks) {
            queue.push(chunk);
        }
    }

    void worker() {
        auto thread_id = std::this_thread::get_id();
        while (true) {
            FileChunk chunk;
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (queue.empty()) {
                    break;
                }
                chunk = queue.front();
                queue.pop();
            }

            active_downloads.acquire();
            chunk_downloads.acquire();

            process_chunk(chunk, thread_id);
        }
    }

    inline void process_chunk(FileChunk chunk, std::thread::id thread_id) {
        std::cout << "Thread " << thread_id << " is downloading chunk " << chunk.chunk_id << " of file " << chunk.file_id << std::endl;
        chunk.download();

        for (auto& f : tracked_files) {
            if (f.file_id == chunk.file_id) {
                f.mark_chunk_downloaded();
                if (f.is_complete()) {
                    completed_files.fetch_add(1);
                    std::cout << "File " << f.file_id << " download complete! Total completed: " << completed_files.load() << std::endl;
                }
                break;
            }
        }
    }

    int get_completed_files() const {
        return completed_files.load();
    }
};