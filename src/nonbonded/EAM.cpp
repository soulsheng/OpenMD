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

#include <cstdio>
#include <cstring>
#include <cmath>

#include "nonbonded/EAM.hpp"
#include "utils/simError.h"
#include "types/EAMInteractionType.hpp"
#include "types/FluctuatingChargeAdapter.hpp"

namespace OpenMD {

  EAM::EAM() : initialized_(false), haveCutoffRadius_(false),
	       forceField_(NULL), electrostatic_(NULL), eamRcut_(0.0),
               mixMeth_(eamJohnson), name_("EAM") {}

  RealType EAM::fastPower(RealType x, int y) {
    RealType temp;
    if( y == 0)
      return 1;
    temp = fastPower(x, y/2);
    if (y%2 == 0)
      return temp*temp;
    else {
      if(y > 0)
        return x*temp*temp;
      else
        return (temp*temp)/x;
    }
  }

  RealType EAM::ZhouPhi(RealType r, RealType re,
                        RealType A, RealType B,
                        RealType alpha, RealType beta,
                        RealType kappa, RealType lambda) {
    return
      ( A*exp (-alpha * (r/re-1.0) ) )  /  (1.0 + fastPower(r/re-kappa, 20))
      - ( B*exp (-beta * (r/re-1.0) ) )  /  (1.0 + fastPower(r/re-lambda, 20));
  }

  RealType EAM::ZhouRho(RealType r, RealType re, RealType fe,
                        RealType beta, RealType lambda) {
    return (fe * exp(-beta * (r/re-1.0))) / (1.0 + fastPower(r/re-lambda, 20));
  }

  RealType EAM::Zhou2001Functional(RealType rho, RealType rhoe,
                                   std::vector<RealType> Fn,
                                   std::vector<RealType> F,
                                   RealType Fe, RealType eta) {
    RealType rhon = 0.85*rhoe;
    RealType rho0 = 1.15*rhoe;
    RealType functional(0.0);
    RealType t;
    if (rho < rhon) {
      t = rho/rhon - 1.0;
      functional =  Fn.at(0) + t * (Fn.at(1) + t * (Fn.at(2) + t * Fn.at(3)));
    } else if (rho >= rhon && rho < rho0) {
      t = rho/rhoe - 1.0;
      functional =  F.at(0) + t * (F.at(1) + t * (F.at(2) + t * F.at(3)));
    } else if (rho >= rho0) {
      t = rho/rhoe;
      functional = Fe*(1.0 - log( pow(t, eta) ) ) * pow(t, eta);
    }
    return functional;
  }

  RealType EAM::Zhou2004Functional(RealType rho, RealType rhoe, RealType rhos,
                                   std::vector<RealType> Fn,
                                   std::vector<RealType> F,
                                   RealType Fe, RealType eta,
                                   RealType rhol, RealType rhoh) {
    RealType rhon = rhol*rhoe;
    RealType rho0 = rhoh*rhoe;
    RealType functional(0.0);
    RealType t;
    if (rho < rhon) {
      t = rho/rhon - 1.0;
      functional =  Fn.at(0) + t * (Fn.at(1) + t * (Fn.at(2) + t * Fn.at(3)));
    } else if (rhon <= rho && rho < rho0) {
      t = rho/rhoe - 1.0;
      functional =  F.at(0) + t * (F.at(1) + t * (F.at(2) + t * F.at(3)));
    } else if (rho0 <= rho) {
      t = rho/rhos;
      functional = Fe*(1.0 - log( pow(t, eta) ) ) * pow(t, eta);
    }
    return functional;
  }

  RealType EAM::Zhou2005Functional(RealType rho, RealType rhoe, RealType rhos,
                                   std::vector<RealType> Fn,
                                   std::vector<RealType> F,
                                   RealType F3plus, RealType F3minus,
                                   RealType Fe, RealType eta) {
    RealType rhon = 0.85*rhoe;
    RealType rho0 = 1.15*rhoe;
    RealType functional(0.0);
    RealType t;
    if (rho < rhon) {
      t = rho/rhon - 1.0;
      functional =  Fn.at(0) + t * (Fn.at(1) + t * (Fn.at(2) + t * Fn.at(3)));
    } else if (rho >= rhon && rho < rhoe) {
      t = rho/rhoe - 1.0;
      functional =  F.at(0) + t * (F.at(1) + t * (F.at(2) + t * F3minus));
    } else if (rho >= rhoe && rho < rho0) {
      t = rho/rhoe - 1.0;
      functional =  F.at(0) + t * (F.at(1) + t * (F.at(2) + t * F3plus));
    } else if (rho >= rho0) {
      t = rho/rhos;
      functional = Fe*(1.0 - log( pow(t, eta) ) ) * pow(t, eta);
    }
    return functional;
  }

