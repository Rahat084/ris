//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADRISConserve.h"

registerMooseObject("MooseApp", ADRISConserve);

InputParameters
ADRISConserve::validParams()
{
  auto params = ADKernelGrad::validParams();
  params.addClassDescription("Same as `Diffusion` in terms of physics/residual, but the Jacobian "
                             "is computed using forward automatic differentiation");
  return params;
}

ADRISConserve::ADRISConserve(const InputParameters & parameters) : ADKernelGrad(parameters) {}

ADRealVectorValue
ADRISConserve::precomputeQpResidual()
{
  return _grad_u[_qp];
}
