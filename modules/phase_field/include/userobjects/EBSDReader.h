/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef EBSDREADER_H
#define EBSDREADER_H

#include "EulerAngleProvider.h"
#include "EBSDAccessFunctors.h"

class EBSDReader;

template<>
InputParameters validParams<EBSDReader>();

/**
 * A GeneralUserObject that reads an EBSD file and stores the centroid
 * data in a data structure which indexes on element centroids.
 */
class EBSDReader : public EulerAngleProvider, public EBSDAccessFunctors
{
public:
  EBSDReader(const InputParameters & params);
  virtual ~EBSDReader();

  virtual void readFile();

  /**
   * Called before execute() is ever called so that data can be cleared.
   */
  virtual void initialize() {}

  /**
   * Called when this object needs to compute something.
   */
  virtual void execute() {}

  /**
   * Called _after_ execute(), could be used to do MPI communictaion.
   */
  virtual void finalize() {}

  /**
   * Get the requested type of data at the point p.
   */
  const EBSDPointData & getData(const Point & p) const;

  /**
   * Get the requested type of average data for feature number i.
   */
  const EBSDAvgData &  getAvgData(unsigned int i) const;

  /**
   * Get the requested type of average data for a given phase and grain.
   */
  const EBSDAvgData &  getAvgData(unsigned int phase, unsigned int grain) const;

  /**
   * EulerAngleProvider interface implementation to fetch a triplet of Euler angles
   */
  virtual const EulerAngles & getEulerAngles(unsigned int) const;

  /**
   * Return the total number of grains
   */
  virtual unsigned int getGrainNum() const;

  /**
   * Return the number of grains in a given phase
   */
  unsigned int getGrainNum(unsigned int phase) const;

  /// Factory function to return a point functor specified by name
  EBSDPointDataFunctor * getPointDataAccessFunctor(const MooseEnum & field_name) const;
  /// Factory function to return a average functor specified by name
  EBSDAvgDataFunctor * getAvgDataAccessFunctor(const MooseEnum & field_name) const;

  /**
   * Creates a map consisting of the node index followd by
   * a vector of all grain weights for that node
   */
  const std::map<dof_id_type, std::vector<Real> > & getNodeToGrainWeightMap() const;

protected:
  // MooseMesh Variables
  MooseMesh & _mesh;
  NonlinearSystem & _nl;

  /// Variables needed to determine reduced order parameter values
  unsigned int _feature_num;
  Point _bottom_left;
  Point _top_right;
  Point _range;

  /// number of additional custom data columns
  unsigned int _custom_columns;

  /// Logically three-dimensional data indexed by geometric points in a 1D vector
  std::vector<EBSDPointData> _data;

  /// Averages by feature ID
  std::vector<EBSDAvgData> _avg_data;

  /// Euler Angles by feature ID
  std::vector<EulerAngles> _avg_angles;

  /// feature ID for given phases and grains
  std::vector<std::vector<unsigned int> > _feature_id;

  /// Map of grain weights per node
  std::map<dof_id_type, std::vector<Real> > _node_to_grn_weight_map;

  /// Dimension of the problem domain
  unsigned int _mesh_dimension;

  /// The number of values in the x, y and z directions.
  unsigned _nx, _ny, _nz;

  /// The spacing of the values in x, y and z directions.
  Real _dx, _dy, _dz;

  /// Grid origin
  Real _minx, _miny, _minz;

  /// Maximum grid extent
  Real _maxx, _maxy, _maxz;

  /// Computes a global index in the _data array given an input *centroid* point
  unsigned indexFromPoint(const Point & p) const;

  /// Transfer the index into the _avg_data array from given index
  unsigned indexFromIndex(unsigned int var) const;

  /// Build map
  void buildNodeToGrainWeightMap();
};

#endif // EBSDREADER_H
