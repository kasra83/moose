# 
# Example problem showing how to use the DerivativeParsedMaterial with SplitCHParsed.
# The free energy is identical to that from SplitCHMath, f_bulk = 1/4*(1-c)^2*(1+c)^2.
# 

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 100
  xmax = 60
  ymax = 60
  elem_type = QUAD4
[]

[Variables]
  [./c]
  [../]
  [./chem_pot]
  [../]
[]

[AuxVariables]
  [./local_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[ICs]
  [./cIC]
    type = RandomIC
    variable = c
    max = 0.2
    block = 0
  [../]
[]

[Kernels]
  [./c_dot]
    type = CoupledTimeDerivative
    variable = chem_pot
    v = c
  [../]
  [./c_res]
    type = SplitCHParsed
    variable = c
    f_name = fbulk
    kappa_name = kappa_c
    w = chem_pot
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = chem_pot
    mob_name = M
  [../]
[]

[AuxKernels]
  [./local_energy]
    type = TotalFreeEnergy
    variable = local_energy
    f_name = fbulk
    interfacial_vars = c
    kappa_names = kappa_c
    execute_on = timestep_end
  [../]
[]

[BCs]
  [./NBC]
    type = NeumannBC
    variable = c
    boundary = '0 1 2 3'
  [../]
[]

[Materials]
  [./mat]
    type = GenericConstantMaterial
    prop_names = 'M kappa_c'
    prop_values = '1.0 0.75'
    block = 0
  [../]
  [./free_energy]
    type = DerivativeParsedMaterial
    block = 0
    f_name = fbulk
    args = c
    constant_names = W
    constant_expressions = 1.0/2^2
    function = c*(1-c)+0.1*(c*plog(c,1e-4)+(1-c)*plog(1-c,1e-4))
    outputs = exodus
  [../]
[]

[Postprocessors]
  [./top]
    type = SideIntegralVariablePostprocessor
    variable = c
    boundary = top
  [../]
  [./total_free_energy]
    type = ElementIntegralVariablePostprocessor
    variable = local_energy
  [../]
  [./Concentratio]
    type = ElementIntegralVariablePostprocessor
    variable = c
    block = 0
  [../]
[]

[Preconditioning]
  [./cw_coupling]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  # petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  # petsc_options_value = 'hypre boomeramg 31'
  type = Transient
  scheme = bdf2
  dt = 2.0
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   lu      1'
  l_max_its = 30
  l_tol = 1e-4
  nl_max_its = 20
  nl_rel_tol = 1e-9
  end_time = 20
[]

[Outputs]
  exodus = true
[]

