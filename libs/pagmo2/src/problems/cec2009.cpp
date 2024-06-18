/* Copyright 2017-2020 PaGMO development team

This file is part of the PaGMO library.

The PaGMO library is free software; you can redistribute it and/or modify
it under the terms of either:

  * the GNU Lesser General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your
    option) any later version.

or

  * the GNU General Public License as published by the Free Software
    Foundation; either version 3 of the License, or (at your option) any
    later version.

or both in parallel, as here.

The PaGMO library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received codetail::pi()es of the GNU General Public License and the
GNU Lesser General Public License along with the PaGMO library.  If not,
see https://www.gnu.org/licenses/. */

#include <cmath>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <pagmo/detail/constants.hpp>
#include <pagmo/exceptions.hpp>
#include <pagmo/problem.hpp>
#include <pagmo/problems/cec2009.hpp>
#include <pagmo/s11n.hpp>
#include <pagmo/types.hpp>

// MINGW-specific warnings.
#if defined(__GNUC__) && defined(__MINGW32__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-attribute=pure"
#endif

namespace pagmo
{

namespace detail
{

namespace cec2009_data
{

namespace
{

// Number of objectives
const std::vector<unsigned short> nobj = {2, 2, 2, 2, 2, 2, 2, 3, 3, 3};

// Inequality constraints dimension
const std::vector<unsigned short> nic = {1, 1, 1, 1, 1, 2, 2, 1, 1, 1};

} // namespace

} // namespace cec2009_data

} // namespace detail

// Pointers to the member functions to be used in fitness
const std::vector<detail::cec2009_data::func_ptr> cec2009::s_u_ptr
    = {&cec2009::UF1, &cec2009::UF2, &cec2009::UF3, &cec2009::UF4, &cec2009::UF5,
       &cec2009::UF6, &cec2009::UF7, &cec2009::UF8, &cec2009::UF9, &cec2009::UF10};

const std::vector<detail::cec2009_data::func_ptr> cec2009::s_c_ptr
    = {&cec2009::CF1, &cec2009::CF2, &cec2009::CF3, &cec2009::CF4, &cec2009::CF5,
       &cec2009::CF6, &cec2009::CF7, &cec2009::CF8, &cec2009::CF9, &cec2009::CF10};

cec2009::cec2009(unsigned prob_id, bool is_constrained, unsigned dim)
    : m_prob_id(prob_id), m_is_constrained(is_constrained), m_dim(dim)
{
    if (prob_id < 1u || prob_id > 10u) {
        pagmo_throw(std::invalid_argument,
                    "Error: CEC2009 Test functions are only defined for prob_id in [1, 10], a prob_id of "
                        + std::to_string(prob_id) + " was requested.");
    }
    if (dim < 1u) {
        pagmo_throw(std::invalid_argument,
                    "Error: CEC2009 Test functions must have a non zero dimension: a dimension of "
                        + std::to_string(dim) + " was requested.");
    }
}

/// Inequality constraint dimension
/**
 * Returns the number of inequality constraints
 *
 * @return the number of inequality constraints
 */
vector_double::size_type cec2009::get_nic() const
{
    if (m_is_constrained) {
        return detail::cec2009_data::nic[m_prob_id - 1u];
    } else {
        return 0u;
    }
}

/// Number of objectives
/**
 * Returns the number of objectives
 *
 * @return the number of objectives
 */
vector_double::size_type cec2009::get_nobj() const
{
    return detail::cec2009_data::nobj[m_prob_id - 1u];
}

/// Box-bounds
/**
 * It returns the box-bounds for this UDP.
 *
 * @return the lower and upper bounds for each of the decision vector components
 */
std::pair<vector_double, vector_double> cec2009::get_bounds() const
{
    vector_double lb(m_dim, 0), ub(m_dim, 0);

    if (!m_is_constrained) { // For UF
        if (m_prob_id == 1u || m_prob_id == 2u || m_prob_id == 5u || m_prob_id == 6u || m_prob_id == 7u) {
            // [0,1] x [-1,1]^{n-1}
            lb[0] = 0.0;
            ub[0] = 1.0;
            for (decltype(m_dim) i = 1u; i < m_dim; ++i) {
                lb[i] = -1.0;
                ub[i] = 1.0;
            }
        } else if (m_prob_id == 3u) {
            // [0,1]^{n}
            for (decltype(m_dim) i = 0u; i < m_dim; ++i) {
                lb[i] = 0.0;
                ub[i] = 1.0;
            }
        } else if (m_prob_id == 4u) {
            // [0,1] x [-2,2]^{n-1}
            lb[0] = 0.0;
            ub[0] = 1.0;
            for (decltype(m_dim) i = 1u; i < m_dim; ++i) {
                lb[i] = -2.0;
                ub[i] = 2.0;
            }
        } else if (m_prob_id == 8u || m_prob_id == 9u || m_prob_id == 10u) {
            // [0,1]^{2} x [-2,2]^{n-2}
            lb[0] = 0.0;
            ub[0] = 1.0;
            lb[1] = 0.0;
            ub[1] = 1.0;
            for (decltype(m_dim) i = 2u; i < m_dim; ++i) {
                lb[i] = -2.0;
                ub[i] = 2.0;
            }
        }
    } else { // For CF
        if (m_prob_id == 2u) {
            // [0,1] x [-1,1]^{n-1}
            lb[0] = 0.0;
            ub[0] = 1.0;
            for (decltype(m_dim) i = 1u; i < m_dim; ++i) {
                lb[i] = -1.0;
                ub[i] = 1.0;
            }
        } else if (m_prob_id == 1u) {
            // [0,1]^{n}
            for (decltype(m_dim) i = 0u; i < m_dim; ++i) {
                lb[i] = 0.0;
                ub[i] = 1.0;
            }
        } else if (m_prob_id == 3u || m_prob_id == 4u || m_prob_id == 5u || m_prob_id == 6u || m_prob_id == 7u) {
            // [0,1] x [-2,2]^{n-1}
            lb[0] = 0.0;
            ub[0] = 1.0;
            for (decltype(m_dim) i = 1u; i < m_dim; ++i) {
                lb[i] = -2.0;
                ub[i] = 2.0;
            }
        } else if (m_prob_id == 8u) {
            // [0,1]^{2} x [-4,4]^{n-2}
            lb[0] = 0.0;
            ub[0] = 1.0;
            lb[1] = 0.0;
            ub[1] = 1.0;
            for (decltype(m_dim) i = 2u; i < m_dim; ++i) {
                lb[i] = -4.0;
                ub[i] = 4.0;
            }
        } else if (m_prob_id == 9u || m_prob_id == 10u) {
            // [0,1]^{2} x [-2,2]^{n-2}
            lb[0] = 0.0;
            ub[0] = 1.0;
            lb[1] = 0.0;
            ub[1] = 1.0;
            for (decltype(m_dim) i = 2u; i < m_dim; ++i) {
                lb[i] = -2.0;
                ub[i] = 2.0;
            }
        }
    }
    return std::make_pair(std::move(lb), std::move(ub));
}

/// Fitness computation
/**
 * Computes the fitness for this UDP
 *
 * @param x the decision vector.
 *
 * @return the fitness of \p x.
 */
vector_double cec2009::fitness(const vector_double &x) const
{
    if (m_is_constrained) {
        return fitness_impl(s_c_ptr[m_prob_id - 1], x);
    } else {
        return fitness_impl(s_u_ptr[m_prob_id - 1], x);
    }
}

/// Problem name
/**
 * @return a string containing the problem name
 */
std::string cec2009::get_name() const
{
    std::string retval("CEC2009 - ");
    if (!m_is_constrained) {
        retval.append("UF");
    } else {
        retval.append("CF");
    }
    retval.append(std::to_string(m_prob_id));
    return retval;
}

/// Object serialization
/**
 * This method will save/load \p this into the archive \p ar.
 *
 * @param ar target archive.
 *
 * @throws unspecified any exception thrown by the serialization of primitive types.
 */
template <typename Archive>
void cec2009::serialize(Archive &ar, unsigned)
{
    detail::archive(ar, m_prob_id, m_is_constrained, m_dim);
}

namespace
{

// Small helper for getting the sign of
// a double.
double sgn(double val)
{
    return ((val) > 0 ? 1.0 : -1.0);
}

} // namespace

// Pointers to member functions are used
vector_double cec2009::fitness_impl(detail::cec2009_data::func_ptr f, const vector_double &x) const
{
    const auto nic = m_is_constrained ? detail::cec2009_data::nic[m_prob_id - 1u] : 0u;
    vector_double retval(nic + detail::cec2009_data::nobj[m_prob_id - 1u], 0.);
    // Syntax is ugly as these are member function pointers.
    ((*this).*(f))(retval, x); // calls f
    return retval;
}

// Let's disable a few compiler warnings emitted by the cec2009 code.
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

// For the coverage analysis we do not cover the code below as its derived from a third party source
// LCOV_EXCL_START

// -------------------------------------------
void cec2009::UF1(vector_double &f, const vector_double &x) const
{
    double count1, count2;
    double sum1, sum2, yj;

    sum1 = sum2 = 0.0;
    count1 = count2 = 0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        yj = x[j - 1] - std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        yj = yj * yj;
        if (j % 2 == 0u) {
            sum2 += yj;
            count2++;
        } else {
            sum1 += yj;
            count1++;
        }
    }
    f[0] = x[0] + 2.0 * sum1 / count1;
    f[1] = 1.0 - std::sqrt(x[0]) + 2.0 * sum2 / count2;
}

