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

  /// Coupled Variables
  const VariableValue & _C_I;
  const VariableValue & _C_V;
  const VariableValue & _C_Fe;
  const VariableValue & _C_Cr;
  const VariableValue & _C_Ni;
  const VariableValue & _C_Si;
    // Chemical Potential for Solutes of Defects
  const Real _mu_Fe;
  const Real _mu_Cr;
  const Real _mu_Ni;
  const Real _mu_Si;
  const Real _mu_V;
  const Real _mu_I;
    // Diffusion Flux of defects
  ADMaterialProperty<Real> & _J_Fe_V;
  ADMaterialProperty<Real> & _J_Fe_I;
  ADMaterialProperty<Real> & _J_Cr_V;
  ADMaterialProperty<Real> & _J_Cr_I;
  ADMaterialProperty<Real> & _J_Ni_V;
  ADMaterialProperty<Real> & _J_Ni_I;
  ADMaterialProperty<Real> & _J_Si_V;
  ADMaterialProperty<Real> & _J_Si_I;
  //Onsager Coefficients
  DenseMatrix<Real> _L_V;
  DenseMatrix<Real> _L_I;
};
