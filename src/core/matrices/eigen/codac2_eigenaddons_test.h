/** 
 *  \file codac2_eigenaddons.h
 * 
 *  This file is included in the declaration of Eigen::MatrixBase,
 *  thanks to the preprocessor token EIGEN_MATRIXBASE_PLUGIN.
 *  See: https://eigen.tuxfamily.org/dox/TopicCustomizing_Plugins.html
 *  and the file codac2_matrices.h
 * 
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2023 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */



inline bool is_empty() const
{
  for(size_t i = 0 ; i < rows() ; i++)
    for(size_t j = 0 ; j < cols() ; j++)
      if((*this)(i,j).is_empty())
        return true;
  return false;
}