  RealType EAM::Zhou2005OxygenFunctional(RealType rho,
                                         std::vector<RealType> OrhoLimits,
                                         std::vector<RealType> OrhoE,
                                         std::vector<std::vector<RealType> > OF) {
    RealType functional(0.0);
    RealType t;

    if (rho < OrhoLimits[1]) {
      t = rho / OrhoE[0] - 1.0;
      functional = OF[0][0] + t * (OF[0][1] + t * (OF[0][2] + t * OF[0][3]));
    } else if (rho >= OrhoLimits[1] && rho < OrhoLimits[2]) {
      t = rho / OrhoE[1] - 1.0;
      functional = OF[1][0] + t * (OF[1][1] + t * OF[1][2]);
    } else if (rho >= OrhoLimits[2] && rho < OrhoLimits[3]) {
      t = rho / OrhoE[2] - 1.0;
      functional = OF[2][0] + t * (OF[2][1] + t * OF[2][2]);
    } else if (rho >= OrhoLimits[3] && rho < OrhoLimits[4]) {
      t = rho / OrhoE[3] - 1.0;
      functional = OF[3][0] + t * (OF[3][1] + t * OF[3][2]);
    } else if (rho >= OrhoLimits[4]) {
      t = rho / OrhoE[4] - 1.0;
      functional = OF[4][0] + t * (OF[4][1] + t * OF[4][2]);
    }
    return functional;
  }

