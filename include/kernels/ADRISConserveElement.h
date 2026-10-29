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

class ADRISConserveElement : public ADKernelGrad
{
public:
  static InputParameters validParams();

  ADRISConserveElement(const InputParameters & parameters);
  
  // Element Name
  std::string _element_name;
 
  // Interstitial Fluxes
  const ADMaterialProperty<RealVectorValue> & _J_I;
  
 // Vacancy Fluxes
  const ADMaterialProperty<RealVectorValue> & _J_V;

protected:
  virtual ADRealVectorValue precomputeQpResidual() override;
};
