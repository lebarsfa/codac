/** 
 *  codac2_Figure2D.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louédec
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Index.h"
#include "codac2_Figure2D.h"
#include "codac2_matrices.h"
#include "codac2_Matrix.h"

using namespace std;
using codac2::Vector;
using codac2::Matrix;

void codac2::Figure2D::draw_ellipsoid(const codac2::Ellipsoid &e, const codac2::StyleProperties &s) {
    //assert_release(this->size() <= e.size());
    for (const auto &output_fig: _output_figures) {
        Matrix G_draw(2, 2);
        Vector mu_draw(2);
        // 2d projection of the ellipsoid
        if (e.size() > 2) {
            // affine space of the projection
            Vector d(Eigen::VectorXd::Zero(e.mu.rows()));
            Matrix T(Eigen::MatrixXd::Zero(e.G.rows(), 2));
            T(output_fig->i(), 0) = 1;
            T(output_fig->j(), 1) = 1;

            // project ellipsoid E(mu,Q) = {x in R^n | (x-mu).T*G.{-T}*G^{-1}*(x-mu)<1}
            // on the affine plan A = {x|x=d+Tt} [Pope -2008]
            // reduce the dimensions of mu and Q

            auto TTG = T.transpose().eval() * e.G;
            Eigen::BDCSVD<Matrix> bdcsvd(Matrix(TTG.eval()), Eigen::ComputeFullU);
            Matrix U(bdcsvd.matrixU());
            Matrix E((Eigen::MatrixXd) bdcsvd.singularValues().asDiagonal());
            G_draw = U * E;
            mu_draw = T.transpose() * (d + T * T.transpose() * (e.mu - d));
        } else {
            G_draw = e.G;
            mu_draw = e.mu;
        }

        // draw the 2d ellipsoid
        Eigen::JacobiSVD<Eigen::MatrixXd> jsvd(G_draw, Eigen::ComputeThinU);
        Matrix U(jsvd.matrixU());
        Vector ab(jsvd.singularValues());

        double theta = atan2(U(1, 0), U(0, 0)).mid();

        output_fig->draw_ellipse(mu_draw, ab, theta, s);
    }
}