  CubicSpline* EAM::getPhi(AtomType* atomType1, AtomType* atomType2) {
    EAMAdapter ea1 = EAMAdapter(atomType1);
    EAMAdapter ea2 = EAMAdapter(atomType2);
    CubicSpline* cs = new CubicSpline();

    RealType rha(0.0), rhb(0.0), pha(0.0), phb(0.0), phab(0.0);
    vector<RealType> rvals;
    vector<RealType> phivals;
    RealType rmax(0.0), dr, r;
    int nr;

    if (ea1.getEAMType()==eamFuncfl && ea2.getEAMType()==eamFuncfl) {

      CubicSpline* z1 = ea1.getZSpline();
      CubicSpline* z2 = ea2.getZSpline();
      CubicSpline* rho1 = ea1.getRhoSpline();
      CubicSpline* rho2 = ea2.getRhoSpline();

      // Thise prefactors convert the charge-charge interactions into
      // kcal / mol all were computed assuming distances are measured in
      // angstroms Charge-Charge, assuming charges are measured in
      // electrons.  Matches value in Electrostatics.cpp
      pre11_ = 332.0637778;

      // make the r grid:

      // we need phi out to the largest value we'll encounter in the
      // radial space;

      rmax = max(rmax, ea1.getRcut());
      rmax = max(rmax, ea1.getNr() * ea1.getDr());

      rmax = max(rmax, ea2.getRcut());
      rmax = max(rmax, ea2.getNr() * ea2.getDr());

      // use the smallest dr (finest grid) to build our grid:

      dr = min(ea1.getDr(), ea2.getDr());
      nr = int(rmax/dr + 0.5);

      for (int i = 0; i < nr; i++) rvals.push_back(RealType(i*dr));

      // construct the pair potential:

      RealType za, zb;

      phivals.push_back(0.0);

      for (unsigned int i = 1; i < rvals.size(); i++ ) {
        r = rvals[i];

        // only use z(r) if we're inside this atom's cutoff radius,
        // otherwise, we'll use zero for the charge.  This effectively
        // means that our phi grid goes out beyond the cutoff of the
        // pair potential

        rha = r <= ea1.getRcut() ? rho1->getValueAt(r) : 0.0;
        rhb = r <= ea2.getRcut() ? rho2->getValueAt(r) : 0.0;

        za = r <= ea1.getRcut() ? z1->getValueAt(r) : 0.0;
        zb = r <= ea2.getRcut() ? z2->getValueAt(r) : 0.0;

        switch(mixMeth_) {
        case eamJohnson:
          phab = 0.0;
          if ( rha > 0.0 ) {
            pha = pre11_ * (za * za) / r;
            phab = phab + 0.5 * (rhb / rha) * pha;
          }
          if ( rhb > 0.0 ) {
            phb = pre11_ * (zb * zb) / r;
            phab = phab + 0.5 * (rha / rhb) * phb;
          }

          break;

        case eamDaw:
          if (r <= ea1.getRcut() && r <= ea2.getRcut() ) {
            phab = pre11_ * (za * zb) / r;
          } else {
            phab = 0.0;
          }

          break;
        case eamUnknownMix:
        default:

          sprintf(painCave.errMsg,
                  "EAM::getPhi hit a mixing method it doesn't know about!\n"
                  );
          painCave.severity = OPENMD_ERROR;
          painCave.isFatal = 1;
          simError();
        }
        phivals.push_back(phab);
      }
      cs->addPoints(rvals, phivals);
    } else {
      EAMAtomData &data1 = EAMdata[EAMtids[ atomType1->getIdent() ]];
      EAMAtomData &data2 = EAMdata[EAMtids[ atomType2->getIdent() ]];

      RealType re1, A1, B1, alpha1, beta1, kappa1, lambda1;
      RealType re2, A2, B2, alpha2, beta2, kappa2, lambda2;


      re1 = ea1.getRe();
      A1 = ea1.getA();
      B1 = ea1.getB();
      alpha1 = ea1.getAlpha();
      beta1 = ea1.getBeta();
      kappa1 = ea1.getKappa();
      lambda1 = ea1.getLambda();

      re2 = ea2.getRe();
      A2 = ea2.getA();
      B2 = ea2.getB();
      alpha2 = ea2.getAlpha();
      beta2 = ea2.getBeta();
      kappa2 = ea2.getKappa();
      lambda2 = ea2.getLambda();

      RealType rmax = 0.0;
      rmax = max(rmax, data1.rcut);
      rmax = max(rmax, data2.rcut);

      // use the smallest dr (finest grid) to build our grid:

      RealType dr = min(data1.rho->getSpacing(), data2.rho->getSpacing());

      int nr = int(rmax/dr + 0.5);

      for (int i = 0; i < nr; i++) rvals.push_back(RealType(i*dr));

      vector<RealType> phivals;
      RealType r;

      phivals.push_back(0.0);

      for (unsigned int i = 1; i < rvals.size(); i++ ) {
        r = rvals[i];
        rha = 0.0;
        rhb = 0.0;
        pha = 0.0;
        phb = 0.0;
        phab = 0.0;

        // rcut values are derived parameters if no splines are pre-set:

        if ( r < data1.rcut ) {
          rha = data1.rho->getValueAt(r);
          pha = ZhouPhi(r, re1, A1, B1, alpha1, beta1, kappa1, lambda1);
        }
        if ( r < data2.rcut ) {
          rhb = data2.rho->getValueAt(r);
          phb = ZhouPhi(r, re2, A2, B2, alpha2, beta2, kappa2, lambda2);
        }

        if ( r < data1.rcut )
          phab = phab + 0.5 * (rhb / rha) * pha;
        if ( r < data2.rcut )
          phab = phab + 0.5 * (rha / rhb) * phb;

        phivals.push_back(phab);
      }
      cs->addPoints(rvals, phivals);
    }

    return cs;
  }

  void EAM::setCutoffRadius( RealType rCut ) {
    eamRcut_ = rCut;
    haveCutoffRadius_ = true;
  }

