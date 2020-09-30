/**
 * created by Safi Azmi and Tsuf Cohen 22.01.2019
 */
#ifndef EX3_MTMMAT_H
#define EX3_MTMMAT_H

#include <vector>
#include "MtmExceptions.h"
#include "Auxilaries.h"
#include "MtmVec.h"

using std::size_t;

namespace MtmMath {

    template <typename T>
    class MtmMat {
    protected:
        Dimensions dimOfMat;
        MtmVec<MtmVec<T>> matrix;
    public:
/*
 * Matrix constructor, dim_t is the dimension of the matrix and val is the initial value for the matrix elements
 */
        explicit MtmMat(Dimensions dim_t, const T& val = T()) try :
                dimOfMat(dim_t),
                matrix(dim_t.getRow(), MtmVec<T>(dim_t.getCol(), val)) {

        }catch(MtmExceptions::OutOfMemory& e) {
            throw;
        }catch(MtmExceptions::IllegalInitialization& e) {
            throw;
        }catch(MtmExceptions::MtmExceptions& e) {
            throw;
        }

        MtmMat(size_t r, size_t c, const T& val = T()) try :
                dimOfMat(r, c), matrix(r, MtmVec<T>(c, val)) {
            if(!c || !r) {
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

        explicit MtmMat(const MtmVec<T>& vec) try :
                dimOfMat(vec.getRowSize(), vec.getcolSize()),
                matrix(vec.getRowSize(), MtmVec<T>(vec.getcolSize())) {
            for(size_t i = 0 ; i<dimOfMat.getRow() ; ++i) {
                for(size_t j = 0 ; j<dimOfMat.getCol() ; ++j) {
                    matrix[i][j] = vec[i+j];
                }
            }
        }catch(MtmExceptions::OutOfMemory& e) {
            throw;
        }catch(MtmExceptions::IllegalInitialization& e) {
            throw;
        }catch(MtmExceptions::MtmExceptions& e) {
            throw;
        }

        MtmMat(const MtmMat& matToCopy) = default;//done
        MtmMat& operator=(const MtmMat& v) = default;//done
        ~MtmMat() = default;

        size_t rowSize() const {
            return dimOfMat.getRow();
        }

        size_t colSize() const {
            return dimOfMat.getCol();
        }

        MtmMat operator-() const; //done
        MtmMat& operator-=(const T& toSub);//done
        MtmMat& operator-=(const MtmMat& mat);//done
        MtmMat& operator-=(const MtmVec<T>& vec);//done
        MtmMat& operator+=(const T& toAdd);//done
        MtmMat& operator+=(const MtmMat& mat);//done
        MtmMat& operator+=(const MtmVec<T>& vec);//done
        MtmMat operator*(const MtmVec<T>& v);//done
        MtmMat& operator*=(const T& toMul);//done
        MtmMat operator*(const MtmMat& mat);//done
        MtmVec<T>& operator[](size_t i);//done
        const MtmVec<T>& operator[](size_t i) const;//done

        Dimensions getDimensions() const {
            Dimensions dim(rowSize(), colSize());
            return dim;
        }

        class iterator {
            MtmMat<T>* pointer;
            size_t indexRow, indexCol, linearIndex;

            friend class MtmMat<T>;

        public:
            iterator(MtmMat<T>* mat, size_t i, size_t j) try :
                    pointer(mat), indexRow(i), indexCol(j) {
                linearIndex = mat->rowSize()*j;
            }catch(std::bad_alloc& e) {
                throw MtmExceptions::OutOfMemory();
            }catch(std::exception& e) {
                throw MtmExceptions::MtmExceptions();
            }

            iterator(const iterator&) = default;//done
            ~iterator() = default;//done
            iterator& operator=(const iterator&) = default;//done
            T& operator*();//done
            virtual iterator& operator++();//done
            bool operator==(const iterator& it) const;//done
            bool operator!=(const iterator& it) const;//done
        };

        class nonzero_iterator : public iterator {
        public:
            nonzero_iterator(MtmMat<T>* mat, size_t i, size_t j) try :
                    iterator(mat, i, j) {

            }catch(std::bad_alloc& e) {
                throw MtmExceptions::OutOfMemory();
            }catch(std::exception& e) {
                throw MtmExceptions::MtmExceptions();
            }

            ~nonzero_iterator() = default;//done
            nonzero_iterator(const nonzero_iterator&) = default;//done
            nonzero_iterator& operator++() override;//done
        };


        virtual iterator begin();//done
        virtual iterator end();//done
        virtual nonzero_iterator nzbegin();//done
        virtual nonzero_iterator nzend();//done

/*
 * Function that get function object f and uses it's () operator on each element in the matrix columns.
 * It outputs a vector in the size of the matrix columns where each element is the final output
 * by the function object's * operator
 */
        template <typename Func>
        MtmVec<T> matFunc(Func& f) const {
            Func newFunc = f;
            MtmMat<T> tmp(*this);
            tmp.unlockTriag();
            MtmVec<T> newVec(colSize());
            newVec.transpose();
            for(size_t j = 0 ; j<colSize() ; j++) {
                for(size_t i = 0 ; i<rowSize() ; i++) {
                    newFunc(tmp[i][j]);
                }
                newVec[j] = *newFunc;
            }
            return newVec;
        }

        /**
         * resize a matrix to dimension dim, new elements gets the value val.
         */
        virtual void resize(Dimensions dim, const T& val = T()) {//done
            if(dim.getRow()==0 || dim.getCol()==0) {
                throw MtmExceptions::ChangeMatFail(dimOfMat, dim);
            }
            MtmMat<T> newMat(dim, val);
            for(size_t i(0) ; (i<dim.getRow()) && (i<rowSize()) ; ++i) {
                for(size_t j(0) ; (j<dim.getCol()) && (j<colSize()) ; ++j) {
                    newMat[i][j] = matrix[i][j];
                }
            }
            *this = newMat;
        }

/*
 * reshapes matrix so linear elements value are the same without changing num of elements.
 */
        virtual void reshape(Dimensions newDim) {
            size_t row_size = rowSize(), col_size = colSize();
            size_t new_row = newDim.getRow(), new_col = newDim.getCol();
            if((row_size*col_size)!=(new_row*new_col)) {
                throw MtmExceptions::ChangeMatFail(dimOfMat, newDim);
            }
            MtmMat<T> tmp(newDim, 0);
            MtmMat<T>::iterator old_element(begin()), new_element(tmp.begin());
            for(size_t i(0) ; i<(row_size*col_size) ; ++i) {
                *new_element = *old_element;
                ++old_element;
                ++new_element;
            }
            *this = tmp;
        }

        /*
         * Performs transpose operation on matrix
         */
        virtual void transpose() {//done
            size_t tmpRowSize = rowSize(), tmpColSize = colSize();
            Dimensions newDim(tmpColSize, tmpRowSize);
            MtmMat<T> transMat(newDim, 0);
            for(size_t j = 0 ; j<colSize() ; ++j) {
                for(size_t i = 0 ; i<rowSize() ; ++i) {
                    transMat[j][i] = matrix[i][j];
                }
            }
            dimOfMat.transpose();
            *this = transMat;
        }

        /**
         *
         * unlock func for arithmetics
         */
        void unlockTriag() {
            for(size_t i(0) ; i<rowSize() ; ++i) {
                this->matrix[i].lock_start = 0;
                this->matrix[i].lock_end = colSize();
            }
        }
    };


    /**---------------------operator(-)--------------------------------------**/

    template <typename T>
    MtmMat<T> MtmMat<T>::operator-() const {
        MtmMat<T> newMat(*this);
        newMat.unlockTriag();
        for(size_t i = 0 ; i<rowSize() ; ++i) {
            for(size_t j = 0 ; j<colSize() ; ++j) {
                newMat.matrix[i][j] = -(newMat.matrix[i][j]);
            }
        }
        return newMat;
    }

    template <typename T>
    MtmMat<T>& MtmMat<T>::operator-=(const T& toSub) {
        return *this += (-toSub);
    }

    template <typename T>
    MtmMat<T>& MtmMat<T>::operator-=(const MtmMat<T>& mat) {
        return *this += (-mat);
    }

    template <typename T>
    MtmMat<T>& MtmMat<T>::operator-=(const MtmVec<T>& vec) {
        return *this += (-vec);
    }

    /**---------------------operator(-) from outside the class---------------**/

    template <typename T>
    //done
    MtmMat<T> operator-(const MtmMat<T>& matA, const MtmMat<T>& matB) {
        return matA+(-matB);
    }

    template <typename T>
    //done
    MtmMat<T> operator-(const MtmMat<T>& matA, const T& toAdd) {
        return MtmMat<T>(matA) += -(toAdd);
    }

    template <typename T>
    //done
    MtmMat<T> operator-(const T& toAdd, const MtmMat<T>& matA) {
        MtmMat<T> tmp(matA);
        return -(tmp) += toAdd;
    }

    template <typename T>
    //done
    MtmMat<T> operator-(const MtmVec<T>& vec, const MtmMat<T>& matA) {
        MtmMat<T> tmp(matA);
        return -(tmp) += vec;
    }

    template <typename T>
    //done
    MtmMat<T> operator-(const MtmMat<T>& matA, const MtmVec<T>& vec) {
        return MtmMat<T>(matA) += -(vec);
    }

    /**---------------------------operator++++-------------------------------**/

    template <typename T>
    MtmMat<T>& MtmMat<T>::operator+=(const T& toAdd) {
        unlockTriag();
        for(size_t i = 0 ; i<rowSize() ; ++i) {
            for(size_t j = 0 ; j<colSize() ; ++j) {
                matrix[i][j] += toAdd;
            }
        }
        return *this;
    }

    template <typename T>
    MtmMat<T>& MtmMat<T>::operator+=(const MtmMat& mat) {
        if(rowSize()!=mat.rowSize() || colSize()!=mat.colSize()) {
            throw MtmExceptions::DimensionMismatch(dimOfMat, mat.dimOfMat);
        }
        unlockTriag();
        MtmMat<T> tmp(mat);
        tmp.unlockTriag();
        for(size_t i = 0 ; i<rowSize() ; ++i) {
            for(size_t j = 0 ; j<colSize() ; ++j) {
                matrix[i][j] += tmp.matrix[i][j];
            }
        }
        return *this;
    }

    template <typename T>
    MtmMat<T>& MtmMat<T>::operator+=(const MtmVec<T>& vec) {
        if(colSize()>1 && rowSize()>1) {
            throw MtmExceptions::DimensionMismatch(
                    dimOfMat,
                    vec.getDimensions());
        }
        unlockTriag();
        if(colSize()==vec.getLength() && vec.isRow()==1 && rowSize()==1) {
            for(size_t i = 0 ; i<rowSize() ; ++i) {
                for(size_t j = 0 ; j<colSize() ; ++j) {
                    matrix[i][j] += vec[j];
                }
            }
            return *this;
        }
        if(rowSize()==vec.getLength() && !vec.isRow() && colSize()==1) {
            for(size_t i = 0 ; i<rowSize() ; ++i) {
                for(size_t j = 0 ; j<colSize() ; ++j) {
                    matrix[i][j] += vec[i];
                }
            }
            return *this;
        }
        throw MtmExceptions::DimensionMismatch(dimOfMat, vec.getDimensions());
    }

    /**-------------------operator++++ from outside the class----------------**/

    template <typename T>
    //done
    MtmMat<T> operator+(const MtmMat<T>& matA, const MtmMat<T>& matB) {
        return MtmMat<T>(matA) += matB;
    }

    template <typename T>
    //done
    MtmMat<T> operator+(const MtmMat<T>& matA, const T& toAdd) {
        return MtmMat<T>(matA) += toAdd;
    }

    template <typename T>
    //done
    MtmMat<T> operator+(const T& toAdd, const MtmMat<T>& matA) {
        return MtmMat<T>(matA) += toAdd;
    }

    template <typename T>
    //done
    MtmMat<T> operator+(const MtmVec<T>& vec, const MtmMat<T>& matA) {
        return MtmMat<T>(matA) += vec;
    }

    template <typename T>
    //done
    MtmMat<T> operator+(const MtmMat<T>& matA, const MtmVec<T>& vec) {
        return MtmMat<T>(matA) += vec;
    }

    /**----------------------------------------------------------------------**/

    /**--------------------------operator******------------------------------**/

    template <typename T>
    MtmMat<T>& MtmMat<T>::operator*=(const T& toMul) {
        unlockTriag();
        for(size_t i = 0 ; i<rowSize() ; ++i) {
            for(size_t j = 0 ; j<colSize() ; ++j) {
                matrix[i][j] = (matrix[i][j])*toMul;
            }
        }
        return *this;
    }

    template <typename T>
    MtmMat<T> MtmMat<T>::operator*(const MtmVec<T>& v) {
        unlockTriag();
        if(v.isRow()==0 && v.getLength()==colSize()) {
            Dimensions newDim(rowSize(), 1);
            MtmMat<T> newMat(newDim, 0);
            for(size_t i = 0 ; i<rowSize() ; ++i) {
                for(size_t j = 0 ; j<colSize() ; ++j) {
                    newMat[i][0] += (matrix[i][j])*(v[j]);
                }
            }
            return newMat;
        }
        if(v.isRow()==1 && colSize()==1) {
            Dimensions newDim(rowSize(), v.getLength());
            MtmMat<T> newMat(newDim, 0);
            for(size_t i = 0 ; i<rowSize() ; ++i) {
                for(size_t j = 0 ; j<v.getLength() ; ++j) {
                    newMat[i][j] = (matrix[i][0])*(v[j]);
                }
            }
            return newMat;
        }
        throw MtmExceptions::DimensionMismatch(dimOfMat, v.getDimensions());
    }


    template <typename T>
    MtmMat<T> MtmMat<T>::operator*(const MtmMat<T>& mat) {
        if(colSize()!=mat.rowSize()) {
            throw MtmExceptions::DimensionMismatch(dimOfMat, mat.dimOfMat);
        }
        Dimensions newDim(rowSize(), mat.colSize());
        MtmMat<T> newMat(newDim, 0);
        unlockTriag();
        MtmMat<T> tmp(mat);
        tmp.unlockTriag();
        for(size_t i = 0 ; i<newMat.rowSize() ; ++i) {
            for(size_t j = 0 ; j<newMat.colSize() ; ++j) {
                for(size_t k = 0 ; k<mat.rowSize() ; ++k) {
                    newMat[i][j] += ((matrix[i][k])*(tmp.matrix[k][j]));
                }
            }
        }
        return newMat;
    }

    /**--------------------operator****** from outside the class-------------**/

    template <typename T>
    MtmMat<T> operator*(const MtmVec<T>& a, const MtmVec<T>& b) {
        if(a.isRow()!=b.isRow()) {
            if(a.isRow()==0) {////col*row
                Dimensions dem(a.getLength(), b.getLength());
                MtmMat<T> matrix(dem, 0);
                for(size_t i = 0 ; i<a.getLength() ; ++i) {
                    for(size_t j = 0 ; j<b.getLength() ; ++j) {
                        matrix[i][j] = (a[i])*(b[j]);
                    }
                }
                return matrix;
            }else if(a.getLength()!=b.getLength()) {
                throw MtmExceptions::DimensionMismatch(
                        a.getDimensions(),
                        b.getDimensions());
            }else {
                Dimensions dem(1, 1);
                MtmMat<T> matrix(dem, 0);
                for(size_t i = 0 ; i<a.getLength() ; ++i) {
                    matrix[0][0] += (a[i])*(b[i]);
                }
                return matrix;
            }
        }
        throw MtmExceptions::DimensionMismatch(
                a.getDimensions(),
                b.getDimensions());
    }


    template <typename T>
    //done
    MtmMat<T> operator*(const MtmMat<T>& mat, const T& toMul) {
        return MtmMat<T>(mat) *= toMul;
    }

    template <typename T>
    //done
    MtmMat<T> operator*(const T& toMul, const MtmMat<T>& mat) {
        return MtmMat<T>(mat) *= toMul;
    }

    template <typename T>
    //done
    MtmMat<T> operator*(const MtmVec<T>& vec, const MtmMat<T>& mat) {
        return MtmMat<T>(vec)*mat;
    }

    template <typename T>
    //done
    MtmMat<T> operator*(const MtmMat<T>& mat, const MtmVec<T>& vec) {
        return mat*vec;
    }

    template <typename T>
    //done
    MtmMat<T> operator*(const MtmMat<T>& matA, const MtmMat<T>& matB) {
        return matA*matB;
    }

    /**----------------------------------------------------------------------**/


    /**--------------------------operator[][][][]----------------------------**/

    template <typename T>
    const MtmVec<T>& MtmMat<T>::operator[](size_t i) const {
        return matrix[i];
    }

    template <typename T>
    MtmVec<T>& MtmMat<T>::operator[](size_t i) {
        return matrix[i];
    }

    /**--------------------------iterator------------------------------------**/


    template <typename T>
    T& MtmMat<T>::iterator::operator*() {
        if(indexRow<0 ||
                indexCol<0 ||
                indexRow>=pointer->rowSize() ||
                indexCol>=pointer->colSize()) {
            throw MtmExceptions::AccessIllegalElement();
        }
        return pointer->matrix[indexRow][indexCol];
    }

    template <typename T>
    typename MtmMat<T>::iterator& MtmMat<T>::iterator::operator++() {
        ++(linearIndex);
        ++(indexRow);
        if((indexRow)>=(pointer->rowSize())) {
            (indexRow) = 0;
            ++indexCol;
        }
        return *this;
    }

    template <typename T>
    bool MtmMat<T>::iterator::operator==(const iterator& it) const {
        if(pointer!=it.pointer) {
            return false;
        }
        return (indexRow==it.indexRow && indexCol==it.indexCol);
    }

    template <typename T>
    bool MtmMat<T>::iterator::operator!=(const iterator& it) const {
        return !(*this==it);
    }

    template <typename T>
    typename MtmMat<T>::iterator MtmMat<T>::begin() {
        return iterator(this, 0, 0);
    }

    template <typename T>
    typename MtmMat<T>::iterator MtmMat<T>::end() {
        return iterator(this, 0, colSize());
    }

    template <typename T>
    typename MtmMat<T>::nonzero_iterator&
    MtmMat<T>::nonzero_iterator::operator++() {
        if(*this==(this->pointer->nzend())) {
            return *this;
        }

        ++(this->linearIndex);
        ++(this->indexRow);
        if(this->indexRow>=(this->pointer->rowSize())) {
            (this->indexRow) = 0;
            ++(this->indexCol);
        }

        if(*this==(this->pointer->nzend())) {
            return *this;
        }

        if((this->pointer->operator[](this->indexRow).lock_start)<=
                (this->indexCol) &&
                (this->indexCol)<=
                        (this->pointer->operator[](this->indexRow).lock_end)) {
            if(**this==T()) {
                return operator++();
            }
            return *this;
        }
        return operator++();
    }

    template <typename T>
    typename MtmMat<T>::nonzero_iterator MtmMat<T>::nzbegin() {
        nonzero_iterator a(this, 0, 0);
        if(a.pointer->matrix[a.indexRow][a.indexCol]==T()) {
            return ++a;
        }
        return a;
    }

    template <typename T>
    typename MtmMat<T>::nonzero_iterator MtmMat<T>::nzend() {
        return nonzero_iterator(this, 0, colSize());
    }
}


#endif //EX3_MTMMAT_H
