//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "RISFluxes.h"

registerMooseObject("ExampleApp", RISFluxes);

InputParameters
RISFluxes::validParams()
{
  InputParameters params = Material::validParams();

  // Allow users to specify vectors defining the points of a piecewise function formed via linear
  // interpolation.
  params.addRequiredParam<std::vector<Real>>(
      "independent_vals",
      "The vector of z-coordinate values for a piecewise function's independent variable");
  params.addRequiredParam<std::vector<Real>>(
      "dependent_vals", "The vector of diffusivity values for a piecewise function's dependent");

  // Allow the user to specify which independent variable's gradient to use for calculating the
  // convection velocity property:
  /*
  params.addCoupledVar(
      "diffusion_gradient",
      "The gradient of this variable will be used to compute a velocity vector property.");
*/
  params.addCoupledVar(
      "vacancy_concentration",
      "Concentration of the vacancy defect due to irradiation");
  params.addCoupledVar(
      "interstitial_concentration",
      "Concentration of the vacancy defect due to irradiation");
  params.addCoupledVar(
      "Fe_concentration",
      "Concentration of the Fe atoms due to irradiation");
  params.addCoupledVar(
      "Cr_concentration",
      "Concentration of the Cr atoms due to irradiation");
  params.addCoupledVar(
      "Ni_concentration",
      "Concentration of the Ni atoms due to irradiation");
  params.addCoupledVar(
      "Si_concentration",
      "Concentration of the Si atoms due to irradiation");
  params.addRequiredParam<Real>( "mu_Fe", 1.0, "Chemical potential of Fe");
  params.addRequiredParam<Real>( "mu_Cr", 1.0, "Chemical potential of Cr");
  params.addRequiredParam<Real>( "mu_Ni", 1.0, "Chemical potential of Ni");
  params.addRequiredParam<Real>( "mu_Si", 1.0, "Chemical potential of Si");
  params.addRequiredParam<Real>( "mu_V", 1.0, "Chemical potential of vacancy");
  params.addRequiredParam<Real>( "mu_I", 1.0, "Chemical potential of interstitial");
  return params;
}

RISFluxes::RISFluxes(const InputParameters & parameters)
  : Material(parameters),
    // Declare that this material is going to provide a Real value typed
    // material property named "diffusivity" that Kernels and other objects can use.
    // This property is "bound" to the class's "_diffusivity" member.
    // Coupled Variables 
    _C_I(coupledValue("interstitial_concentration")),
    _C_V(coupledValue("vacancy_concentration")),
    _C_Fe(coupledValue("Fe_concentration")),
    _C_Cr(coupledValue("Cr_concentration")),
    _C_Ni(coupledValue("Ni_concentration")),
    _C_Si(coupledValue("Si_concentration")),
    // Chemical Potential for Solutes of Defects
    _mu_Fe(getParam<Real>("mu_Fe")),
    _mu_Cr(getParam<Real>("mu_Cr")),
    _mu_Ni(getParam<Real>("mu_Ni")),
    _mu_Si(getParam<Real>("mu_Si")),
    _mu_V(getParam<Real>("mu_V")),
    _mu_I(getParam<Real>("mu_I")),
    // Diffusion Flux of defects
    _J_Fe_V(declareADProperty<Real>("Fe_vacancy")),
    _J_Fe_I(declareADProperty<Real>("Fe_interstitial")),
    _J_Cr_V(declareADProperty<Real>("Cr_vacancy")),
    _J_Cr_I(declareADProperty<Real>("Cr_interstital")),
    _J_Ni_V(declareADProperty<Real>("Ni_vacancy")),
    _J_Ni_I(declareADProperty<Real>("Ni_interstital")),
    _J_Si_V(declareADProperty<Real>("Si_vacancy")),
    _J_Si_I(declareADProperty<Real>("Si_interstital")),
    // Onsager Coefficients
    _L_V(4,4),
    _L_I(4,4)
{
}

void
RISFluxes::computeOnsagerCoefQp()
{
}
void
RISFluxes::computeQpProperties()
{
    computeOnsagerCoefQp();

}