  void EAM::initialize() {
    // set up the mixing method:
    ForceFieldOptions& fopts = forceField_->getForceFieldOptions();
    string EAMMixMeth = fopts.getEAMMixingMethod();
    toUpper(EAMMixMeth);

    if (EAMMixMeth == "JOHNSON")
      mixMeth_ = eamJohnson;
    else if (EAMMixMeth == "DAW")
      mixMeth_ = eamDaw;
    else
      mixMeth_ = eamUnknownMix;

    // find all of the EAM atom Types:
    EAMtypes.clear();
    EAMtids.clear();
    EAMdata.clear();
    MixingMap.clear();
    nEAM_ = 0;

    EAMtids.resize( forceField_->getNAtomType(), -1);

    set<AtomType*>::iterator at;
    for (at = simTypes_.begin(); at != simTypes_.end(); ++at) {
      if ((*at)->isEAM()) nEAM_++;
    }
    EAMdata.resize(nEAM_);
    MixingMap.resize(nEAM_);

    for (at = simTypes_.begin(); at != simTypes_.end(); ++at) {
      if ((*at)->isEAM()) addType(*at);
    }

    // find all of the explicit EAM interactions:
    ForceField::NonBondedInteractionTypeContainer* nbiTypes = forceField_->getNonBondedInteractionTypes();
    ForceField::NonBondedInteractionTypeContainer::MapTypeIterator j;
    ForceField::NonBondedInteractionTypeContainer::KeyType keys;
    NonBondedInteractionType* nbt;

    for (nbt = nbiTypes->beginType(j); nbt != NULL;
         nbt = nbiTypes->nextType(j)) {

      if (nbt->isEAMTable() || nbt->isEAMZhou() ) {

        keys = nbiTypes->getKeys(j);
        AtomType* at1 = forceField_->getAtomType(keys[0]);
        if (at1 == NULL) {
          sprintf( painCave.errMsg,
                   "EAM::initialize could not find AtomType %s\n"
                   "\tfor %s - %s explicit interaction.\n",
                   keys[0].c_str(), keys[0].c_str(), keys[1].c_str());
          painCave.severity = OPENMD_ERROR;
          painCave.isFatal = 1;
          simError();
        }

        AtomType* at2 = forceField_->getAtomType(keys[1]);
        if (at2 == NULL) {
          sprintf( painCave.errMsg,
                   "EAM::initialize could not find AtomType %s\n"
                   "\tfor %s - %s explicit interaction.\n",
                   keys[1].c_str(), keys[0].c_str(), keys[1].c_str());
          painCave.severity = OPENMD_ERROR;
          painCave.isFatal = 1;
          simError();
        }

        EAMInteractionType* eamit = dynamic_cast<EAMInteractionType*>(nbt);

        if (eamit == NULL) {
          sprintf( painCave.errMsg,
                   "EAM::initialize could not convert NonBondedInteractionType\n"
                   "\tto EAMInteractionType for %s - %s interaction.\n",
                   at1->getName().c_str(),
                   at2->getName().c_str());
          painCave.severity = OPENMD_ERROR;
          painCave.isFatal = 1;
          simError();
        }

        if (nbt->isEAMTable()) {
          vector<RealType> phiAB = eamit->getPhi();
          RealType dr = eamit->getDr();
          int nr = eamit->getNr();

          addExplicitInteraction(at1, at2, dr, nr, phiAB);

        } else if (nbt->isEAMZhou()) {

          RealType re = eamit->getRe();
          RealType alpha = eamit->getAlpha();
          RealType beta = eamit->getBeta();
          RealType A = eamit->getA();
          RealType B = eamit->getB();
          RealType kappa = eamit->getKappa();
          RealType lambda = eamit->getLambda();

          addExplicitInteraction(at1, at2, re, alpha, beta, A, B,
                                 kappa, lambda);
        }
      }
    }
    initialized_ = true;
  }



