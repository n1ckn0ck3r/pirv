#include "DownloadManager.hpp"

int main() {
    DownloadManager manager(1, 2);

    manager.add_file(FileDownload(101, 3));
    manager.add_file(FileDownload(102, 2));

    std::thread t1(&DownloadManager::worker, &manager);
    std::thread t2(&DownloadManager::worker, &manager);
    std::thread t3(&DownloadManager::worker, &manager);

    t1.detach(), t2.detach(), t3.detach();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Main thread is adding another file to download." << std::endl;
    return 0;
}