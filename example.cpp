#include <CL/sycl.hpp>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <float.h>

using namespace cl::sycl;

cpu_selector cpu_sel;
gpu_selector gpu_sel;
queue cpu_queue = queue(cpu_sel);
queue gpu_queue = queue(gpu_sel);

float sqr(float x) {
    return x * x;
}

#define RRRRRRRRAND_MAX 32767  

int main() {
    static unsigned long int next = 1;
    auto rand = [&](void) {
        next = next * 1103515245 + 12345;
        return (unsigned int)(next/65536) % (RRRRRRRRAND_MAX + 1);
    };
    auto srand = [&](unsigned int seed) {
        next = seed;
    };
    int n;
    std::cin >> n;
    std::vector <float> v(2 * n);
    for (int i = 0; i < 2 * n; ++i) {
        std::cin >> v[i];
    }
    // std::cout << std::endl;
    buffer <float, 1> a(v.data(), range<1>(2 * n));
    float m_res = 1000000;
    std::vector <int> result(n);
    buffer <int, 1> res(result.data(), range<1>(n));
    buffer <float, 1> min_res(&m_res, range<1>(1));
    // for (int forik = 0; forik < 10; ++forik) {
    while(true) {
        gpu_queue.submit([&](handler &cgh) {
            auto A = a.get_access<access::mode::read>(cgh);
            auto RES = res.get_access<access::mode::write>(cgh);
            auto MIN_RES = min_res.get_access<access::mode::read_write>(cgh);
            cgh.parallel_for<class For1>(range<1>(24), [=](id<1> index) {
                int p[60];
                for (int i = 0; i < 60; ++i) {
                    p[i] = i;
                }
                for (int i = 60 - 1; i >= 2; --i) {
                    int j = rand() % (i - 1) + 2;
                    std::swap(p[i], p[j]);
                }
                std::swap(p[1], p[60 - 1]);
                float dist = 0.;
                for (int i = 1; i < 60; ++i) {
                    dist += sqrt(sqr(A[2 * p[i]] - A[2 * p[i-1]]) + sqr(A[2 * p[i] + 1] - A[2 * p[i-1] + 1]));
                }
                if (dist < MIN_RES[0]) {
                    for (int i = 0; i < n; ++i) {
                        RES[i] = p[i];
                    }
                    MIN_RES[0] = dist;
                }
            });
        });
        cpu_queue.submit([&](handler &cgh) {
            auto A = a.get_access<access::mode::read>(cgh);
            auto RES = res.get_access<access::mode::write>(cgh);
            auto MIN_RES = min_res.get_access<access::mode::read_write>(cgh);
            cgh.parallel_for<class For2>(range<1>(24), [=](id<1> index) {
                int p[60];
                for (int i = 0; i < 60; ++i) {
                    p[i] = i;
                }
                for (int i = 60 - 1; i >= 2; --i) {
                    int j = rand() % (i - 1) + 2;
                    std::swap(p[i], p[j]);
                }
                std::swap(p[1], p[60 - 1]);
                float dist = 0.;
                for (int i = 1; i < 60; ++i) {
                    dist += sqrt(sqr(A[2 * p[i]] - A[2 * p[i-1]]) + sqr(A[2 * p[i] + 1] - A[2 * p[i-1] + 1]));
                }
                if (dist < MIN_RES[0]) {
                    for (int i = 0; i < n; ++i) {
                        RES[i] = p[i];
                    }
                    MIN_RES[0] = dist;
                }
            });
        });
        cpu_queue.wait();
        gpu_queue.wait();
        auto acc = res.get_access<access::mode::read>();
        for (int i = 1; i < n - 1; ++i) {
            std::cout << acc[i] << ",";
        }
        std::cout << std::endl;
    }
    return 0;
}