#include <iostream>

#include "MtmVec.h"
#include "MtmMat.h"
#include "MtmMatSq.h"
#include "MtmMatTriag.h"
#include "Complex.h"


using namespace MtmMath;
using std::cout;
using std::endl;

void myAssert(bool flag) {
    if (not(flag)) {
        cout << "fail" <<endl;
    }
}

void FuncExample() {
    class sumFunc {
        int sum;
    public:
        sumFunc() : sum(0) {}
        void operator()(int x) {
            sum += x;
        }
        int operator*() { return  sum;}
    };

    MtmVec<int> v(5,0);
    v[1]=3;v[2]=-7;v[3]=-1;v[4]=2;
    MtmMat<int> m(Dimensions(2,3),0);
    m[0][0]=1;m[1][0]=2;
    m[0][1]=3;m[1][1]=2;
    m[0][2]=5;m[1][2]=-6;
    sumFunc f;
    myAssert (v.vecFunc(f) == -3);
    MtmVec<int> res(m.matFunc(f));
    myAssert(res[0]==3 and res[1]==5 and res[2]==-1);
}


int main() {

    FuncExample(); //10 points

}

