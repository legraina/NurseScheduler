/*
* DemandGenerator.cpp
*
*  Created on: April 27, 2015
*      Author: jeremy omer
*/

#include "DemandGenerator.h"
#include "ReadWrite.h"
#include "MyTools.h"
#include "Nurse.h"
#include "Greedy.h"


//----------------------------------------------------------------------------
// Constructors and destructors
//----------------------------------------------------------------------------

DemandGenerator::~DemandGenerator() {

}


//----------------------------------------------------------------------------
// Check the feasibility of a demand scenario
//----------------------------------------------------------------------------

bool DemandGenerator::checkDemandFeasibility(Demand* pDemand) {

	// Create empty preferences and initial states (this represents the most favorable situation)
	// They are needed to run the greedy algorithm.
	// We prefer take empty objects than arbitrary values, because the optimization of the current week
	// is likely to find a favorable situation
	Preferences* pPref = new Preferences(pScenario_->nbNurses_,pScenario_->nbDays(),pScenario_->nbShifts_);
	vector<State> emptyStates;
	for (int i = 0; i < pScenario_->nbNurses_; i++) {
		State state;
		emptyStates.push_back(state);
	}


	// Todo: build empty preferences and empty initial state to test the feasibility
	Greedy* pGreedy = new Greedy(pScenario_, pDemand, pPref, &emptyStates);
	bool ans = pGreedy->constructiveGreedy();
	if(ans){
		std::cout << "# Demand has been checked and is valid" << std::endl;
	}
	return ans;

}

//----------------------------------------------------------------------------
// Generate nbDemands_ demand scenaios through perturbations of the demand
// history
// Each demand must consider nbDays_ days
//----------------------------------------------------------------------------

vector<Demand*> DemandGenerator::generatePerturbedDemands() {

	// number of demands generated until now
	vector<Demand*> generatedDemands;

	// Generate the demands
	for(int coDemand = 0; coDemand < nbDemandsToGenerate_; coDemand++) {
		generatedDemands.push_back(generateSinglePerturbatedDemand());
	}
	return generatedDemands;
}


// generate 1 demand through perturbations of the demand history
Demand * DemandGenerator::generateSinglePerturbatedDemand(bool checkFeasibility){

	// number of demands in the history
	int nbPastDemands = demandHistory_.size();

	// // number of demands generated until now
	// Demand * pGeneratedDemand;

	// the generation of demands will depend on the number of weeks treated by the
	// demand
	int nbWeeksInGeneratedDemands = (nbDaysInGeneratedDemands_-1)/7+1;

	// the demands are going to be generated by perturbing past demands
	while (true) {

		// one reference history demand is randomly drawn for each week
		vector<int> indexInHistory;
		for (int i = 0; i < nbWeeksInGeneratedDemands; i++) {
			indexInHistory.push_back(rdm_()%nbPastDemands);
		}

		// create the first week
		Demand* pCompleteDemand = demandHistory_[indexInHistory[0]]->randomPerturbation();

		// int t = demandHistory_.size();
		// int i = indexInHistory[0];


		// create the following weeks append them to the complete demand
		for (int i = 0; i < nbWeeksInGeneratedDemands-1; i++) {
			Demand* pWeekDemand = demandHistory_[indexInHistory[i]]->randomPerturbation();
			pCompleteDemand->push_back(pWeekDemand);
			delete pWeekDemand;
		}

		// keep only the required number of days
		pCompleteDemand->keepFirstNDays(nbDaysInGeneratedDemands_);

		// keep the generated demand only if it is feasible
		if (checkFeasibility){
			if(checkDemandFeasibility(pCompleteDemand)) {
				return pCompleteDemand;
			}
			else {
				delete pCompleteDemand;
			}
		} else {
			return pCompleteDemand;
		}
	}
	return 0;
}





