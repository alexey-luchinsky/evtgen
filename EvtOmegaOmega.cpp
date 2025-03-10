// test
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenModels/EvtOmegaOmega.hh"
#include "EvtGenBase/EvtDiracSpinor.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include <EvtGammaMatrix.hh>

#include "EvtGenBase/EvtComplex.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtVector3R.hh"


EvtOmegaOmega::~EvtOmegaOmega() {}

std::string EvtOmegaOmega::getName(){

  return "OMEGAOMEGA";     

}


EvtDecayBase* EvtOmegaOmega::clone(){

  return new EvtOmegaOmega;

}

void EvtOmegaOmega::init(){

  // check that there are 0 arguments
  checkNArg(0);
  checkNDaug(3);

  checkSpinParent(EvtSpinType::DIRAC);

  checkSpinDaughter(0,EvtSpinType::DIRAC);
  checkSpinDaughter(1,EvtSpinType::DIRAC);
  checkSpinDaughter(2,EvtSpinType::NEUTRINO);
}

void EvtOmegaOmega::initProbMax(){

  //setProbMax(650.0);

}



EvtVector4C EvtOmegaOmega::EvtSigmaCurrent(const EvtDiracSpinor &d, const EvtDiracSpinor &dp) 	
{
	EvtVector4C temp;

	//Здесь идет код для VCurrent. Его нужно изменить
 
  	temp.set(0,d*(EvtGammaMatrix::v0()*dp));
	temp.set(1,d*(EvtGammaMatrix::v1()*dp));
  	temp.set(2,d*(EvtGammaMatrix::v2()*dp));
 	temp.set(3,d*(EvtGammaMatrix::v3()*dp));
   
   	return temp;
}

void EvtOmegaOmega::HadronicAmp( EvtParticle* parent, 
                                 EvtParticle* child, 
                                 EvtVector4C* T,
                                 const int i, 
                                 const int j )
{
	const EvtDiracSpinor Sfinal = child->spParent(i);
 	const EvtDiracSpinor Sinit  = parent->sp(j); 
	 // \bar{u} \gamma^{\mu} u
  	T[0] = EvtLeptonVCurrent( Sfinal, Sinit );

  	// \bar{u} \gamma^{\mu}\gamma^{5} u
  	T[1] = EvtLeptonACurrent( Sfinal, Sinit );
  
  	// \bar{u} Sigma u
  	T[2] = EvtSigmaCurrent( Sfinal, Sinit );
  
  	// \bar{u} Sigma \gamma^{5} u
  	T[3] = EvtSigmaCurrent( Sfinal, Sinit );
  
  	return;
}

void EvtOmegaOmega::decay(EvtParticle *b1){
  static EvtId TAUM=EvtPDL::getId("tau-");

  b1->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *b2, *l, *nul;

	b2=b1->getDaug(0);
  	l = b1->getDaug(1);
  	nul = b1->getDaug(2);

	const EvtVector4R pb1 = b1->getP4lab();
  	const EvtVector4R pb2 = b2->getP4lab();
	EvtVector4R q=pb1-pb2;

	const double f1 = 1/(1+q*q);
	const double g1 = 1;
	const double f2 = 1;
	const double g2 = 1;

	EvtVector4C H[2][2]; // vector current
	EvtVector4C T[6];
    	// Hadronic current
    	for ( int i =0 ; i < 2; ++i ){
       		for ( int j = 0; j < 2; ++j ){
         	HadronicAmp( b1, b2, T, i, j );
         
         	H[i][j] = ( f1*T[0] - g1*T[1] - f2*T[2] + g2*T[3]);
         
       		}
    	}
	//Leptonic current
	EvtVector4C l[2];
	l[0]=EvtLeptonVACurrent(l->spParent(0),nul->spParentNeutrino());
    	l[1]=EvtLeptonVACurrent(l->spParent(1),nul->spParentNeutrino());

	vertex(0,0,0, H[0][0]*l[0]);

	
/*	for ( int i =0 ; i < 2; ++i ){
     //  		for ( int j = 0; j < 2; ++j ){
        	                
   //Тут нужно суммировать три раза?      
       		}
    	}

*/
 /*   
  EvtVector4C l1, l2, tau1, tau2;

 if (p->getId()==TAUM) {

    tau1=EvtLeptonVACurrent(nut->spParentNeutrino(),p->sp(0));
    tau2=EvtLeptonVACurrent(nut->spParentNeutrino(),p->sp(1));
    l1=EvtLeptonVACurrent(l->spParent(0),nul->spParentNeutrino());
    l2=EvtLeptonVACurrent(l->spParent(1),nul->spParentNeutrino());

  }
  else{
    tau1=EvtLeptonVACurrent(p->sp(0),nut->spParentNeutrino());
    tau2=EvtLeptonVACurrent(p->sp(1),nut->spParentNeutrino());
    l1=EvtLeptonVACurrent(nul->spParentNeutrino(),l->spParent(0));
    l2=EvtLeptonVACurrent(nul->spParentNeutrino(),l->spParent(1));
  }

  vertex(0,0,tau1*l1);
  vertex(0,1,tau1*l2);
  vertex(1,0,tau2*l1);
  vertex(1,1,tau2*l2);*/
  return;

}
