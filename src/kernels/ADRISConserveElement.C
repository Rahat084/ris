//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADRISConserveElement.h"

registerMooseObject("RISApp", ADRISConserveElement);

InputParameters
ADRISConserveElement::validParams()
{
  auto params = ADKernelGrad::validParams();
  params.addClassDescription("Same as `Diffusion` in terms of physics/residual, but the Jacobian "
                             "is computed using forward automatic differentiation");
  params.addRequiredParam<std::string>( "element_name", "The entity with conserved diffusion flux");
  return params;
}

ADRISConserveElement::ADRISConserveElement(const InputParameters & parameters) : ADKernelGrad(parameters),
    _element_name(getParam<std::string>("element_name")),
    _J_I(getADMaterialProperty<std::vector<Real>>(_element_name + "_interstitial_flux")),
    _J_V(getADMaterialProperty<std::vector<Real>>(_element_name + "_vacancy_flux"))
{
}

ADRealVectorValue
ADRISConserveElement::precomputeQpResidual()
{
   MaterialProperty<ADRealVectorValue> J_element;
   for (const auto i : make_range(LIBMESH_DIM))
	   J_element[_qp](i) = _J_I[_qp][i] + _J_V[_qp][i];
  //return _grad_u[_qp];
  return J_element[_qp];
}