void cec2009::UF2(vector_double &f, const vector_double &x) const
{
    double count1, count2;
    double sum1, sum2, yj;

    sum1 = sum2 = 0.0;
    count1 = count2 = 0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        if (j % 2 == 0u) {
            yj = x[j - 1]
                 - 0.3 * x[0]
                       * (x[0] * std::cos(24.0 * detail::pi() * x[0] + 4.0 * j * detail::pi() / (double)m_dim) + 2.0)
                       * std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
            sum2 += yj * yj;
            count2++;
        } else {
            yj = x[j - 1]
                 - 0.3 * x[0]
                       * (x[0] * std::cos(24.0 * detail::pi() * x[0] + 4.0 * j * detail::pi() / (double)m_dim) + 2.0)
                       * std::cos(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
            sum1 += yj * yj;
            count1++;
        }
    }
    f[0] = x[0] + 2.0 * sum1 / count1;
    f[1] = 1.0 - std::sqrt(x[0]) + 2.0 * sum2 / count2;
}

void cec2009::UF3(vector_double &f, const vector_double &x) const
{
    double count1, count2;
    double sum1, sum2, prod1, prod2, yj, pj;

    sum1 = sum2 = 0.0;
    count1 = count2 = 0;
    prod1 = prod2 = 1.0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        yj = x[j - 1] - std::pow(x[0], 0.5 * (1.0 + 3.0 * (j - 2.0) / ((double)m_dim - 2.0)));
        pj = std::cos(20.0 * yj * detail::pi() / std::sqrt(j + 0.0));
        if (j % 2 == 0u) {
            sum2 += yj * yj;
            prod2 *= pj;
            count2++;
        } else {
            sum1 += yj * yj;
            prod1 *= pj;
            count1++;
        }
    }
    f[0] = x[0] + 2.0 * (4.0 * sum1 - 2.0 * prod1 + 2.0) / count1;
    f[1] = 1.0 - std::sqrt(x[0]) + 2.0 * (4.0 * sum2 - 2.0 * prod2 + 2.0) / count2;
}

