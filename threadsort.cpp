#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <chrono>

void
thread_sort(std::vector<int>* v, size_t start, size_t finish) {
    qsort(v->data() + start, finish - start, sizeof((*v)[0]),
        [](const void* a, const void* b) {
            int ia = *((int*)a);
            int ib = *((int*)b);
            if (ia < ib) return -1;
            if (ia > ib) return 1;
            return 0;
        });
}
void
do_the_sort(std::vector<int>& a) {
    // This is what you need to modify. This is so slow...
    // It's single threaded, we can do better, right?

    std::thread sorter(thread_sort, &a, (size_t)0, a.size());
    sorter.join();

    // You should determine the number of cores available and
    // create a thread for each available core. Each thread
    // should be given a part of the array to work on.
    // C++, look at std::thread::hardware_concurrency()
    // C, it varies based on OS, but for Linux: sysconf(_SC_NPROCESSORS_ONLN)
    // and Windows: GetSystemInfo()

    // Your main thread will need to join the results together
    // to finish the sorting process. Note: you're welcome
    // to use std::sort() (C++) or the qsort() function (C) in
    // your thread. This isn't a data structures class.
}

bool
verifysorted(const std::vector<int>& a) {
    // Make sure the array really is sorted
    for (size_t i = 1; i < a.size(); i++) {
        if (a[i - 1] > a[i])
            return false;
    }
    return true;
}

int main()
{
    std::vector<int> nums;

    int x;
    while (std::cin >> x)
        nums.push_back(x);


    auto start = std::chrono::high_resolution_clock::now();

    do_the_sort(nums);

    auto finish = std::chrono::high_resolution_clock::now();

    if (!verifysorted(nums)) {
        std::cout << "Your array isn't sorted. Fast and wrong is wrong." << std::endl;
        return 1;
    }
 
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    return 0;
}
