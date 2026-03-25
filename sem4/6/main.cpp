#include "MatrixProcessor.h"

int main() {
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3, 5},
        {2, 4, 6, 10},
        {3, 6, 9, 15},
        {4, 8, 12, 20}
    };

    MatrixProcessor<int> proc(matrix, 4);

    proc.print();

    proc.apply([](int x) { return x*x; });

    proc.print();

    return 0;
}