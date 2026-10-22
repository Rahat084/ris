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

class ADRISConserveSolute : public ADKernelGrad
{
public:
  static InputParameters validParams();

  ADRISConserveSolute(const InputParameters & parameters);
  
  // Solute Name
  std::string _solute_name;
 
  // Interstitial Fluxes
  const ADMaterialProperty<Real>& _J_I;
  
 // Vacancy Fluxes
  const ADMaterialProperty<Real>& _J_V;

protected:
  virtual ADRealVectorValue precomputeQpResidual() override;
};