  void EAM::addType(AtomType* atomType){

    EAMAdapter ea = EAMAdapter(atomType);
    EAMAtomData eamAtomData;

    switch(ea.getEAMType()) {
    case eamFuncfl: {
      eamAtomData.rho = ea.getRhoSpline();
      eamAtomData.F = ea.getFSpline();
      eamAtomData.Z = ea.getZSpline();
      eamAtomData.rcut = ea.getRcut();
      break;
    }
    case eamZhou2001:
    case eamZhou2004:
    case eamZhou2005: {
      EAMType et = ea.getEAMType();
      RealType re = ea.getRe();
      RealType fe = ea.get_fe();
      RealType rhoe = ea.getRhoe();
      RealType beta = ea.getBeta();
      RealType lambda = ea.getLambda();
      std::vector<RealType> Fn = ea.getFn();
      std::vector<RealType> F = ea.getF();
      RealType Fe = ea.getFe();
      RealType eta = ea.getEta();
      RealType latticeConstant = ea.getLatticeConstant();

      int Nr = 2000;
      eamAtomData.rcut = latticeConstant * sqrt(10.0) / 2.0;
      RealType dr = eamAtomData.rcut/(RealType)(Nr-1);
      RealType r;

      int Nrho = 2000;
      RealType rhomax = ZhouRho(0.0, re, fe, beta, lambda);
      RealType drho = rhomax/(RealType)(Nrho-1);
      RealType rho;

      std::vector<RealType> rvals;
      std::vector<RealType> zvals;
      std::vector<RealType> rhovals;
      std::vector<RealType> funcvals;

      for (int i = 0; i < Nr; i++) {
        r = RealType(i)*dr;
        rvals.push_back(r);
        rhovals.push_back( ZhouRho(r, re,  fe, beta,  lambda) );
      }
      eamAtomData.rho = new CubicSpline();
      eamAtomData.rho->addPoints(rvals, rhovals);

      rhovals.clear();
      if (et == eamZhou2001) {
        for (int i = 0; i < Nrho; i++) {
          rho = RealType(i)*drho;
          rhovals.push_back(rho);
          funcvals.push_back( Zhou2001Functional(rho, rhoe, Fn, F, Fe, eta) );
        }
      } else if (et == eamZhou2004) {
        RealType rhos = ea.getRhos();
        RealType rhol = ea.getRhol();
        RealType rhoh = ea.getRhoh();
        for (int i = 0; i < Nrho; i++) {
          rho = RealType(i)*drho;
          rhovals.push_back(rho);
          funcvals.push_back( Zhou2004Functional(rho, rhoe, rhos, Fn, F, Fe,
                                                 eta, rhol, rhoh) );
        }
      } else if (et == eamZhou2005) {
        RealType rhos = ea.getRhos();
        RealType F3plus = ea.getF3plus();
        RealType F3minus = ea.getF3minus();
        for (int i = 0; i < Nrho; i++) {
          rho = RealType(i)*drho;
          rhovals.push_back(rho);
          funcvals.push_back( Zhou2005Functional(rho, rhoe, rhos, Fn, F,
                                                 F3plus,  F3minus, Fe, eta) );
        }
      }

      eamAtomData.F = new CubicSpline();
      eamAtomData.F->addPoints(rhovals, funcvals);
      break;
    }
    case eamZhou2005Oxygen : {
      RealType re = ea.getRe();
      RealType fe = ea.get_fe();
      RealType gamma = ea.getGamma();
      RealType nu = ea.getNu();
      std::vector<RealType> OrhoLimits = ea.getOrhoLimits();
      std::vector<RealType> OrhoE = ea.getOrhoE();
      std::vector<std::vector<RealType> > OF = ea.getOF();

      int Nr = 2000;
      eamAtomData.rcut = 7.0;
      RealType dr = eamAtomData.rcut/(RealType)(Nr-1);
      RealType r;

      int Nrho = 2000;
      RealType rhomax = max(100.0, ZhouRho(0.0, re, fe, gamma, nu));
      RealType drho = rhomax/(RealType)(Nrho-1);
      RealType rho;

      std::vector<RealType> rvals;
      std::vector<RealType> zvals;
      std::vector<RealType> rhovals;
      std::vector<RealType> funcvals;

      for (int i = 0; i < Nr; i++) {
        r = RealType(i)*dr;
        rvals.push_back(r);
        rhovals.push_back( ZhouRho(r, re,  fe, gamma,  nu) );
      }
      eamAtomData.rho = new CubicSpline();
      eamAtomData.rho->addPoints(rvals, rhovals);

      rhovals.clear();
      for (int i = 0; i < Nrho; i++) {
        rho = RealType(i)*drho;
        rhovals.push_back(rho);
        funcvals.push_back( Zhou2005OxygenFunctional(rho, OrhoLimits, OrhoE,
                                                     OF) );
      }

      eamAtomData.F = new CubicSpline();
      eamAtomData.F->addPoints(rhovals, funcvals);
      break;
    }

    case eamUnknown: {
      sprintf( painCave.errMsg,
               "EAM::addType found an unknown EAM type for atomType %s\n",
               atomType->getName().c_str());
      painCave.severity = OPENMD_ERROR;
      painCave.isFatal = 1;
      simError();
      break;
    }
    }

    FluctuatingChargeAdapter fqa = FluctuatingChargeAdapter(atomType);
    if (fqa.isFluctuatingCharge()) {
      if (fqa.isMetallic()) {
        eamAtomData.isFluctuatingCharge = true;
        eamAtomData.nValence = fqa.getNValence();
      } else {
        eamAtomData.isFluctuatingCharge = false;
      }
    } else {
      eamAtomData.isFluctuatingCharge = false;
    }

    // add it to the map:
    int atid = atomType->getIdent();
    int eamtid = EAMtypes.size();

    pair<set<int>::iterator,bool> ret;
    ret = EAMtypes.insert( atid );
    if (ret.second == false) {
      sprintf( painCave.errMsg,
               "EAM already had a previous entry with ident %d\n",
               atid);
      painCave.severity = OPENMD_INFO;
      painCave.isFatal = 0;
      simError();
    }

    EAMtids[atid] = eamtid;
    EAMdata[eamtid] = eamAtomData;
    MixingMap[eamtid].resize(nEAM_);

    // Now, iterate over all known types and add to the mixing map:

    std::set<int>::iterator it;
    for( it = EAMtypes.begin(); it != EAMtypes.end(); ++it) {

      int eamtid2 = EAMtids[ (*it) ];
      AtomType* atype2 = forceField_->getAtomType( (*it) );

      EAMInteractionData mixer;
      mixer.phi = getPhi(atomType, atype2);
      mixer.rcut = mixer.phi->getLimits().second;
      mixer.explicitlySet = false;

      MixingMap[eamtid2].resize( nEAM_ );

      MixingMap[eamtid][eamtid2] = mixer;
      if (eamtid2 != eamtid) {
        MixingMap[eamtid2][eamtid] = mixer;
      }
    }
    return;
  }

