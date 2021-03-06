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
 * [3]  Sun, Lin & Gezelter, J. Chem. Phys. 128, 234107 (2008).          
 * [4]  Kuang & Gezelter,  J. Chem. Phys. 133, 164101 (2010).
 * [5]  Vardeman, Stocker & Gezelter, J. Chem. Theory Comput. 7, 834 (2011).
 */
 
#include "io/GayBerneAtomTypesSectionParser.hpp"
#include "types/AtomType.hpp"
#include "types/GayBerneAdapter.hpp"
#include "brains/ForceField.hpp"
#include "utils/simError.h"


namespace OpenMD {
  
  GayBerneAtomTypesSectionParser::GayBerneAtomTypesSectionParser(ForceFieldOptions& options) : options_(options){
    setSectionName("GayBerneAtomTypes");
  }
  
  void GayBerneAtomTypesSectionParser::parseLine(ForceField& ff,const std::string& line, int lineNo){
    StringTokenizer tokenizer(line);
    int nTokens = tokenizer.countTokens();    
    //in GayBerneAtomTypesSection, a line at least contains 7 tokens
    //atomTypeName   d    l    eps_X  eps_S   eps_E   dw
    if (nTokens < 7)  {
      sprintf(painCave.errMsg, "GayBerneAtomTypesSectionParser: Not enough tokens at line %d\n"
              "\tPlease note that GB atoms now require separate specification of epsilon\n"
              "\tvalues for cross (X), Side-by-Side (S), and End-to-End (E) for each\n"
              "\tellipsoid type.\n",
              lineNo);
      painCave.isFatal = 1;
      simError();            
    } else {
      
      std::string atomTypeName = tokenizer.nextToken();    
      AtomType* atomType = ff.getAtomType(atomTypeName);
      if (atomType != NULL) {

        RealType GB_d = tokenizer.nextTokenAsDouble();
        RealType GB_l = tokenizer.nextTokenAsDouble();  
        RealType GB_eps_X = tokenizer.nextTokenAsDouble();
        RealType GB_eps_S = tokenizer.nextTokenAsDouble();
        RealType GB_eps_E = tokenizer.nextTokenAsDouble();
        RealType GB_dw = tokenizer.nextTokenAsDouble();

        GayBerneAdapter gba = GayBerneAdapter(atomType);
        gba.makeGayBerne(GB_d, GB_l, GB_eps_X, GB_eps_S, GB_eps_E, GB_dw);
        
      } else {
        sprintf(painCave.errMsg, "GayBerneAtomTypesSectionParser: Can not find matching AtomType %s\n"
                "\tfor this GayBerne atom type\n",
                atomTypeName.c_str());
        painCave.isFatal = 1;
        simError();         
      }                       
    }    
  }  
} //end namespace OpenMD
