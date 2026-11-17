# Global Values
K0 =  7E-6 # Radiation Dose rate 
eps = 20.0 # Damage Efficiency
[Mesh]
  [rectangle]
    type = GeneratedMeshGenerator
    dim = 1
    nx = 1000
    #ny = 80
    #elem_type = Quad
    xmax = 7E-5
    xmin = 0
    #ymax = 1E-6
    #ymin = 0 
  []
[]

[Variables]
  [C_Vac]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0.5
  []
  [C_Int]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0.5
  []
  [C_Fe]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0.75
  []
  [C_Cr]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0.18
  []
  [C_Ni]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0.05
  []
  [C_Si]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0.02
  []
[]

[Kernels]
  [C_Vac_dot]
    type = ADTimeDerivative
    variable = C_Vac
  []
  [C_Int_dot]
    type = ADTimeDerivative
    variable = C_Int
  []
  [C_Fe_dot]
    type = ADTimeDerivative
    variable = C_Fe
  []
  [C_Cr_dot]
    type = ADTimeDerivative
    variable = C_Cr
  []
  [C_Ni_dot]
    type = ADTimeDerivative
    variable = C_Ni
  []
  [C_Si_dot]
    type = ADTimeDerivative
    variable = C_Si
  []
  [Vac_diff]
    type = ADRISConserveDefect
    variable = C_Vac
    defect_type = 'vacancy'
    element_names = 'Fe Cr Ni Si'
  []
  [Int_diff]
    type = ADRISConserveDefect
    variable = C_Int
    defect_type = 'interstitial'
    element_names = 'Fe Cr Ni Si'
  []
  [Fe_diff]
    type = ADRISConserveElement
    variable = C_Fe
    element_name = 'Fe'
  []
  [Cr_diff]
    type = ADRISConserveElement
    variable = C_Cr
    element_name = 'Cr'
  []
  [Ni_diff]
    type = ADRISConserveElement
    variable = C_Ni
    element_name = 'Ni'
  []
  [Si_diff]
    type = ADRISConserveElement
    variable = C_Si
    element_name = 'Si'
  []
  [Vac_recomb]
    type = ADRecombination
    variable = C_Vac
    recombination_rate = 0.5
    defect_conjugate = C_Int
  []
  [Int_recomb]
    type = ADRecombination
    variable = C_Int
    recombination_rate = 0.5
    defect_conjugate = C_Vac
  []
  [Vac_source]
    type = ADBodyForce
    variable = C_Vac
    value = ${fparse K0 * eps / 100 }
  []
  [Int_source]
    type = ADBodyForce
    variable = C_Int
    value = ${fparse K0 * eps / 100 }
  []
[]

[BCs]
  [Vac_fluxes]
    type = ADDefectFluxBC
    variable = C_Vac
    defect_type = 'vacancy'
    element_names = 'Fe Cr Ni Si'
    boundary = 'right'
  []
  [Int_fluxes]
    type = ADDefectFluxBC
    variable = C_Int
    defect_type = 'interstitial'
    element_names = 'Fe Cr Ni Si'
    boundary = 'right'
  []
[]

[Materials]
  [radiation_fluxes]
    type = RISFluxes
    element_names = 'Fe Cr Ni Si'
    element_int_diffCoef = '2E5 2E5 2E5 4.3E5'
    element_vac_diffCoef = '1.5E5 1.75E5 1.3E5 1.7E5'
    vacancy_concentration = C_Vac
    interstitial_concentration = C_Int
    element_concentration = 'C_Fe C_Cr C_Ni C_Si'
    corr_factor_vac = 0.78
    corr_factor_int = 0.44
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  start_time = 0.0
  end_time = 1.0
  dt = 1E-2
  dtmin = 1E-25
  nl_abs_tol = 1e-8
  nl_rel_tol = 1e-8
[]

[Debug]
  show_material_props = true
[]

[Outputs]
  execute_on = 'timestep_end'
  exodus = true
[]
