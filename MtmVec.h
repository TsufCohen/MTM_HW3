/**
 * created by Safi Azmi and Tsuf Cohen 22.01.2019
 */
#ifndef EX3_MTMVEC_H
#define EX3_MTMVEC_H

#include <iostream>
#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "Complex.h"

using std::exception;
using std::size_t;
using std::vector;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;

namespace MtmMath {
    template <typename T>
    class MtmVec {
        bool isRowVector;
        size_t length;
        Dimensions dimVec;
        vector<T> vec;
    public:
        size_t lock_start;
        size_t lock_end;

/*
 * Vector constructor, m is the number of elements in it and val is the initial value for the matrix elements
 */
        explicit MtmVec(size_t m, const T& val = T()) try :
                isRowVector(false),
                length(m),
                dimVec(length, 1),
                vec(length, val),
                lock_start(0),
                lock_end(length) {
            if(!m) {
                throw MtmExceptions::IllegalInitialization();
            }
        }catch(std::bad_alloc& e) {
            throw MtmExceptions::OutOfMemory();
        }catch(MtmExceptions::IllegalInitialization& e) {
            throw;
        }catch(MtmExceptions::OutOfMemory& e) {
            throw;
        }catch(std::exception& e) {
            throw MtmExceptions::MtmExceptions();
        }

        MtmVec() = default;
        MtmVec(const MtmVec& vecToCopy) = default;
        MtmVec& operator=(const MtmVec& v) = default;
        ~MtmVec() = default;

        size_t getLength() const {
            return length;
        }

        bool isRow() const {
            return isRowVector;
        }

        size_t getRowSize() const {
            return dimVec.getRow();
        }

        const size_t getcolSize() const {
            return dimVec.getCol();
        }

        MtmVec operator-() const;
        MtmVec& operator-=(const T& toSub);
        MtmVec& operator-=(const MtmVec& v);
        MtmVec& operator+=(const T& toAdd);
        MtmVec& operator+=(const MtmVec& v);
        MtmVec& operator*=(const T& toMul);

        Dimensions getDimensions() const {
            Dimensions dim(dimVec.getRow(), 1);
            return dim;
        }

        friend bool operator==(const MtmVec& a, const MtmVec& b) {
            if(a.isRaw==b.isRaw && a.length==b.length) {
                if(a.vec==b.vec) {
                    return true;
                }
            }
            return false;
        }

        friend bool operator!=(const MtmVec& a, const MtmVec& b) {
            return !(a==b);
        }

        T& operator[](size_t index);
        const T& operator[](size_t index) const;

        class iterator {
            MtmVec<T>* pointer;
            size_t index;

            friend class MtmVec<T>;

        public:
            iterator(MtmVec<T>* vec, size_t num) try :
                    pointer(vec), index(num) {

            }catch(std::bad_alloc& e) {
                throw MtmExceptions::OutOfMemory();
            }catch(std::exception& e) {
                throw MtmExceptions::MtmExceptions();
            }

            iterator(const iterator&) = default;
            ~iterator() = default;
            iterator& operator=(const iterator&) = default;
            T& operator*();
            virtual iterator& operator++();
            bool operator==(const iterator& it) const;
            bool operator!=(const iterator& it) const;
        };

        class nonzero_iterator : public iterator {
        public:
            nonzero_iterator(MtmVec<T>* vec, size_t index) try :
                    iterator(vec, index) {

            }catch(std::bad_alloc& e) {
                throw MtmExceptions::OutOfMemory();
            }catch(std::exception& e) {
                throw MtmExceptions::MtmExceptions();
            }

            ~nonzero_iterator() = default;
            nonzero_iterator(const nonzero_iterator&) = default;
            nonzero_iterator& operator++() override;
        };


        iterator begin();
        iterator end();
        nonzero_iterator nzbegin();
        nonzero_iterator nzend();


/*
 * Function that get function object f and uses it's () operator on each element in the vectors.
 * It outputs the function object's * operator after iterating on all the vector's elements
 */
        template <typename Func>
        T vecFunc(Func& f) const {
            Func newFunc = f;
            for(size_t i = 0 ; i<length ; ++i) {
                newFunc(vec[i]);
            }
            return *newFunc;
        }

        /*
         * Resizes a vector to dimension dim, new elements gets the value val.
         * Notice vector cannot transpose through this method.
         */
        void resize(Dimensions dim, const T& val = T()) {
            size_t rowSize = dim.getRow(), colSize = dim.getCol();
            if(isRow()==0 && colSize==1) {
                vec.resize(rowSize, val);
                length = rowSize;
                dimVec(rowSize, colSize);
                lock_end = length;
            }else if(isRow()==1 && rowSize==1) {
                vec.resize(colSize, val);
                length = colSize;
                lock_end = length;
                dimVec(rowSize, colSize);
            }else {
                throw MtmExceptions::ChangeMatFail(dimVec, dim);
            }
        }

        /*
         * Performs transpose operation on matrix
         */
        void transpose() {
            if(isRow()==0) {
                isRowVector = 1;
            }else {
                isRowVector = 0;
            }
            dimVec.transpose();
        }
    };
    /**------------------++++++++++++++++operators----------------------------**/
    /**+ between  vector & scalar **/
    template <typename T>
    MtmVec<T>& MtmVec<T>::operator+=(const T& toAdd) {
        for(size_t i = 0 ; i<vec.size() ; ++i) {
            vec[i] += toAdd;
        }
        return *this;
    }

