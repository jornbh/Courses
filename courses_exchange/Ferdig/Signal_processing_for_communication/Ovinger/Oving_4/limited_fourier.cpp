#include<iostream>
#include <vector>
using std::vector;
struct complex{
    double re;
    double im;
    complex(double re, double im) : re(re), im(im) {}
    complex(): re(0), im(0) {}
};
complex operator*(const complex &lhs, const complex &rhs){
    complex output;
    output.re = lhs.re*rhs.re - lhs.im*rhs.im;
    output.im = lhs.re*rhs.im + lhs.im*rhs.re;
    return output;
}
void operator+=(complex &lhs, const complex &rhs){
    lhs.re+= rhs.re;
    lhs.im += rhs.im;
}
std::ostream& operator<<(std::ostream &os, const complex &rhs){
    os <<"{"<<rhs.re<<", "<<rhs.im<<"}";
    return os; 
}
const complex BASE_TURN(0.309,0.951);
const int TURNS_IN_PERIOD = 10;


template <class T>
std::ostream& operator<<(std::ostream &os, const std::vector<T> &rhs){
    os<< "[";
    for(int i =0; i<rhs.size(); ++i){
        os<< rhs[i] << ", ";
    }
    std::cout <<"]";
    return os;
}
vector<complex> transform(vector<complex> x){
    int N = x.size();
    vector<complex> bases(TURNS_IN_PERIOD);
    vector<complex> output(N);
    bases[0] = BASE_TURN;
    for(int i =1; i<bases.size(); ++i){
        bases[i] = BASE_TURN*bases[i-1];
    }
    for(int k =0; k<N; ++k){
        for(int n =0; n<N; ++k){
            int index = (k*n) % TURNS_IN_PERIOD;
            output[n] += bases[index]*x[k];
        }
    }
    return output;
}
// complex operator


int main(int vargs, char** kwargs){
    complex a(1,0), b(0,1), c;
    vector<complex> x(5);
    x[0] = complex(1,0);
    std::cout << x;
    vector<complex> y = transform(x);


    // a.re =1;
    // b.im =1;
    c = a*b*b;

    std::cout <<c<<"\n";



    return 0;
}