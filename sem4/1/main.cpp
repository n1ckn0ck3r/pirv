#include "ParallelSum.h"

int main() {
    ParallelSum<int> ps1({1, 2, 3, 1, 2, 3, 4, 5, 6, 1, 2, 3}, 4);
    ParallelSum<float> ps2({1, 2, 3, 1, 2, 3, 4, 5, 6, 1, 2, 3}, 4);
    ParallelSum<double> ps3({1, 2, 3, 1, 2, 3, 4, 5, 6, 1, 2, 3}, 4);

    int result1 = ps1.compute_sum();
    std::cout << result1 << std::endl;
    float result2 = ps2.compute_sum();
    std::cout << result2 << std::endl;
    double result3 = ps3.compute_sum();
    std::cout << result3 << std::endl;

    

    return 0;
}