//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADRecombination.h"

registerMooseObject("RISApp", ADRecombination);

InputParameters
ADRecombination::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription(
      "Kernel representing the contribution of the PDE term $-L*v$, where $L$ is a reaction rate "
      "material property, $v$ is a scalar variable (nonlinear or coupled), and whose Jacobian "
      "contribution is calculated using automatic differentiation.");
  params.addRequiredParam<Real>(
      "recombination_rate",  "Rates of the recombination of element vacancy and interstitials");
  params.addRequiredCoupledVar(
      "defect_conjugate",
      "Concentration of the conjugate defect of the alloy");
  return params;
}

ADRecombination::ADRecombination(const InputParameters & parameters)
  : ADKernel(parameters),
    _recomb_rate(getParam<Real>("recombination_rate")),
    _C_defect(coupledValue("defect_conjugate"))
{
}

ADReal
ADRecombination::computeQpResidual()
{
  return _test[_i][_qp] * _recomb_rate * _C_defect[_qp] * _u[_qp];
}
