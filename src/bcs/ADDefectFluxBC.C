//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADDefectFluxBC.h"

registerMooseObject("MooseApp", ADDefectFluxBC);

InputParameters
ADDefectFluxBC::validParams()
{
  InputParameters params = ADIntegratedBC::validParams();
  params.addClassDescription("Imposes the Robin integrated boundary condition "
                             "$\\frac{\\partial u}{\\partial n}=u$.");
  params.addRequiredParam<std::string>(
      "defect_type",  "Type of defect for which the boundary condition is set ");
  params.addRequiredParam<std::vector<std::string>>(
      "element_names",
      {},
      "Name of the elements");
  return params;
}

ADDefectFluxBC::ADDefectFluxBC(const InputParameters & parameters)
  : ADIntegratedBC(parameters),
    _defect_type(getParam<std::string>("defect_type")),
    _element_names(getParam<std::vector<std::string>>("element_names"))
{
  for (const auto & name : _element_names)
    _element_defect_fluxes.push_back(&getADMaterialProperty<std::vector<Real>>(name + _defect_type));
}

ADReal
ADDefectFluxBC::computeQpResidual()
{
MaterialProperty<ADRealVectorValue> J_Defect;
for (unsigned int i = 0; _element_names.size(); ++i)
    for (unsigned int j = 0; j < LIBMESH_DIM; ++j)
	J_Defect[_qp](j) += (*_element_defect_fluxes[i])[_qp][j]; 

  return  J_Defect[_qp] * _normals[_qp] * _test[_i][_qp] ;
}
