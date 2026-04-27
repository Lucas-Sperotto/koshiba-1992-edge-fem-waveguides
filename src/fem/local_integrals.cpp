#include "koshiba/fem/local_integrals.hpp"

#include "koshiba/fem/edge_shape.hpp"

namespace koshiba::fem {

LocalIntegrals compute_local_integrals(const mesh::Triangle& triangle) {
    const EdgeShape edge_shape(triangle);
    const auto& edge = edge_shape.coefficients();
    const auto nodal = triangle.nodal_coefficients();
    const auto& vertices = triangle.vertices();
    const auto centroid = triangle.centroid();
    const double area = triangle.area();

    const double sum_x2_plus_centroid =
        vertices[0].x * vertices[0].x +
        vertices[1].x * vertices[1].x +
        vertices[2].x * vertices[2].x +
        9.0 * centroid.x * centroid.x;
    const double sum_y2_plus_centroid =
        vertices[0].y * vertices[0].y +
        vertices[1].y * vertices[1].y +
        vertices[2].y * vertices[2].y +
        9.0 * centroid.y * centroid.y;

    LocalIntegrals integrals{
        Eigen::Matrix3d::Zero(),
        Eigen::Matrix3d::Zero(),
        Eigen::Matrix3d::Zero(),
        Eigen::Matrix3d::Zero(),
        Eigen::Matrix3d::Zero(),
        Eigen::Matrix3d::Zero(),
        Eigen::Matrix3d::Zero(),
        Eigen::Matrix3d::Zero(),
    };

    for (Eigen::Index row = 0; row < 3; ++row) {
        for (Eigen::Index col = 0; col < 3; ++col) {
            const std::size_t r = static_cast<std::size_t>(row);
            const std::size_t c = static_cast<std::size_t>(col);

            // Eq. (A1): integral of {U}{U}^T.
            integrals.a1_uu(row, col) =
                area * edge.a[r] * edge.a[c] +
                area * centroid.y * (edge.a[r] * edge.c[c] + edge.c[r] * edge.a[c]) +
                (area / 12.0) * edge.c[r] * edge.c[c] * sum_y2_plus_centroid;

            // Eq. (A2): integral of {V}{V}^T.
            integrals.a2_vv(row, col) =
                area * edge.b[r] * edge.b[c] -
                area * centroid.x * (edge.b[r] * edge.c[c] + edge.c[r] * edge.b[c]) +
                (area / 12.0) * edge.c[r] * edge.c[c] * sum_x2_plus_centroid;

            // Eq. (A3): derivative products of edge functions.
            integrals.a3_derivative(row, col) = area * edge.c[r] * edge.c[c];

            // Eq. (A4): integral of {U}{N_x}^T.
            integrals.a4_u_nx(row, col) =
                0.5 * (edge.a[r] + edge.c[r] * centroid.y) * nodal[c].b;

            // Eq. (A5): integral of {V}{N_y}^T.
            integrals.a5_v_ny(row, col) =
                0.5 * (edge.b[r] - edge.c[r] * centroid.x) * nodal[c].c;

            // Eq. (A6): consistent nodal mass.
            integrals.a6_nn(row, col) = area / 12.0 * (row == col ? 2.0 : 1.0);

            // Eq. (A7): integral of {N_x}{N_x}^T.
            integrals.a7_nx_nx(row, col) = nodal[r].b * nodal[c].b / (4.0 * area);

            // Eq. (A8): integral of {N_y}{N_y}^T.
            integrals.a8_ny_ny(row, col) = nodal[r].c * nodal[c].c / (4.0 * area);
        }
    }

    return integrals;
}

}  // namespace koshiba::fem
