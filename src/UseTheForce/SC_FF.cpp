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
 *
 *  Created by Charles F. Vardeman II on 11/9/05.
 *  @author  Charles F. Vardeman II 
 *  @version $Id: SC_FF.cpp,v 1.12 2009-11-25 20:01:57 gezelter Exp $
 *
 */

#include "UseTheForce/SC_FF.hpp"
#include "UseTheForce/DarkSide/eam_interface.h"
#include "UseTheForce/DarkSide/lj_interface.h"
#include "UseTheForce/DarkSide/sticky_interface.h"
#include "UseTheForce/DarkSide/suttonchen_interface.h"
#include "UseTheForce/ForceFieldFactory.hpp"
#include "io/OptionSectionParser.hpp"
#include "io/BaseAtomTypesSectionParser.hpp"
#include "io/AtomTypesSectionParser.hpp"
#include "io/SCAtomTypesSectionParser.hpp"
#include "UseTheForce/ForceFieldCreator.hpp"
#include "utils/simError.h"
namespace OpenMD {
  
  SC_FF::SC_FF(){
    
    //set default force field filename
    setForceFieldFileName("SuttonChen.frc");
    spMan_.push_back(new OptionSectionParser(forceFieldOptions_));
    spMan_.push_back(new BaseAtomTypesSectionParser());
    spMan_.push_back(new AtomTypesSectionParser());
    spMan_.push_back(new SCAtomTypesSectionParser(forceFieldOptions_));
  }
  
  void SC_FF::parse(const std::string& filename) {
    ifstrstream* ffStream;
    ffStream = openForceFieldFile(filename);
    
    spMan_.parse(*ffStream, *this);
    
    ForceField::AtomTypeContainer::MapTypeIterator i;
    AtomType* at;
    
    // Set forcefield options
    
    for (at = atomTypeCont_.beginType(i); at != NULL; at = atomTypeCont_.nextType(i)) {
      // useBase sets the responsibilities, and these have to be done 
      // after the atomTypes and Base types have all been scanned:

      std::vector<AtomType*> ayb = at->allYourBase();      
      if (ayb.size() > 1) {
        for (int j = ayb.size()-1; j > 0; j--) {
          
          ayb[j-1]->useBase(ayb[j]);

        }
      }
      at->makeFortranAtomType();
    }
    
    
    for (at = atomTypeCont_.beginType(i); at != NULL; at = atomTypeCont_.nextType(i)) {
      at->complete();
    }
    
    delete ffStream;
  }
  
    
  SC_FF::~SC_FF(){
    // We need to clean up the fortran side so we don't have bad things happen if
    // we try to create a second SC force field.
    destroySCTypes();
  }
} //end namespace OpenMD

