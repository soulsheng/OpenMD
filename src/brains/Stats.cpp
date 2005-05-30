/*
 * Copyright (c) 2005 The University of Notre Dame. All Rights Reserved.
 *
 * The University of Notre Dame grants you ("Licensee") a
 * non-exclusive, royalty free, license to use, modify and
 * redistribute this software in source and binary code form, provided
 * that the following conditions are met:
 *
 * 1. Acknowledgement of the program authors must be made in any
 *    publication of scientific results based in part on use of the
 *    program.  An acceptable form of acknowledgement is citation of
 *    the article in which the program was described (Matthew
 *    A. Meineke, Charles F. Vardeman II, Teng Lin, Christopher
 *    J. Fennell and J. Daniel Gezelter, "OOPSE: An Object-Oriented
 *    Parallel Simulation Engine for Molecular Dynamics,"
 *    J. Comput. Chem. 26, pp. 252-271 (2005))
 *
 * 2. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 3. Redistributions in binary form must reproduce the above copyright
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
 */
  
/**
 * @file Stats.cpp
 * @author tlin
 * @date 11/04/2004
 * @time 14:26am
 * @version 1.0
 */

#include "brains/Stats.hpp"

namespace oopse {

  bool Stats::isInit_ = false;
  std::string Stats::title_[Stats::ENDINDEX - Stats::BEGININDEX];
  std::string Stats::units_[Stats::ENDINDEX - Stats::BEGININDEX];

  Stats::Stats() {

    if (!isInit_) {
      init();
      isInit_ = true;
    }

  }

  void Stats::init() {

    Stats::title_[TIME] = "Time";
    Stats::title_[TOTAL_ENERGY] = "Total Energy";
    Stats::title_[POTENTIAL_ENERGY] = "Potential Energy";
    Stats::title_[KINETIC_ENERGY] = "Kinetic Energy";
    Stats::title_[TEMPERATURE] = "Temperature";
    Stats::title_[PRESSURE] = "Pressure";
    Stats::title_[VOLUME] = "Volume";
    Stats::title_[CONSERVED_QUANTITY] = "Conserved Quantity";             
    Stats::title_[TRANSLATIONAL_KINETIC] = "Translational Kinetic";
    Stats::title_[ROTATIONAL_KINETIC] = "Rotational Kinetic";
    Stats::title_[LONG_RANGE_POTENTIAL] = "Long Range Potential";
    Stats::title_[SHORT_RANGE_POTENTIAL] = "Short Range Potential";
    Stats::title_[VANDERWAALS_POTENTIAL] = "van der waals Potential";
    Stats::title_[BOND_POTENTIAL] = "Bond Potential";
    Stats::title_[BEND_POTENTIAL] = "Bend Potential";
    Stats::title_[DIHEDRAL_POTENTIAL] = "Dihedral Potential";
    Stats::title_[IMPROPER_POTENTIAL] = "Improper Potential";
    Stats::title_[VRAW] = "Raw Potential";
    Stats::title_[VHARM] = "Harmonic Potential";
    Stats::title_[PRESSURE_TENSOR_X] = "presure tensor x";
    Stats::title_[PRESSURE_TENSOR_Y] = "presure tensor y";
    Stats::title_[PRESSURE_TENSOR_Z] = "presure tensor z";

    Stats::units_[TIME] = "fs";
    Stats::units_[TOTAL_ENERGY] = "kcal/mol";
    Stats::units_[POTENTIAL_ENERGY] = "kcal/mol";
    Stats::units_[KINETIC_ENERGY] = "kcal/mol";
    Stats::units_[TEMPERATURE] = "K";
    Stats::units_[PRESSURE] = "atm";
    Stats::units_[VOLUME] = "A^3";
    Stats::units_[CONSERVED_QUANTITY] = "kcal/mol";             
    Stats::units_[TRANSLATIONAL_KINETIC] = "kcal/mol";
    Stats::units_[ROTATIONAL_KINETIC] = "kcal/mol";
    Stats::units_[LONG_RANGE_POTENTIAL] = "kcal/mol";
    Stats::units_[SHORT_RANGE_POTENTIAL] = "kcal/mol";
    Stats::units_[VANDERWAALS_POTENTIAL] = "kcal/mol";
    Stats::units_[BOND_POTENTIAL] = "kcal/mol";
    Stats::units_[BEND_POTENTIAL] = "kcal/mol";
    Stats::units_[DIHEDRAL_POTENTIAL] = "kcal/mol";
    Stats::units_[IMPROPER_POTENTIAL] = "kcal/mol";
    Stats::units_[VRAW] = "kcal/mol";
    Stats::units_[VHARM] = "kcal/mol";
    Stats::units_[PRESSURE_TENSOR_X] = "amu*fs^-2*Ang^-1";
    Stats::units_[PRESSURE_TENSOR_Y] = "amu*fs^-2*Ang^-1";
    Stats::units_[PRESSURE_TENSOR_Z] = "amu*fs^-2*Ang^-1";
  }

}