void cec2009::UF4(vector_double &f, const vector_double &x) const
{
    double count1, count2;
    double sum1, sum2, yj, hj;

    sum1 = sum2 = 0.0;
    count1 = count2 = 0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        yj = x[j - 1] - std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        hj = std::abs(yj) / (1.0 + std::exp(2.0 * std::abs(yj)));
        if (j % 2 == 0u) {
            sum2 += hj;
            count2++;
        } else {
            sum1 += hj;
            count1++;
        }
    }
    f[0] = x[0] + 2.0 * sum1 / count1;
    f[1] = 1.0 - x[0] * x[0] + 2.0 * sum2 / count2;
}

void cec2009::UF5(vector_double &f, const vector_double &x) const
{
    double count1, count2;
    double sum1, sum2, yj, hj, N, E;

    sum1 = sum2 = 0.0;
    count1 = count2 = 0;
    N = 10.0;
    E = 0.1;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        yj = x[j - 1] - std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        hj = 2.0 * yj * yj - std::cos(4.0 * detail::pi() * yj) + 1.0;
        if (j % 2 == 0u) {
            sum2 += hj;
            count2++;
        } else {
            sum1 += hj;
            count1++;
        }
    }
    hj = (0.5 / N + E) * std::abs(std::sin(2.0 * N * detail::pi() * x[0]));
    f[0] = x[0] + hj + 2.0 * sum1 / count1;
    f[1] = 1.0 - x[0] + hj + 2.0 * sum2 / count2;
}

