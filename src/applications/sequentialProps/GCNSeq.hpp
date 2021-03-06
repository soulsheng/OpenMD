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
#ifndef APPLICATIONS_STATICPROPS_GCNSEQ_HPP
#define APPLICATIONS_STATICPROPS_GCNSEQ_HPP

#include <string>
#include <vector>
#include "applications/sequentialProps/SequentialAnalyzer.hpp"

namespace OpenMD {

  /**
   * @class GCNSeq
   * @brief Generalized Coordinate Number Sequence
   *
   * Computes a frame-by-frame sequence of distributions of
   * generalized coordinate numbers as described in:
   *
   *   "Finding optimal surface sites on heterogeneous catalysts by
   *    counting nearest neighbors," by F. Calle-Vallejo et al.,
   *    Science 350(6257) pp. 185-189 (2015).
   *    http://dx.doi.org/10.1126/science.aab3501
   *
   * Note that extra parameters mussed be declared: 
   *
   *   rCut = cutoff radius for finding lists of nearest neighbors
   *   sele1 = selection of StuntDoubles used for the GCN distribution
   *   sele2 = selection of StuntDoubles used for nearest neighbor computation
   */
  class GCNSeq : public SequentialAnalyzer {
    
  public:
    GCNSeq(SimInfo* info, const std::string& filename, const std::string& sele1,
           const std::string& sele2, RealType rCut, int bins);

    virtual ~GCNSeq();
    virtual void doFrame(int istep);
    virtual void writeSequence();
    
  private:

    RealType rCut_;    
    int bins_;
    
    bool usePBC_;    
    int nnMax_;
    RealType delta_;
    int selectionCount1_;
    int selectionCount2_;
    
    std::vector<int> count_;
    std::vector<std::vector<RealType> >  histogram_;
  };

}
#endif