  void EAM::addExplicitInteraction(AtomType* atype1, AtomType* atype2,
                                   RealType dr, int nr,
                                   vector<RealType> phiVals) {

    EAMInteractionData mixer;
    CubicSpline* cs = new CubicSpline();
    vector<RealType> rVals;

    for (int i = 0; i < nr; i++) rVals.push_back(i * dr);

    cs->addPoints(rVals, phiVals);
    mixer.phi = cs;
    mixer.rcut = mixer.phi->getLimits().second;
    mixer.explicitlySet = true;

    int atid1 = atype1->getIdent();
    int atid2 = atype2->getIdent();

    int eamtid1, eamtid2;

    pair<set<int>::iterator,bool> ret;
    ret = EAMtypes.insert( atid1 );
    if (ret.second == false) {
      // already had this type in the EAMMap, just get the eamtid:
      eamtid1 = EAMtids[ atid1 ];
    } else {
      // didn't already have it, so make a new one and assign it:
      eamtid1 = nEAM_;
      EAMtids[atid1] = nEAM_;
      nEAM_++;
    }

    ret = EAMtypes.insert( atid2 );
    if (ret.second == false) {
      // already had this type in the EAMMap, just get the eamtid:
      eamtid2 = EAMtids[ atid2 ];
    } else {
      // didn't already have it, so make a new one and assign it:
      eamtid2 = nEAM_;
      EAMtids[atid2] = nEAM_;
      nEAM_++;
    }

    MixingMap.resize(nEAM_);
    MixingMap[eamtid1].resize(nEAM_);
    MixingMap[eamtid1][eamtid2] = mixer;

    if (eamtid2 != eamtid1) {
      MixingMap[eamtid2].resize(nEAM_);
      MixingMap[eamtid2][eamtid1] = mixer;
    }
    return;
  }

  void EAM::addExplicitInteraction(AtomType* atype1, AtomType* atype2,
                                   RealType re, RealType alpha, RealType beta,
                                   RealType A, RealType B, RealType kappa,
                                   RealType lambda) {

    EAMInteractionData mixer;
    CubicSpline* cs = new CubicSpline();
    std::vector<RealType> rVals;
    std::vector<RealType> phiVals;

    int Nr = 2000;
    RealType r;

    // default to FCC if we don't specify HCP or BCC:
    RealType rcut = sqrt(5.0) * re;

    RealType dr = rcut/(RealType)(Nr-1);

    for (int i = 0; i < Nr; i++) {
      r = RealType(i)*dr;
      rVals.push_back(r);
      phiVals.push_back( ZhouPhi(r, re, A, B, alpha, beta,
                                 kappa, lambda) );
    }

    cs->addPoints(rVals, phiVals);
    mixer.phi = cs;
    mixer.rcut = mixer.phi->getLimits().second;
    mixer.explicitlySet = true;

    int atid1 = atype1->getIdent();
    int atid2 = atype2->getIdent();

    int eamtid1, eamtid2;

    pair<set<int>::iterator,bool> ret;
    ret = EAMtypes.insert( atid1 );
    if (ret.second == false) {
      // already had this type in the EAMMap, just get the eamtid:
      eamtid1 = EAMtids[ atid1 ];
    } else {
      // didn't already have it, so make a new one and assign it:
      eamtid1 = nEAM_;
      EAMtids[atid1] = nEAM_;
      nEAM_++;
    }

    ret = EAMtypes.insert( atid2 );
    if (ret.second == false) {
      // already had this type in the EAMMap, just get the eamtid:
      eamtid2 = EAMtids[ atid2 ];
    } else {
      // didn't already have it, so make a new one and assign it:
      eamtid2 = nEAM_;
      EAMtids[atid2] = nEAM_;
      nEAM_++;
    }

    MixingMap.resize(nEAM_);
    MixingMap[eamtid1].resize(nEAM_);
    MixingMap[eamtid1][eamtid2] = mixer;

    if (eamtid2 != eamtid1) {
      MixingMap[eamtid2].resize(nEAM_);
      MixingMap[eamtid2][eamtid1] = mixer;
    }
    return;
  }

