/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef COMPUTEJACOBIANBLOCKSTHREAD_H
#define COMPUTEJACOBIANBLOCKSTHREAD_H

#include "ComputeFullJacobianThread.h"

// libMesh includes
#include "libmesh/elem_range.h"


class FEProblem;
class NonlinearSystem;

/**
 * Helper class for holding the preconditioning blocks to fill.
 */
class JacobianBlock
{
public:
  JacobianBlock(libMesh::System & precond_system, SparseMatrix<Number> & jacobian, unsigned int ivar, unsigned int jvar) :
      _precond_system(precond_system),
      _jacobian(jacobian),
      _ivar(ivar),
      _jvar(jvar)
    {}

  libMesh::System & _precond_system;
  SparseMatrix<Number> & _jacobian;
  unsigned int _ivar, _jvar;
};

/**
 * Specialization for filling multiple "small" preconditioning matrices simulatenously.
 */
class ComputeJacobianBlocksThread : public ComputeFullJacobianThread
{
public:
  ComputeJacobianBlocksThread(FEProblem & fe_problem, std::vector<JacobianBlock*> & blocks);

  // Splitting Constructor
  ComputeJacobianBlocksThread(ComputeJacobianBlocksThread & x, Threads::split split);

  virtual ~ComputeJacobianBlocksThread();

  void join(const ComputeJacobianThread & /*y*/)
  {}

protected:
  virtual void postElement(const Elem * elem);

  std::vector<JacobianBlock*> _blocks;
};

#endif //COMPUTEJACOBIANBLOCKSTHREAD_H
