//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ADKernelGrad.h"

class ADRISConserveDefect : public ADKernelGrad
{
public:
  static InputParameters validParams();

  ADRISConserveDefect(const InputParameters & parameters);
  
  // Defect type 
  std::string _defect_type;
  // Element names
  const std::vector<std::string> _element_names;
 
   std::vector<const ADMaterialProperty<RealVectorValue> *> _element_defect_fluxes;
  /*
  // Fe based defect Fluxes
  const ADMaterialProperty<std::vector<Real>>& _J_Fe;
  
 // Cr based defect Fluxes
  const ADMaterialProperty<std::vector<Real>>& _J_Cr;

 // Ni based defect Fluxes
  const ADMaterialProperty<std::vector<Real>>& _J_Ni;

 // Cr based defect Fluxes
  const ADMaterialProperty<std::vector<Real>>& _J_Si;
  */

protected:
  virtual ADRealVectorValue precomputeQpResidual() override;
};
