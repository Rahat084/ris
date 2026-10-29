//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "RISFluxes.h"

registerMooseObject("RISApp", RISFluxes);

InputParameters
RISFluxes::validParams()
{
  InputParameters params = Material::validParams();

  // Allow users to specify vectors defining the points of a piecewise function formed via linear
  // interpolation.
  params.addRequiredParam<std::vector<std::string>>(
      "element_names",
      "Name of the elements");
  params.addRequiredParam<std::vector<Real>>(
      "element_int_diffCoef",  "Interstitial Diffusion Coefficient of the elements");
  params.addRequiredParam<std::vector<Real>>(
      "element_vac_diffCoef",  "Vacancy Diffusion Coefficient of the elements");
  params.addRequiredCoupledVar(
      "vacancy_concentration",
      "Concentration of the vacancy defect due to irradiation");
  params.addRequiredCoupledVar(
      "interstitial_concentration",
      "Concentration of the interstitial defect due to irradiation");
  params.addRequiredCoupledVar(
      "element_concentration",
      "Concentration of the elements of the alloy");
  /*
  params.addRequiredParam<std::vector<std::string>>( "coupledvar_element_names", {}, "name of the coupled variables of the elements");
  params.addRequiredParam<Real>( "mu_V", 1.0, "Chemical potential of vacancy");
  params.addRequiredParam<Real>( "mu_I", 1.0, "Chemical potential of interstitial");
  params.addRequiredParam<std::vector<Real>>( "mu_elements", {}, "Chemical potential of elements");
  */
  params.addRequiredParam<Real>( "corr_factor_vac", "Correlation factor of vacancies");
  params.addRequiredParam<Real>( "corr_factor_int", "Correlation factor of interstitials");
  return params;
}

RISFluxes::RISFluxes(const InputParameters & parameters)
  : Material(parameters),
    _element_names(getParam<std::vector<std::string>>("element_names")),
    _element_int_diffCoef(getParam<std::vector<Real>>("element_int_diffCoef")),
    _element_vac_diffCoef(getParam<std::vector<Real>>("element_vac_diffCoef")),
    // Coupled Variables 
    _C_I(adCoupledValue("interstitial_concentration")),
    _C_V(adCoupledValue("vacancy_concentration")),
    _C_elements(adCoupledValues("element_concentration")),
    // Chemical Potential for Solutes of Defects
    _grad_C_I(adCoupledGradient("interstitial_concentration")),
    _grad_C_V(adCoupledGradient("vacancy_concentration")),
    _grad_C_elements(adCoupledGradients("element_concentration")),
    // Diffusion Flux of defects
    // Onsager Coefficients
    _f0_V(getParam<Real>("corr_factor_vac")),
    _f0_I(getParam<Real>("corr_factor_int")),
    _L_V(_element_names.size(),_element_names.size()),
    _L_I(_element_names.size(),_element_names.size())
{
      for (const auto & name : _element_names)
      {
    _element_int_fluxes.push_back(&declareADProperty<std::vector<Real>>(name + "_interstitial_flux"));
    _element_vac_fluxes.push_back(&declareADProperty<std::vector<Real>>(name + "_vacancy_flux"));
      }
      /*
      _C_elements.resize(_element_names.size());
      _grad_C_elements.resize(_element_names.size());
      for (unsigned int i = 0; i < _element_names.size(); ++i)

      {_C_elements[i] = &CoupledValue("element_concentration", i);
	_grad_C_elements[i] = &CoupledGradient("element_concentration", i);
      }
      */
}

void
RISFluxes::computeOnsagerCoefQp()
{
    Real CmDm_V = 0; 
    Real CmDm_I = 0;

    for (unsigned int i=0; i <_element_names.size(); ++i)
    { 
	CmDm_V += (*_C_elements[i])[_qp].value() * _element_vac_diffCoef[i];
	CmDm_I += (*_C_elements[i])[_qp].value() * _element_int_diffCoef[i];
    }

    for (unsigned int i=0; i<_element_names.size(); ++i)
	{
	    _L_V(i,i) =  ((*_C_elements[i])[_qp].value() * _element_vac_diffCoef[i] * _C_V[_qp].value()) * (1 + (1 -_f0_V)/(_f0_V) * ((*_C_elements[i])[_qp].value() * _element_vac_diffCoef[i])/CmDm_V); 

	    _L_I(i,i) =  ((*_C_elements[i])[_qp].value() * _element_int_diffCoef[i] * _C_I[_qp].value()) * (1 + (1 -_f0_I)/(_f0_I) * ( (*_C_elements[i])[_qp].value() * _element_int_diffCoef[i])/CmDm_I); 
// Explicit symmetric implementation of off diagonal terms
    for ( unsigned int j=0; j < i; ++j)
    {
	    _L_V(i,j) =  (1 -_f0_V)/(_f0_V) * ((*_C_elements[i])[_qp].value() * _element_vac_diffCoef[i] * (*_C_elements[j])[_qp].value() * _element_vac_diffCoef[j] *  _C_V[_qp].value())/(CmDm_V) ; 

	    _L_I(i,j) =  (1 -_f0_I)/(_f0_I) * ((*_C_elements[i])[_qp].value() * _element_int_diffCoef[i] * (*_C_elements[j])[_qp].value() * _element_int_diffCoef[j] *  _C_I[_qp].value())/( CmDm_I) ; 

	    _L_V(j, i) = _L_V(i, j);
	    _L_I(j, i) = _L_I(i, j);
	}
    }

}
void
RISFluxes::computeQpProperties()
{
    computeOnsagerCoefQp();

    for (unsigned int i = 0; i < _element_names.size(); ++i)
    {
	(*_element_vac_fluxes[i])[_qp].assign(LIBMESH_DIM, 0.0);
	(*_element_int_fluxes[i])[_qp].assign(LIBMESH_DIM, 0.0);
	for (unsigned  j = 0; j < _element_names.size(); ++j)
	    for (unsigned  k = 0; k < LIBMESH_DIM; ++k)
	{
    (*_element_vac_fluxes[i])[_qp][k] += _L_V(i, j) * ((*_grad_C_elements[j])[_qp](k).value() / (*_C_elements[j])[_qp].value() - _grad_C_V[_qp](k).value() / _C_V[_qp].value() ) ; 
    (*_element_int_fluxes[i])[_qp][k] += _L_V(i, j) * ((*_grad_C_elements[j])[_qp](k).value() / (*_C_elements[j])[_qp].value() + _grad_C_I[_qp](k).value() / _C_I[_qp].value() ) ; 
	}

}
}

