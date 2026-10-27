//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ADIntegratedBC.h"

class ADDefectFluxBC : public ADIntegratedBC
{
public:
  static InputParameters validParams();

  ADDefectFluxBC(const InputParameters & parameters);

protected:
  ADReal computeQpResidual() override;

  // Defect type 
  std::string _defect_type;
  // Element names
  const std::vector<std::string> _element_names;
  // Defect flux
   std::vector <const ADMaterialProperty<std::vector<Real>>*> _element_defect_fluxes;
};
