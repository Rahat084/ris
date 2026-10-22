//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADRISConserveSolute.h"

registerMooseObject("MooseApp", ADRISConserveSolute);

InputParameters
ADRISConserveSolute::validParams()
{
  auto params = ADKernelGrad::validParams();
  params.addClassDescription("Same as `Diffusion` in terms of physics/residual, but the Jacobian "
                             "is computed using forward automatic differentiation");
  params.addRequiredParam<std::string>( "Solute_name", "Fe", "The entity with conserved diffusion flux");
  return params;
}

ADRISConserveSolute::ADRISConserveSolute(const InputParameters & parameters) : ADKernelGrad(parameters),
    _solute_name(getParam<std::string>("solute_name")),
    _J_I(getADMaterialProperty<Real>(_solute_name + "_interstitial")),
    _J_V(getADMaterialProperty<Real>(_solute_name + "_vacancy"))
{
}

ADRealVectorValue
ADRISConserveSolute::precomputeQpResidual()
{
  //return _grad_u[_qp];
  return _J_I[_qp] + _J_V[_qp];
}
