#include <iostream>
#include <vector>
#include <chrono>
#include <sycl/sycl.hpp>

void vectorAddCPU(const std::vector<float>& A, const std::vector<float>& B, std::vector<float>& C) {
	for (int i = 0; i < A.size(); ++i) {
		C[i] = A[i] + B[i];
	}
}

int main() {
    try {
        sycl::queue q( sycl::gpu_selector_v );
        std::cout << "Выполняется на " << q.get_device().get_info<sycl::info::device::name>() << std::endl;

        const int N = 1e6;
        std::vector<float> A(N), B(N), C_cpu(N), C_gpu(N);

        for (int i = 0; i < N; ++i) {
            A[i] = static_cast<float>(i);
            B[i] = static_cast<float>(2 * i);
        }

        auto start_cpu = std::chrono::high_resolution_clock::now();
        vectorAddCPU(A, B, C_cpu);
        auto end_cpu = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration_cpu = end_cpu - start_cpu;

        auto start_gpu = std::chrono::high_resolution_clock::now();
        {
            sycl::buffer bufA(A.data(), sycl::range<1>(N));
            sycl::buffer bufB(B.data(), sycl::range<1>(N));
            sycl::buffer bufC(C_gpu.data(), sycl::range<1>(N));

            q.submit([&](sycl::handler& h) {
                auto accA = bufA.get_access<sycl::access::mode::read>(h);
                auto accB = bufB.get_access<sycl::access::mode::read>(h);
                auto accC = bufC.get_access<sycl::access::mode::write>(h);

                h.parallel_for(sycl::range<1>(N), [=](sycl::id<1> i) {
                    accC[i] = accA[i] + accB[i];
                    });
                });
            q.wait();
        }

        auto end_gpu = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration_gpu = end_gpu - start_gpu;

        bool c = true;
        for (int i = 0; i < N; ++i) {
            if (std::abs(C_cpu[i] - C_gpu[i]) > 1e-5) {
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