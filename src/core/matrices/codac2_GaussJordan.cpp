/** 
 *  codac2_GaussJordan.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_GaussJordan.h"

using namespace std;

namespace codac2
{
  Matrix rising(const Matrix& R_, const Matrix& U_, const Matrix& A)
  {
    Matrix R = R_, U = U_;
    size_t n = A.rows(), m = A.cols();
    size_t p = m-n;

    for(int i = n-1 ; i > 0 ; i--)
    {
      Matrix K = U(i,i+p)*Matrix::Identity(n,n);
      K.block(0,i,i,1) = -U.block(0,i+p,i,1);
      R = K*R;
      U = R*A;
    }

    return R;
  }

  Matrix precond(const Matrix& P, const Matrix& L, const Matrix& U)
  {
    Matrix A = P.inverse()*(L*U);
    Matrix R = (P.inverse().eval()*L).inverse().eval();
    return rising(R,U,A);
  }

  Matrix gauss_jordan(const Matrix& A)
  {
    size_t n = A.rows(), m = A.cols();
    Eigen::FullPivLU<Matrix> lu(A);

    Matrix L = Matrix::Identity(n,n);
    //if(std::pow(L.determinant(),2) < 1e-5)
    //{
    //  cout << "[Matrix gauss_jordan(const Matrix& A)] -> eye Matrix" << endl;
    //  return Matrix::eye(n,n);
    //}

    L.block(0,0,n,n).triangularView<Eigen::StrictlyLower>() = lu.matrixLU();

    Matrix P = lu.permutationP();
    Matrix U = lu.matrixLU().triangularView<Eigen::Upper>();

    return precond(P,L,U);
  }
}