void cec2009::UF6(vector_double &f, const vector_double &x) const
{
    double count1, count2;
    double sum1, sum2, prod1, prod2, yj, hj, pj, N, E;
    N = 2.0;
    E = 0.1;

    sum1 = sum2 = 0.0;
    count1 = count2 = 0;
    prod1 = prod2 = 1.0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        yj = x[j - 1] - std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        pj = std::cos(20.0 * yj * detail::pi() / std::sqrt(j + 0.0));
        if (j % 2 == 0u) {
            sum2 += yj * yj;
            prod2 *= pj;
            count2++;
        } else {
            sum1 += yj * yj;
            prod1 *= pj;
            count1++;
        }
    }

    hj = 2.0 * (0.5 / N + E) * std::sin(2.0 * N * detail::pi() * x[0]);
    if (hj < 0.0) hj = 0.0;
    f[0] = x[0] + hj + 2.0 * (4.0 * sum1 - 2.0 * prod1 + 2.0) / count1;
    f[1] = 1.0 - x[0] + hj + 2.0 * (4.0 * sum2 - 2.0 * prod2 + 2.0) / count2;
}

void cec2009::UF7(vector_double &f, const vector_double &x) const
{
    double count1, count2;
    double sum1, sum2, yj;

    sum1 = sum2 = 0.0;
    count1 = count2 = 0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        yj = x[j - 1] - std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        if (j % 2 == 0u) {
            sum2 += yj * yj;
            count2++;
        } else {
            sum1 += yj * yj;
            count1++;
        }
    }
    yj = std::pow(x[0], 0.2);
    f[0] = yj + 2.0 * sum1 / count1;
    f[1] = 1.0 - yj + 2.0 * sum2 / count2;
}

