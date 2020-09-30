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


void iterators() {
    MtmMatSq<int> m(2,0);
    m[1][0]=1;m[1][1]=2;

    int res_it[]={0,1,0,2};
    int res_nz_it[]={1,2};

    int i=0;
    for (MtmMatSq<int>::iterator it=m.begin();it!=m.end();++it) {
        myAssert (res_it[i]==(*it));
        ++i;
    }
    
    i=0;
    for (MtmMatSq<int>::nonzero_iterator it=m.nzbegin();it!=m.nzend();++it) {
        myAssert (res_nz_it[i]==(*it));
        ++i;
    }
    int newRes[]={0,1,2,3};
    i=0;
    for (MtmMatSq<int>::iterator it=m.begin();it!=m.end();++it) {
        *it=i;
        myAssert (newRes[i]==(*it));
        ++i;
    }


}

int main() {
    iterators(); //10 points
}

