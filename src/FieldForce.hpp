
#ifndef FIELDFORCE_HPP
#define FIELDFORCE_HPP

#include "linearalgebra.hpp"

const lalge::Scalar vac_permit = 8.85418782e-12;
const lalge::Scalar vac_permea = 1.25663706e-6;

const lalge::Scalar grav_const = 6.673e-11;
const lalge::Scalar elec_const = ((lalge::Scalar) 1/(4*lalge::pi*vac_permit));
const lalge::Scalar mag_const = ((lalge::Scalar) vac_permea/(4*lalge::pi));

#endif
