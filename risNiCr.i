# Global Values
K0 =  7E1 # Radiation Dose rate 
eps = 20.0 # Damage Efficiency
Riv = 5E-2 # Recombination Rate
Ni_conc = 0.82
Cr_conc = 0.18
Vac_conc = 1E-10
Int_conc = 1E-15
[Mesh]
  [rectangle]
    type = GeneratedMeshGenerator
    dim = 1
    nx = 1000
    #ny = 80
    #elem_type = EDGE3 #Quad
    xmax = 7E-4
    xmin = 0
    #bias_x = 0.6
    #second_order = true
  []
[]

[Variables]
  [C_Vac]
    order = FIRST
    family = LAGRANGE
    initial_condition = ${Vac_conc}
  []
  [C_Int]
    order = FIRST
    family = LAGRANGE
    initial_condition = ${Int_conc}
  []
  [C_Ni]
    order = FIRST
    family = LAGRANGE
    initial_condition = ${Ni_conc}
  []
  [C_Cr]
    order = FIRST
    family = LAGRANGE
    initial_condition = ${Cr_conc}
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
  [C_Ni_dot]
    type = ADTimeDerivative
    variable = C_Ni
  []
  [C_Cr_dot]
    type = ADTimeDerivative
    variable = C_Cr
  []
  [Vac_diff]
    type = ADRISConserveDefect
    variable = C_Vac
    defect_type = 'vacancy'
    element_names = 'Ni Cr'
  []
  [Int_diff]
    type = ADRISConserveDefect
    variable = C_Int
    defect_type = 'interstitial'
    element_names = 'Ni Cr'
  []
  [Ni_diff]
    type = ADRISConserveElement
    variable = C_Ni
    element_name = 'Ni'
  []
  [Cr_diff]
    type = ADRISConserveElement
    variable = C_Cr
    element_name = 'Cr'
  []
  [Vac_recomb]
    type = ADRecombination
    variable = C_Vac
    recombination_rate = ${Riv}
    defect_conjugate = C_Int
  []
  [Int_recomb]
    type = ADRecombination
    variable = C_Int
    recombination_rate = ${Riv}
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
  [Vac_sink_gb]
    type = ADDefectFluxBC
    variable = C_Vac
    defect_type = 'vacancy'
    element_names = 'Ni Cr'
    boundary = 'right'
  []
  [Int_sink_gb]
    type = ADDefectFluxBC
    variable = C_Int
    defect_type = 'interstitial'
    element_names = 'Ni Cr'
    boundary = 'right'
  []
[]

[Materials]
  [radiation_fluxes]
    type = RISFluxes
    element_names = 'Ni Cr'
    element_int_diffCoef = '2E5 1.3E5'
    element_vac_diffCoef = '1.3E5 4.75E5'
    vacancy_concentration = C_Vac
    interstitial_concentration = C_Int
    element_concentration = 'C_Ni C_Cr'
    corr_factor_vac = 0.78
    corr_factor_int = 0.44
  []
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  start_time = 0.0
  end_time = 1.0
  dt = 1E-5
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
