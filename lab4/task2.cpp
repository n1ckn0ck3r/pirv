#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <sycl/sycl.hpp>

void increaseBrightnessCPU(const std::vector<unsigned char>& input, std::vector<unsigned char>& output, int width, int height, unsigned char delta) {
	for (int i = 0; i < height * width; ++i) {
		int val = input[i] + delta;
		output[i] = (val > 255) ? 255 : static_cast<unsigned char>(val);
	}
}

int main() {
    try {
        sycl::queue q( sycl::gpu_selector_v );
        std::cout << "Выполняется на " << q.get_device().get_info<sycl::info::device::name>() << std::endl;

        const int width = 1024;
        const int height = 1024;
        const size_t N = width * height;
        const unsigned char delta = 25;

        std::vector<unsigned char> In(N), Out_cpu(N), Out_gpu(N);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 255);
        for (size_t i = 0; i < N; ++i) {
            In[i] = static_cast<unsigned char>(dist(gen));
        }

        auto start_cpu = std::chrono::high_resolution_clock::now();
        increaseBrightnessCPU(In, Out_cpu, width, height, delta);
        auto end_cpu = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration_cpu = end_cpu - start_cpu;

        auto start_gpu = std::chrono::high_resolution_clock::now();
        {
            sycl::buffer buffIn(In.data(), sycl::range<1>(N));
            sycl::buffer buffOut(Out_gpu.data(), sycl::range<1>(N));

            q.submit([&](sycl::handler& h) {
                auto accIn = buffIn.get_access<sycl::access::mode::read>(h);
                auto accOut = buffOut.get_access<sycl::access::mode::write>(h);

                sycl::range<2> global_range(height, width);

                h.parallel_for(global_range, [=](sycl::item<2> item) {
                    size_t row = item[0];
                    size_t col = item[1];

                    size_t i = row * width + col;
                    int val = accIn[i] + delta;
                    accOut[i] = (val > 255) ? 255 : static_cast<unsigned char>(val);
                    });
                });
            q.wait();
        }
        auto end_gpu = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration_gpu = end_gpu - start_gpu;

        bool c = true;
        for (size_t i = 0; i < N; ++i) {
            if (Out_cpu[i] != Out_gpu[i]) {
                c = false;
                break;
            }
        }

        std::cout << "Результат верификации: " << (c ? "УСПЕШНО" : "ОШИБКА") << std::endl;
        std::cout << "Затраченное на CPU время: " << duration_cpu.count() << " мс" << std::endl;
        std::cout << "Затраченное на GPU время: " << duration_gpu.count() << " мс" << std::endl;
        std::cout << "Ускорение: " << duration_cpu.count() / duration_gpu.count() << "x" << std::endl;
    } catch (const sycl::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}