void cec2009::UF8(vector_double &f, const vector_double &x) const
{
    double count1, count2, count3;
    double sum1, sum2, sum3, yj;

    sum1 = sum2 = sum3 = 0.0;
    count1 = count2 = count3 = 0;
    for (decltype(m_dim) j = 3u; j <= m_dim; ++j) {
        yj = x[j - 1] - 2.0 * x[1] * std::sin(2.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        if (j % 3 == 1u) {
            sum1 += yj * yj;
            count1++;
        } else if (j % 3 == 2u) {
            sum2 += yj * yj;
            count2++;
        } else {
            sum3 += yj * yj;
            count3++;
        }
    }
    f[0] = std::cos(0.5 * detail::pi() * x[0]) * std::cos(0.5 * detail::pi() * x[1]) + 2.0 * sum1 / count1;
    f[1] = std::cos(0.5 * detail::pi() * x[0]) * std::sin(0.5 * detail::pi() * x[1]) + 2.0 * sum2 / count2;
    f[2] = std::sin(0.5 * detail::pi() * x[0]) + 2.0 * sum3 / count3;
}

void cec2009::UF9(vector_double &f, const vector_double &x) const
{
    double count1, count2, count3;
    double sum1, sum2, sum3, yj, E;

    E = 0.1;
    sum1 = sum2 = sum3 = 0.0;
    count1 = count2 = count3 = 0;
    for (decltype(m_dim) j = 3u; j <= m_dim; ++j) {
        yj = x[j - 1] - 2.0 * x[1] * std::sin(2.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        if (j % 3 == 1u) {
            sum1 += yj * yj;
            count1++;
        } else if (j % 3 == 2u) {
            sum2 += yj * yj;
            count2++;
        } else {
            sum3 += yj * yj;
            count3++;
        }
    }
    yj = (1.0 + E) * (1.0 - 4.0 * (2.0 * x[0] - 1.0) * (2.0 * x[0] - 1.0));
    if (yj < 0.0) yj = 0.0;
    f[0] = 0.5 * (yj + 2 * x[0]) * x[1] + 2.0 * sum1 / count1;
    f[1] = 0.5 * (yj - 2 * x[0] + 2.0) * x[1] + 2.0 * sum2 / count2;
    f[2] = 1.0 - x[1] + 2.0 * sum3 / count3;
}

void cec2009::UF10(vector_double &f, const vector_double &x) const
{
    double count1, count2, count3;
    double sum1, sum2, sum3, yj, hj;

    sum1 = sum2 = sum3 = 0.0;
    count1 = count2 = count3 = 0;
    for (decltype(m_dim) j = 3u; j <= m_dim; ++j) {
        yj = x[j - 1] - 2.0 * x[1] * std::sin(2.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        hj = 4.0 * yj * yj - std::cos(8.0 * detail::pi() * yj) + 1.0;
        if (j % 3 == 1u) {
            sum1 += hj;
            count1++;
        } else if (j % 3 == 2u) {
            sum2 += hj;
            count2++;
        } else {
            sum3 += hj;
            count3++;
        }
    }
    f[0] = std::cos(0.5 * detail::pi() * x[0]) * std::cos(0.5 * detail::pi() * x[1]) + 2.0 * sum1 / count1;
    f[1] = std::cos(0.5 * detail::pi() * x[0]) * std::sin(0.5 * detail::pi() * x[1]) + 2.0 * sum2 / count2;
    f[2] = std::sin(0.5 * detail::pi() * x[0]) + 2.0 * sum3 / count3;
}

/****************************************************************************/
// constraint test instances
/****************************************************************************/
void cec2009::CF1(vector_double &f, const vector_double &x) const
{
    double count1, count2;
    double sum1, sum2, yj, N, a;
    N = 10.0;
    a = 1.0;

    sum1 = sum2 = 0.0;
    count1 = count2 = 0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        yj = x[j - 1] - std::pow(x[0], 0.5 * (1.0 + 3.0 * (j - 2.0) / ((double)m_dim - 2.0)));
        if (j % 2 == 1u) {
            sum1 += yj * yj;
            count1++;
        } else {
            sum2 += yj * yj;
            count2++;
        }
    }
    f[0] = x[0] + 2.0 * sum1 / count1;
    f[1] = 1.0 - x[0] + 2.0 * sum2 / count2;
    // Inequality constraint
    f[2] = f[1] + f[0] - a * std::abs(std::sin(N * detail::pi() * (f[0] - f[1] + 1.0))) - 1.0;
    f[2] = -f[2]; // convert to g(x) <= 0 form
}

void cec2009::CF2(vector_double &f, const vector_double &x) const
{
    double count1, count2;
    double sum1, sum2, yj, N, a, t;
    N = 2.0;
    a = 1.0;

    sum1 = sum2 = 0.0;
    count1 = count2 = 0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        if (j % 2 == 1) {
            yj = x[j - 1] - std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
            sum1 += yj * yj;
            count1++;
        } else {
            yj = x[j - 1] - std::cos(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
            sum2 += yj * yj;
            count2++;
        }
    }
    f[0] = x[0] + 2.0 * sum1 / count1;
    f[1] = 1.0 - std::sqrt(x[0]) + 2.0 * sum2 / count2;
    // Inequality constraint
    t = f[1] + std::sqrt(f[0]) - a * std::sin(N * detail::pi() * (sqrt(f[0]) - f[1] + 1.0)) - 1.0;
    f[2] = sgn(t) * std::abs(t) / (1 + std::exp(4.0 * std::abs(t)));
    f[2] = -f[2]; // convert to g(x) <= 0 form
}

void cec2009::CF3(vector_double &f, const vector_double &x) const
{
    double count1, count2;
    double sum1, sum2, prod1, prod2, yj, pj, N, a;
    N = 2.0;
    a = 1.0;

    sum1 = sum2 = 0.0;
    count1 = count2 = 0;
    prod1 = prod2 = 1.0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        yj = x[j - 1] - std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        pj = std::cos(20.0 * yj * detail::pi() / std::sqrt(j + 0.0));
        if (j % 2 == 0u) {
            sum2 += yj * yj;
            prod2 *= pj;
            count2++;
        } else {
            sum1 += yj * yj;
            prod1 *= pj;
            count1++;
        }
    }

    f[0] = x[0] + 2.0 * (4.0 * sum1 - 2.0 * prod1 + 2.0) / count1;
    f[1] = 1.0 - x[0] * x[0] + 2.0 * (4.0 * sum2 - 2.0 * prod2 + 2.0) / count2;
    // Inequality constraint
    f[2] = f[1] + f[0] * f[0] - a * std::sin(N * detail::pi() * (f[0] * f[0] - f[1] + 1.0)) - 1.0;
    f[2] = -f[2]; // convert to g(x) <= 0 form
}

void cec2009::CF4(vector_double &f, const vector_double &x) const
{
    double sum1, sum2, yj, t;

    sum1 = sum2 = 0.0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        yj = x[j - 1] - std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        if (j % 2 == 1u) {
            sum1 += yj * yj;
        } else {
            if (j == 2u)
                sum2 += yj < 1.5 - 0.75 * std::sqrt(2.0) ? std::abs(yj) : (0.125 + (yj - 1) * (yj - 1));
            else
                sum2 += yj * yj;
        }
    }
    f[0] = x[0] + sum1;
    f[1] = 1.0 - x[0] + sum2;
    // Inequality constraint
    t = x[1] - std::sin(6.0 * x[0] * detail::pi() + 2.0 * detail::pi() / (double)m_dim) - 0.5 * x[0] + 0.25;
    f[2] = sgn(t) * std::abs(t) / (1 + std::exp(4.0 * std::abs(t)));
    f[2] = -f[2]; // convert to g(x) <= 0 form
}

