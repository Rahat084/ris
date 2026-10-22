//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADRISConserveDefect.h"

registerMooseObject("MooseApp", ADRISConserveDefect);

InputParameters
ADRISConserveDefect::validParams()
{
  auto params = ADKernelGrad::validParams();
  params.addClassDescription("Same as `Diffusion` in terms of physics/residual, but the Jacobian "
                             "is computed using forward automatic differentiation");
  params.addRequiredParam<std::string>( "defect_type", "vacancy", "The defect with conserved diffusion flux");
  return params;
}

ADRISConserveDefect::ADRISConserveDefect(const InputParameters & parameters) : ADKernelGrad(parameters),
    _defect_type(getParam<std::string>("defect_type")),
    _J_Fe(getADMaterialProperty<Real>("Fe_" + _defect_type)),
    _J_Cr(getADMaterialProperty<Real>("Cr_" + _defect_type)),
    _J_Ni(getADMaterialProperty<Real>("Ni_" + _defect_type)),
    _J_Si(getADMaterialProperty<Real>("Si_" + _defect_type))
{
}

ADRealVectorValue
ADRISConserveDefect::precomputeQpResidual()
{
  //return _grad_u[_qp];
  return _J_Fe[_qp] + _J_Cr[_qp] + _J_Ni[_qp] + _J_Si[_qp];
}
