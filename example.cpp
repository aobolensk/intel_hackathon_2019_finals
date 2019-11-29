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
    double very_global_dist = 340257274234.;
    std::vector<int>p(60);
    for(int i = 0; i < 60; i++) {
        p[i] = i;
    }
    for (int hh = 2; hh < 60; ++hh) {
        for (int i = 0; i < 60; ++i) was[i] = 0;
        std::swap(p[2], p[hh]);
        int teq = p[2], next = -1;
        std::vector<int>tmppath;
        tmppath.push_back(teq);
        double global_dist = sqrt(sqr(v[2*p[0]]-v[2*p[2]]) + sqr(v[2*p[0]+1]-v[2*p[2]+1]));
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
        global_dist += sqrt(sqr(v[2*p[1]]-v[2*p[58]]) + sqr(v[2*p[1]+1]-v[2*p[58]+1]));
        if (global_dist < very_global_dist) {
            for(int i = 0; i < 58; i++) {
                std::cout<<tmppath[i]<<",";
            }
            std::cout << std::endl;
            very_global_dist = global_dist;
        }
        // std::cout << "gg: " << very_global_dist << std::endl;
        std::swap(p[2], p[hh]);
    }
    return 0;
}
