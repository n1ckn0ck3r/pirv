#include "HyperThreadingSimulator.h"

int main() {
    std::vector<std::string> queue = {"A", "B", "C", "D"};
    HyperThreadingSimulator hts(queue);

    hts.execute();

    return 0;
}