void cec2009::CF5(vector_double &f, const vector_double &x) const
{
    double sum1, sum2, yj;

    sum1 = sum2 = 0.0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        if (j % 2 == 1u) {
            yj = x[j - 1] - 0.8 * x[0] * std::cos(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
            sum1 += 2.0 * yj * yj - std::cos(4.0 * detail::pi() * yj) + 1.0;
        } else {
            yj = x[j - 1] - 0.8 * x[0] * std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
            if (j == 2u)
                sum2 += yj < 1.5 - 0.75 * std::sqrt(2.0) ? std::abs(yj) : (0.125 + (yj - 1) * (yj - 1));
            else
                sum2 += 2.0 * yj * yj - std::cos(4.0 * detail::pi() * yj) + 1.0;
        }
    }
    f[0] = x[0] + sum1;
    f[1] = 1.0 - x[0] + sum2;
    // Inequality constraint
    f[2] = x[1] - 0.8 * x[0] * std::sin(6.0 * x[0] * detail::pi() + 2.0 * detail::pi() / (double)m_dim) - 0.5 * x[0]
           + 0.25;
    f[2] = -f[2]; // convert to g(x) <= 0 form
}

void cec2009::CF6(vector_double &f, const vector_double &x) const
{
    double sum1, sum2, yj;

    sum1 = sum2 = 0.0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        if (j % 2 == 1u) {
            yj = x[j - 1] - 0.8 * x[0] * std::cos(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
            sum1 += yj * yj;
        } else {
            yj = x[j - 1] - 0.8 * x[0] * std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
            sum2 += yj * yj;
        }
    }
    f[0] = x[0] + sum1;
    f[1] = (1.0 - x[0]) * (1.0 - x[0]) + sum2;
    // Inequality constraint
    f[2] = x[1] - 0.8 * x[0] * std::sin(6.0 * x[0] * detail::pi() + 2.0 * detail::pi() / (double)m_dim)
           - sgn((x[0] - 0.5) * (1.0 - x[0])) * std::sqrt(std::abs((x[0] - 0.5) * (1.0 - x[0])));
    f[3] = x[3] - 0.8 * x[0] * std::sin(6.0 * x[0] * detail::pi() + 4.0 * detail::pi() / (double)m_dim)
           - sgn(0.25 * std::sqrt(1 - x[0]) - 0.5 * (1.0 - x[0]))
                 * std::sqrt(std::abs(0.25 * std::sqrt(1 - x[0]) - 0.5 * (1.0 - x[0])));
    // convert to g(x) <= 0 form
    f[2] = -f[2];
    f[3] = -f[3];
}