  void EAM::calcDensity(InteractionData &idat) {

    if (!initialized_) initialize();

    EAMAtomData &data1 = EAMdata[EAMtids[idat.atid1]];
    EAMAtomData &data2 = EAMdata[EAMtids[idat.atid2]];
    RealType m;

    if (haveCutoffRadius_)
      if ( *(idat.rij) > eamRcut_) return;

    if ( *(idat.rij) < data1.rcut) {
      m = 1.0;
      if (data1.isFluctuatingCharge) {
        m -= *(idat.flucQ1) / RealType(data1.nValence);
      }
      *(idat.rho2) += m * data1.rho->getValueAt( *(idat.rij) );
    }

    if ( *(idat.rij) < data2.rcut) {
      m = 1.0;
      if (data2.isFluctuatingCharge) {
        m -= *(idat.flucQ2) / RealType(data2.nValence);
      }
      *(idat.rho1) += m * data2.rho->getValueAt( *(idat.rij));
    }

    return;
  }

  void EAM::calcFunctional(SelfData &sdat) {

    if (!initialized_) initialize();
    EAMAtomData &data1 = EAMdata[ EAMtids[sdat.atid] ];

    data1.F->getValueAndDerivativeAt( *(sdat.rho), *(sdat.frho),
                                      *(sdat.dfrhodrho) );

    (*(sdat.selfPot))[METALLIC_FAMILY] += *(sdat.frho);
    if (sdat.isSelected)
      (*(sdat.selePot))[METALLIC_FAMILY] += *(sdat.frho);

    if (sdat.doParticlePot) {
      *(sdat.particlePot) += *(sdat.frho);
    }

    return;
  }


