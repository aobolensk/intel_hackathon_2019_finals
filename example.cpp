#include <CL/sycl.hpp>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>
#include <float.h>

using namespace cl::sycl;

double sqr(double a) {
    return a*a;
}

int was[60];

int main() {
    int n;
    std::cin>>n;
    std::vector<float>v(2 * n);
    for(int i = 0; i < n; i++) {
        std::cin>>v[i*2]>>v[i*2+1];
    }
    std::mt19937 rng(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    std::uniform_int_distribution<int>dist[60];
    for(int i = 1; i < 60; i++) {
        dist[i] = std::uniform_int_distribution<int>(2, i);
    }
    
    double very_global_dist = 340257274234.;
    while(true) {
    // for(int forik = 0; forik < 10; forik++) {
        // std::cout << "+" << std::endl;
        for (int i = 0; i < 60; ++i) was[i] = 0;
        std::vector<int>p(60);
        for(int i = 0; i < 60; i++) {
            p[i] = i;
        }
        // for (int i = 60 - 1; i >= 2; --i) {
        //     int j = dist[i](rng);
        //     std::swap(p[i], p[j]);
        // }
        int teq = p[2], next = -1;
        // std::cout << p[0] << " " << p[1] << " " << p[2] << std::endl;
        std::vector<int>tmppath;
        tmppath.push_back(teq);
        double global_dist = 0;
        for (int j = 0; j < 57; ++j) {
            double dist = 6000;
            was[teq] =  1;
            next = -1;
            for(int i = 2; i < 60; i++) {
                if(was[i])
                    continue;
                double tmp = sqrt(sqr(v[2*teq]-v[2*i]) + sqr(v[2*teq+1]-v[2*i+1]));
                if(dist > tmp) {
                    next = i;
                    dist = tmp;
                }
                // std::cout << dist << " " << tmp << std::endl;
            }
            if (next == -1) assert(0);
            tmppath.push_back(next);
            global_dist+=dist;
            teq = next;
        }
        if (global_dist < very_global_dist) {
            for(int i = 0; i < 58; i++) {
                std::cout<<tmppath[i]<<",";
            }
            std::cout << std::endl;
            very_global_dist = global_dist;
        }
    }
    return 0;
}
