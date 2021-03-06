/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef NSMOMENTUMPRESSUREWEAKSTAGNATIONBC_H
#define NSMOMENTUMPRESSUREWEAKSTAGNATIONBC_H

#include "NSWeakStagnationBC.h"

// Forward Declarations
class NSMomentumPressureWeakStagnationBC;

template<>
InputParameters validParams<NSMomentumPressureWeakStagnationBC>();


/**
 * This class implements the pressure term of the momentum
 * equation boundary integral for use in weak stagnation
 * boundary conditions.
 */
class NSMomentumPressureWeakStagnationBC : public NSWeakStagnationBC
{

public:
  NSMomentumPressureWeakStagnationBC(const InputParameters & parameters);

  virtual ~NSMomentumPressureWeakStagnationBC(){}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned jvar);

  // Required parameters
  unsigned _component;
};

#endif // NSMOMENTUMPRESSUREWEAKSTAGNATIONBC_H