    /**+ between 2 vector's**/
    template <typename T>
    MtmVec<T>& MtmVec<T>::operator+=(const MtmVec& v) {
        if(this->length!=v.length) {
            throw MtmExceptions::DimensionMismatch(dimVec, v.dimVec);
        }
        if(isRow()!=v.isRow()) {
            throw MtmExceptions::DimensionMismatch(dimVec, v.dimVec);
        }
        for(size_t i = 0 ; i<length ; ++i) {
            vec[i] += v.vec[i];
        }
        return *this;
    }

    template <typename T>
    MtmVec<T> operator+(const MtmVec<T>& a, const MtmVec<T>& b) {
        if(a.getLength()!=b.getLength()) {
            throw MtmExceptions::DimensionMismatch(
                    a.getDimensions(),
                    b.getDimensions());
        }
        if(a.isRow()!=b.isRow()) {
            throw MtmExceptions::DimensionMismatch(
                    a.getDimensions(),
                    b.getDimensions());
        }
        MtmVec<T> c = a;
        return c += b;
    }

    template <typename T>
    MtmVec<T> operator+(const T& b, const MtmVec<T>& a) {
        return MtmVec<T>(a) += b;
    }

    template <typename T>
    MtmVec<T> operator+(const MtmVec<T>& a, const T& b) {
        return MtmVec<T>(a) += b;
    }

/**----------------------(-----)operator-------------------------------------**/
    template <typename T>
    MtmVec<T> MtmVec<T>::operator-() const {
        MtmVec<T> newVec(*this);
        for(size_t i = 0 ; i<newVec.length ; ++i) {
            newVec.vec[i] = -(newVec.vec[i]);
        }
        return newVec;
    }

    template <typename T>
    MtmVec<T>& MtmVec<T>::operator-=(const T& toSub) {
        return *this += (-toSub);
    }

    template <typename T>
    MtmVec<T>& MtmVec<T>::operator-=(const MtmVec<T>& v) {
        return *this += (-v);
    }

    template <typename T>
    MtmVec<T> operator-(const T& b, const MtmVec<T>& a) {
        return MtmVec<T>(-a) += b;
    }

    template <typename T>
    MtmVec<T> operator-(const MtmVec<T>& a, const T& b) {
        return MtmVec<T>(a) -= b;
    }

    template <typename T>
    MtmVec<T> operator-(const MtmVec<T>& a, const MtmVec<T>& b) {
        return MtmVec<T>(a) -= b;
    }

    /**----------------------------**********operators-----------------------**/
    template <typename T>
    MtmVec<T>& MtmVec<T>::operator*=(const T& toMul) {
        for(size_t i = 0 ; i<length ; ++i) {
            vec[i] = vec[i]*(toMul);
        }
        return *this;
    }

    template <typename T>
    MtmVec<T> operator*(const T& b, const MtmVec<T>& a) {
        return MtmVec<T>(a) *= b;
    }

    template <typename T>
    MtmVec<T> operator*(const MtmVec<T>& a, const T& b) {
        return MtmVec<T>(a) *= b;
    }
    /**----------------------------------------------------------------------**/


    /**-----------------------operator[]][][][][][]--------------------------**/
    template <typename T>
    T& MtmVec<T>::operator[](size_t index) {
        if(index>=length) {
            throw MtmExceptions::AccessIllegalElement();
        }
        if(lock_start<=index && index<=lock_end) {
            return vec[index];
        }
        throw MtmExceptions::AccessIllegalElement();
    }

    template <typename T>
    const T& MtmVec<T>::operator[](size_t index) const {
        if(index>=length) {
            throw MtmExceptions::IllegalInitialization();
        }
        return vec[index];
    }
    /**----------------------------------------------------------------------**/


    /**--------------------------iterator------------------------------------**/

    template <typename T>
    T& MtmVec<T>::iterator::operator*() {
        if(index>=pointer->length) {
            throw MtmExceptions::AccessIllegalElement();
        }
        return pointer->vec.at(index);
    }

    template <typename T>
    typename MtmVec<T>::iterator& MtmVec<T>::iterator::operator++() {
        ++index;
        return *this;
    }

    template <typename T>
    bool MtmVec<T>::iterator::operator==(const iterator& it) const {
        if(pointer!=it.pointer) {
            return false;
        }
        return index==it.index;
    }

    template <typename T>
    bool MtmVec<T>::iterator::operator!=(const iterator& it) const {
        if(pointer!=it.pointer) {
            return true;
        }
        return index!=it.index;
    }

    template <typename T>
    typename MtmVec<T>::iterator MtmVec<T>::begin() {
        return iterator(this, 0);
    }

    template <typename T>
    typename MtmVec<T>::iterator MtmVec<T>::end() {
        return iterator(this, length);
    }

    template <typename T>
    typename MtmVec<T>::nonzero_iterator&
    MtmVec<T>::nonzero_iterator::operator++() {
        if(*this==(this->pointer->nzend())) {
            return *this;
        }

        ++(this->index);

        if(*this==(this->pointer->nzend())) {
            return *this;
        }

        if((this->pointer->lock_start)<=(this->index) &&
            (this->index)<=(this->pointer->lock_end)) {
            if(**this==T()) {
                return operator++();
            }
            return *this;
        }
        return operator++();
    }

    template <typename T>
    typename MtmVec<T>::nonzero_iterator MtmVec<T>::nzbegin() {

        nonzero_iterator a(this, 0);
        if(a.pointer->vec[a.index]==T()) {
            return ++a;
        }
        return a;
    }

    template <typename T>
    typename MtmVec<T>::nonzero_iterator MtmVec<T>::nzend() {
        return nonzero_iterator(this, length);
    }

}
#endif //EX3_MTMVEC_H