void cec2009::CF7(vector_double &f, const vector_double &x) const
{
    double sum1, sum2, yj;

    sum1 = sum2 = 0.0;
    for (decltype(m_dim) j = 2u; j <= m_dim; ++j) {
        if (j % 2 == 1u) {
            yj = x[j - 1] - std::cos(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
            sum1 += 2.0 * yj * yj - std::cos(4.0 * detail::pi() * yj) + 1.0;
        } else {
            yj = x[j - 1] - std::sin(6.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
            if (j == 2u || j == 4u)
                sum2 += yj * yj;
            else
                sum2 += 2.0 * yj * yj - std::cos(4.0 * detail::pi() * yj) + 1.0;
        }
    }
    f[0] = x[0] + sum1;
    f[1] = (1.0 - x[0]) * (1.0 - x[0]) + sum2;
    // Inequality constraint
    f[2] = x[1] - std::sin(6.0 * x[0] * detail::pi() + 2.0 * detail::pi() / (double)m_dim)
           - sgn((x[0] - 0.5) * (1.0 - x[0])) * std::sqrt(std::abs((x[0] - 0.5) * (1.0 - x[0])));
    f[3] = x[3] - std::sin(6.0 * x[0] * detail::pi() + 4.0 * detail::pi() / (double)m_dim)
           - sgn(0.25 * std::sqrt(1 - x[0]) - 0.5 * (1.0 - x[0]))
                 * std::sqrt(std::abs(0.25 * std::sqrt(1 - x[0]) - 0.5 * (1.0 - x[0])));
    // convert to g(x) <= 0 form
    f[2] = -f[2];
    f[3] = -f[3];
}

void cec2009::CF8(vector_double &f, const vector_double &x) const
{
    double count1, count2, count3;
    double sum1, sum2, sum3, yj, N, a;
    N = 2.0;
    a = 4.0;

    sum1 = sum2 = sum3 = 0.0;
    count1 = count2 = count3 = 0;
    for (decltype(m_dim) j = 3u; j <= m_dim; ++j) {
        yj = x[j - 1] - 2.0 * x[1] * std::sin(2.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        if (j % 3 == 1u) {
            sum1 += yj * yj;
            count1++;
        } else if (j % 3 == 2u) {
            sum2 += yj * yj;
            count2++;
        } else {
            sum3 += yj * yj;
            count3++;
        }
    }
    f[0] = std::cos(0.5 * detail::pi() * x[0]) * std::cos(0.5 * detail::pi() * x[1]) + 2.0 * sum1 / count1;
    f[1] = std::cos(0.5 * detail::pi() * x[0]) * std::sin(0.5 * detail::pi() * x[1]) + 2.0 * sum2 / count2;
    f[2] = std::sin(0.5 * detail::pi() * x[0]) + 2.0 * sum3 / count3;
    // Inequality constraint
    f[3] = (f[0] * f[0] + f[1] * f[1]) / (1 - f[2] * f[2])
           - a * std::abs(sin(N * detail::pi() * ((f[0] * f[0] - f[1] * f[1]) / (1 - f[2] * f[2]) + 1.0))) - 1.0;
    f[3] = -f[3]; // convert to g(x) <= 0 form
}

void cec2009::CF9(vector_double &f, const vector_double &x) const
{
    double count1, count2, count3;
    double sum1, sum2, sum3, yj, N, a;
    N = 2.0;
    a = 3.0;

    sum1 = sum2 = sum3 = 0.0;
    count1 = count2 = count3 = 0;
    for (decltype(m_dim) j = 3u; j <= m_dim; ++j) {
        yj = x[j - 1] - 2.0 * x[1] * std::sin(2.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        if (j % 3 == 1u) {
            sum1 += yj * yj;
            count1++;
        } else if (j % 3 == 2u) {
            sum2 += yj * yj;
            count2++;
        } else {
            sum3 += yj * yj;
            count3++;
        }
    }
    f[0] = std::cos(0.5 * detail::pi() * x[0]) * std::cos(0.5 * detail::pi() * x[1]) + 2.0 * sum1 / count1;
    f[1] = std::cos(0.5 * detail::pi() * x[0]) * std::sin(0.5 * detail::pi() * x[1]) + 2.0 * sum2 / count2;
    f[2] = std::sin(0.5 * detail::pi() * x[0]) + 2.0 * sum3 / count3;
    // Inequality constraint
    f[3] = (f[0] * f[0] + f[1] * f[1]) / (1 - f[2] * f[2])
           - a * std::sin(N * detail::pi() * ((f[0] * f[0] - f[1] * f[1]) / (1 - f[2] * f[2]) + 1.0)) - 1.0;
    f[3] = -f[3]; // convert to g(x) <= 0 form
}

void cec2009::CF10(vector_double &f, const vector_double &x) const
{
    double count1, count2, count3;
    double sum1, sum2, sum3, yj, hj, N, a;
    N = 2.0;
    a = 1.0;

    sum1 = sum2 = sum3 = 0.0;
    count1 = count2 = count3 = 0;
    for (decltype(m_dim) j = 3u; j <= m_dim; ++j) {
        yj = x[j - 1] - 2.0 * x[1] * std::sin(2.0 * detail::pi() * x[0] + j * detail::pi() / (double)m_dim);
        hj = 4.0 * yj * yj - std::cos(8.0 * detail::pi() * yj) + 1.0;
        if (j % 3 == 1u) {
            sum1 += hj;
            count1++;
        } else if (j % 3 == 2u) {
            sum2 += hj;
            count2++;
        } else {
            sum3 += hj;
            count3++;
        }
    }
    f[0] = std::cos(0.5 * detail::pi() * x[0]) * std::cos(0.5 * detail::pi() * x[1]) + 2.0 * sum1 / count1;
    f[1] = std::cos(0.5 * detail::pi() * x[0]) * std::sin(0.5 * detail::pi() * x[1]) + 2.0 * sum2 / count2;
    f[2] = std::sin(0.5 * detail::pi() * x[0]) + 2.0 * sum3 / count3;
    // Inequality constraint
    f[3] = (f[0] * f[0] + f[1] * f[1]) / (1 - f[2] * f[2])
           - a * std::sin(N * detail::pi() * ((f[0] * f[0] - f[1] * f[1]) / (1 - f[2] * f[2]) + 1.0)) - 1.0;
    f[3] = -f[3]; // convert to g(x) <= 0 form
}
// -------------------------------------------
// LCOV_EXCL_STOP

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

} // namespace pagmo

PAGMO_S11N_PROBLEM_IMPLEMENT(pagmo::cec2009)
