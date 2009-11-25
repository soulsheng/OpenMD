/*
 * Copyright (c) 2005 The University of Notre Dame. All Rights Reserved.
 *
 * The University of Notre Dame grants you ("Licensee") a
 * non-exclusive, royalty free, license to use, modify and
 * redistribute this software in source and binary code form, provided
 * that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 * This software is provided "AS IS," without a warranty of any
 * kind. All express or implied conditions, representations and
 * warranties, including any implied warranty of merchantability,
 * fitness for a particular purpose or non-infringement, are hereby
 * excluded.  The University of Notre Dame and its licensors shall not
 * be liable for any damages suffered by licensee as a result of
 * using, modifying or distributing the software or its
 * derivatives. In no event will the University of Notre Dame or its
 * licensors be liable for any lost revenue, profit or data, or for
 * direct, indirect, special, consequential, incidental or punitive
 * damages, however caused and regardless of the theory of liability,
 * arising out of the use of or inability to use software, even if the
 * University of Notre Dame has been advised of the possibility of
 * such damages.
 *
 * SUPPORT OPEN SCIENCE!  If you use OpenMD or its source code in your
 * research, please cite the appropriate papers when you publish your
 * work.  Good starting points are:
 *                                                                      
 * [1]  Meineke, et al., J. Comp. Chem. 26, 252-271 (2005).             
 * [2]  Fennell & Gezelter, J. Chem. Phys. 124, 234104 (2006).          
 * [3]  Sun, Lin & Gezelter, J. Chem. Phys. 128, 24107 (2008).          
 * [4]  Vardeman & Gezelter, in progress (2009).                        
 */
 
#ifndef TYPES_DIRECTIONALATOMTYPE_HPP
#define TYPES_DIRECTIONALATOMTYPE_HPP

#include "types/AtomType.hpp"
#include "math/SquareMatrix3.hpp"

namespace OpenMD {
 
  /**
   * @class DirectionalAtomType 
   *
   * DirectionalAtomType is what OpenMD looks to for unchanging data
   * about a directional atoms. 
   */
  class DirectionalAtomType : public AtomType {
    
  public:
    
    DirectionalAtomType();

    void copyAllData(AtomType* at);

    virtual ~DirectionalAtomType() { } ;

    Mat3x3d getI() {return I;}
    
    void    setI(Mat3x3d theI) {I = theI;}
    
    RotMat3x3d getElectroBodyFrame() {
      return electroBodyFrame_;
    }
    
    void setElectroBodyFrame(const RotMat3x3d& electroBodyFrame) {
      electroBodyFrame_ =electroBodyFrame;
    }
    
    void setDipole();
    void setSplitDipole();
    void setQuadrupole();
    void setGayBerne();
    void setSticky();
    void setStickyPower();
    void setShape();
    
    virtual void complete();
    
  private:
    
    Mat3x3d I;
    RotMat3x3d electroBodyFrame_;
  };
  
  
  struct StickyParam {
    RealType w0;
    RealType v0;
    RealType v0p;
    RealType rl;
    RealType ru;
    RealType rlp;
    RealType rup;
  };
  
  struct GayBerneParam{
    RealType GB_d;
    RealType GB_l;
    RealType GB_eps;
    RealType GB_eps_ratio;
    RealType GB_dw;
  };
  
  typedef SimpleTypeData<StickyParam> StickyParamGenericData;
  
  typedef SimpleTypeData<GayBerneParam> GayBerneParamGenericData;
  
  typedef SimpleTypeData<Vector3d> Vector3dGenericData;
  
}
#endif
