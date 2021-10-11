#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <fstream>
#include <iterator>

using namespace std;

void minim(vector<int> const& v, int l, int r, int &start, int i, vector<int> &min) {
    int minimum = start;
    for (int i=l; i <= r; ++i) {
        if (v[i] < minimum) {
            minimum = v[i];
        }
    }
    min[i]=minimum;
}

int main() {
    int n, k;
    cout << "Enter number of threads:" << '\n';
    cin >> k;
    cout << "Enter number of elements" << '\n';
    cin >> n;
    vector<int> v(n);
    vector<int> min(k);
    for (int i=0; i<n; ++i) {
        cin >> v[i];
    }
    vector<thread> ths(k);
    int iterator=0;
    for (int i=0; i<k; ++i) {
        if (i==k-1) {
            ths[i] = thread(minim, ref(v), iterator, n-1, ref(v[iterator]), i, ref(min));
        } else {
            ths[i] = thread(minim, ref(v), iterator, iterator + n / k - 1, ref(v[iterator]), i, ref(min));
            iterator += n/k;
        }
    }
    for (int i=0; i<k; ++i) {
        ths[i].join();
    }
    /*for (int i : min) {
        cout << i << '\n';
    } */
    int res = min[0];
    for (int i=0; i<min.size(); ++i) {
        if (min[i] < res) {
            res = min[i];
        }
    }
    cout << res << '\n';
    return 0;
}
