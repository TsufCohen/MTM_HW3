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

void moreIteratorTests() {
    MtmMatSq<int> m1(2,0);
    MtmMatSq<int>::nonzero_iterator it1=m1.nzbegin();
    MtmMatSq<int>::nonzero_iterator it2=m1.nzend();
    myAssert(not(it1!=it2));
    MtmMatSq<int> m2(2,0);
    myAssert(m1.begin()!=m2.begin());
}

int main() {
    moreIteratorTests();
}

