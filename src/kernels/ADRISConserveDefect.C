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
  params.addRequiredParam<std::vector<std::string>>(
      "element_names",
      {},
      "Name of the elements");
  return params;
}

ADRISConserveDefect::ADRISConserveDefect(const InputParameters & parameters) : ADKernelGrad(parameters),
    _defect_type(getParam<std::string>("defect_type")),
    _element_names(getParam<std::vector<std::string>>("element_names"))
    /*
    _J_Fe(getADMaterialProperty<std::vector<Real>>("Fe_" + _defect_type)),
    _J_Cr(getADMaterialProperty<std::vector<Real>>("Cr_" + _defect_type)),
    _J_Ni(getADMaterialProperty<std::vector<Real>>("Ni_" + _defect_type)),
    _J_Si(getADMaterialProperty<std::vector<Real>>("Si_" + _defect_type))
    */
{
  for (const auto & name : _element_names)
    _element_defect_fluxes.push_back(&getADMaterialProperty<std::vector<Real>>(name + _defect_type));
}

ADRealVectorValue
ADRISConserveDefect::precomputeQpResidual()
{
MaterialProperty<ADRealVectorValue> J_Defect;
for (unsigned int i = 0; _element_names.size(); ++i)
    for (unsigned int j = 0; j < LIBMESH_DIM; ++j)
	J_Defect[_qp](j) += (*_element_defect_fluxes[i])[_qp][j]; 
  //return _grad_u[_qp];
  return J_Defect[_qp];
}
