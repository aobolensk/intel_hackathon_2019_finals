#include <CL/sycl.hpp>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>
#include <float.h>

using namespace cl::sycl;

cpu_selector cpu_sel;
// gpu_selector gpu_sel;
queue cpu_queue = queue(cpu_sel);
// queue gpu_queue = queue(gpu_sel);

float sqr(float x) {
    return x * x;
}

int main() {
    std::ifstream data("data.txt");
    int n;
    data >> n;
    std::vector <float> v(2 * n);
    for (int i = 0; i < 2 * n; ++i) {
        data >> v[i];
    }
    // std::cout << std::endl;
    buffer <float, 1> a(v.data(), range<1>(2 * n));
    float m_res = 1000000;
    std::vector <int> result(n + 1);
    buffer <int, 1> res(result.data(), range<1>(n));
    buffer <float, 1> min_res(&m_res, range<1>(1));
    // for (int forik = 0; forik < 10; ++forik) {
    while(true) {
        cpu_queue.submit([&](handler &cgh) {
            // stream out(1000000, 1000000, cgh);
            stream out(1024, 80, cgh);
            // stream out2(100000, 100000, cgh);
            srand(time(0));
            auto A = a.get_access<access::mode::read_write>(cgh);
            auto RES = res.get_access<access::mode::read_write>(cgh);
            auto MIN_RES = min_res.get_access<access::mode::read_write>(cgh);
            // cgh.parallel_for<class For2>(range<1>(120), [=](id<1> index) {
                // out << index << A[index] << endl;//  << " ";
                // out << index;
            // });
            // out << "lul";
            // static std::mt19937 rng(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
            cgh.parallel_for<class For1>(range<1>(24), [=](id<1> index) {
                // out << index;
                // buffer <int, 1> p(range<1>(n));
                // auto P = p.get_access<access::mode::read_write>(cgh);
                // int *p = new int[n];
                // for (int i = 0; i < n; ++i) {
                    // p[i] = i;
                    // out << p[i] << " ";
                // }
                // out << endl;
                int p[60];
                for (int i = 0; i < 60; ++i) {
                    p[i] = i;
                }
                for (int i = 60 - 1; i >= 2; --i) {
                    // std::uniform_int_distribution<int> dist(1, i);
                    // int j = dist(rng);
                    int j = rand() % (i - 1) + 2;
                    std::swap(p[i], p[j]);
                }
                std::swap(p[1], p[60 - 1]);
                float dist = 0.;
                for (int i = 1; i < 60; ++i) {
                    dist += sqr(A[2 * p[i]] - A[2 * p[i-1]]) + sqr(A[2 * p[i] + 1] - A[2 * p[i-1] + 1]);
                }
                if (dist < MIN_RES[0]) {
                    for (int i = 0; i < n; ++i) {
                        RES[i] = p[i];
                    }
                    // RES[i] = dist;
                    MIN_RES[0] = dist;
                }
                // out << "p: ";
                // for (int i = 0; i < 60; ++i) {
                    // out << p[i] << " ";
                // }
                // out << 5 << endl;
                // out << endl; 
            });

        });
        cpu_queue.wait();
        // std::cout << m_res << ": ";
        for (int i = 0; i < n; ++i) {
            std::cout << result[i] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
