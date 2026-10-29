//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Material.h"

class RISFluxes : public Material
{
public:
  RISFluxes(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties() override;
  void computeOnsagerCoefQp();

private:

  /// element names and diffusion coefficients
  const std::vector<std::string> _element_names;
  std::vector<Real> _element_int_diffCoef;
  std::vector<Real> _element_vac_diffCoef;
  /// Coupled Variables
  const ADVariableValue & _C_I;
  const ADVariableValue  & _C_V;
  std::vector<const ADVariableValue *>   _C_elements;
  // chemical potentials
  const ADVariableGradient & _grad_C_I;
  const ADVariableGradient & _grad_C_V;
  std::vector<const ADVariableGradient *>  _grad_C_elements;
  //Onsager Coefficients
  const Real _f0_V;
  const Real _f0_I;
  DenseMatrix<Real> _L_V;
  DenseMatrix<Real> _L_I;
  // Diffusion Flux of defects
   std::vector <ADMaterialProperty<std::vector<Real>>*> _element_int_fluxes;
   std::vector <ADMaterialProperty<std::vector<Real>>*> _element_vac_fluxes;
};
