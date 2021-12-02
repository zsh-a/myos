#include<bits/stdc++.h>
using namespace std;


int main(){

    struct A{
        uint32_t x : 8;
    }__attribute__((packed));
    uint32_t z;
    cin >> z;
    A a{.x = z};
    auto x = a.x;
    cout << x;
}