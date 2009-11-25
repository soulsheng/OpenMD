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
 
/**
 * @file MoleculeCreator.hpp
 * @author tlin
 * @date 11/04/2004
 * @time 13:51am
 * @version 1.0
 */

#ifndef BRAINS_MOLECULECREATOR_HPP
#define BRAINS_MOLECULECREATOR_HPP

#include "brains/SimInfo.hpp"
#include "types/AtomStamp.hpp"
#include "types/BondStamp.hpp"
#include "types/BendStamp.hpp"
#include "types/CutoffGroupStamp.hpp"
#include "types/RigidBodyStamp.hpp"
#include "types/TorsionStamp.hpp"
#include "types/InversionStamp.hpp"
#include "primitives/Molecule.hpp"
namespace OpenMD {

  /**
   * @class MoleculeCreator MoleculeCreator.hpp "brains/MoleculeCreator.hpp"
   * @brief
   */
  class MoleculeCreator {
  public:
    virtual Molecule* createMolecule(ForceField* ff, MoleculeStamp *molStamp,
				     int stampId, int globalIndex,  
                                     LocalIndexManager* localIndexMan);

  protected:
        
    /** Create an atom by its stamp */
    virtual Atom* createAtom(ForceField* ff, Molecule* mol, AtomStamp* stamp, 
			     LocalIndexManager* localIndexMan);
    virtual RigidBody* createRigidBody(MoleculeStamp *molStamp, Molecule* mol, 
				       RigidBodyStamp* rbStamp,  
                                       LocalIndexManager* localIndexMan); 
    virtual Bond* createBond(ForceField* ff, Molecule* mol, BondStamp* stamp);
    virtual Bend* createBend(ForceField* ff, Molecule* mol, BendStamp* stamp);
    virtual Torsion* createTorsion(ForceField* ff, Molecule* mol, 
                                   TorsionStamp* stamp);
    virtual Inversion* createInversion(ForceField* ff, Molecule* mol, 
                                       InversionStamp* stamp);
    virtual CutoffGroup* createCutoffGroup(Molecule* mol, 
                                           CutoffGroupStamp* stamp);
    virtual CutoffGroup* createCutoffGroup(Molecule * mol, Atom* atom);
    virtual void createConstraintPair(Molecule* mol);     
    virtual void createConstraintElem(Molecule* mol);
  };


}
#endif //BRAINS_MOLECULECREATOR_HPP