  void EAM::calcForce(InteractionData &idat) {

    if (!initialized_) initialize();

    if (haveCutoffRadius_)
      if ( *(idat.rij) > eamRcut_) return;

    int eamtid1 = EAMtids[idat.atid1];
    int eamtid2 = EAMtids[idat.atid2];
    EAMAtomData &data1 = EAMdata[eamtid1];
    EAMAtomData &data2 = EAMdata[eamtid2];

    // get type-specific cutoff radii

    RealType rci = data1.rcut;
    RealType rcj = data2.rcut;

    RealType rha(0.0), drha(0.0), rhb(0.0), drhb(0.0);
    RealType pha(0.0), dpha(0.0), phb(0.0), dphb(0.0);

    RealType phab(0.0), dvpdr(0.0), Na(0.0), Nb(0.0), qa(0.0), qb(0.0);
    RealType va(1.0), vb(1.0);
    RealType drhoidr(0.0), drhojdr(0.0), dudr(0.0);
    RealType u, ui, up, uip;

    if (data1.isFluctuatingCharge) {
      Na = RealType(data1.nValence);
      qa = *(idat.flucQ1);
      va = (1.0 - qa / Na);
    }
    if (data2.isFluctuatingCharge) {
      Nb = RealType(data2.nValence);
      qb = *(idat.flucQ2);
      vb = (1.0 - qb / Nb);
    }

    rhat =  *(idat.d) / *(idat.rij);

    if ( *(idat.rij) < rci) {
      data1.rho->getValueAndDerivativeAt( *(idat.rij), rha, drha);
      CubicSpline* phi = MixingMap[eamtid1][eamtid1].phi;
      phi->getValueAndDerivativeAt( *(idat.rij), pha, dpha);
    }

    if ( *(idat.rij) < rcj) {
      data2.rho->getValueAndDerivativeAt( *(idat.rij), rhb, drhb );
      CubicSpline* phi = MixingMap[eamtid2][eamtid2].phi;
      phi->getValueAndDerivativeAt( *(idat.rij), phb, dphb);
    }

    switch(mixMeth_) {
    case eamJohnson:
      u = rha/rhb;
      up = drha/rhb - rha*drhb/(rhb*rhb);
      ui = rhb/rha;
      uip = -up / (u*u);

      if ( *(idat.rij) < rci ) {
        phab = phab + 0.5 * ui * (vb/va) * pha;
        dvpdr = dvpdr + 0.5 * (vb/va) * (ui * dpha + uip * pha);

        if (data1.isFluctuatingCharge) {
          *(idat.dVdFQ1) += 0.5 * (ui * vb * pha) / (Na * va * va);
        }
        if (data2.isFluctuatingCharge) {
          *(idat.dVdFQ2) -= 0.5 * (ui * pha) / (Nb*va);
        }
      }

      if ( *(idat.rij) < rcj) {
        phab = phab + 0.5 * u * (va/vb) * phb;
        dvpdr = dvpdr + 0.5 * (va/vb) * (u * dphb + up * phb);

        if (data1.isFluctuatingCharge) {
          *(idat.dVdFQ1) -= 0.5 * (u * phb) / (Na*vb);
        }
        if (data2.isFluctuatingCharge) {
          *(idat.dVdFQ2) += 0.5 * (u * va * phb) / (Nb * vb * vb);
        }
      }
      break;
    case eamDaw:
      if ( *(idat.rij) <  MixingMap[eamtid1][eamtid2].rcut) {
        MixingMap[eamtid1][eamtid2].phi->getValueAndDerivativeAt( *(idat.rij),
                                                                  phab, dvpdr);
      }
      break;
    case eamUnknownMix:
    default:

      sprintf(painCave.errMsg,
              "EAM::calcForce hit a mixing method it doesn't know about!\n"
              );
      painCave.severity = OPENMD_ERROR;
      painCave.isFatal = 1;
      simError();
    }

    if ( *(idat.rij) < rci) {
      data1.rho->getValueAndDerivativeAt( *(idat.rij), rha, drha);
    }

    if ( *(idat.rij) < rcj) {
      data2.rho->getValueAndDerivativeAt( *(idat.rij), rhb, drhb );
    }

    drhoidr = drha;
    drhojdr = drhb;

    dudr = drhojdr* *(idat.dfrho1) + drhoidr* *(idat.dfrho2) + dvpdr;

    *(idat.f1) += rhat * dudr;

    if (idat.doParticlePot) {
      // particlePot is the difference between the full potential and
      // the full potential without the presence of a particular
      // particle (atom1).
      //
      // This reduces the density at other particle locations, so we
      // need to recompute the density at atom2 assuming atom1 didn't
      // contribute.  This then requires recomputing the density
      // functional for atom2 as well.

      *(idat.particlePot1) += data2.F->getValueAt( *(idat.rho2) - rha )
        - *(idat.frho2);

      *(idat.particlePot2) += data1.F->getValueAt( *(idat.rho1) - rhb)
        - *(idat.frho1);
    }

    (*(idat.pot))[METALLIC_FAMILY] += phab;
    if (idat.isSelected)
      (*(idat.selePot))[METALLIC_FAMILY] += phab;

    *(idat.vpair) += phab;

    if (idat.doElectricField) {
      // Borrow the appropriate field code we're using in Electrostatic:
      RealType eff = electrostatic_->getFieldFunction( *(idat.rij) );

      if (data1.isFluctuatingCharge) {
        *(idat.eField2) += qa * eff * rhat;
      }
      if (data2.isFluctuatingCharge) {
        *(idat.eField1) += qb * eff * rhat;
      }
    }


    if (data1.isFluctuatingCharge) {
      *(idat.dVdFQ1) -= *(idat.dfrho2) * rha / RealType(data1.nValence);
    }
    if (data2.isFluctuatingCharge) {
      *(idat.dVdFQ2) -= *(idat.dfrho1) * rhb / RealType(data2.nValence);
    }

    return;
  }

  RealType EAM::getSuggestedCutoffRadius(pair<AtomType*, AtomType*> atypes) {
    if (!initialized_) initialize();

    RealType cut = 0.0;

    int atid1 = atypes.first->getIdent();
    int atid2 = atypes.second->getIdent();
    int eamtid1 = EAMtids[atid1];
    int eamtid2 = EAMtids[atid2];

    if (eamtid1 != -1) {
      EAMAtomData data1 = EAMdata[eamtid1];
      cut = data1.rcut;
    }

    if (eamtid2 != -1) {
      EAMAtomData data2 = EAMdata[eamtid2];
      if (data2.rcut > cut)
        cut = data2.rcut;
    }

    return cut;
  }

}
