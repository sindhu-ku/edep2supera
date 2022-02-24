

#include "SuperaDriver.h"
#include "supera/data/Particle.h"
#include <iostream>

namespace edep2supera { 

	void SuperaDriver::ReadEvent(const TG4Event *ev) // returns a supera.Event to be used in SuperaAtomic
	{
		supera::EventInput result;

		//std::cout << ev << std::endl;

		for (auto const &traj : ev->Trajectories)
		{
			supera::ParticleInput part_input;

			part_input.valid   = true;
			part_input.part    = this->TG4TrajectoryToParticle(traj);
			part_input.part.id = result.size();
			part_input.type    = this->InferProcessType(traj);

			result.push_back(part_input);
		}


	}

	supera::Particle SuperaDriver::TG4TrajectoryToParticle(const TG4Trajectory& edepsim_part)
	{
		supera::Particle result;
		auto const& start = edepsim_part.Points.front().GetPosition();
		auto const& end   = edepsim_part.Points.back().GetPosition();

		// first, last steps, distance travel, energy_deposit, num_voxels

		result.trackid = edepsim_part.GetTrackId(); ///< Geant4 track id
		result.pdg = edepsim_part.GetPDGCode();	 ///< PDG code
		result.px = edepsim_part.GetInitialMomentum().Px();
		result.py = edepsim_part.GetInitialMomentum().Py();
		result.pz = edepsim_part.GetInitialMomentum().Pz();										///< (x,y,z) component of particle's initial momentum
		result.vtx = supera::Vertex(start.X() / 10., start.Y() / 10., start.Z() / 10., start.T()); ///< (x,y,z,t) of particle's vertex information
		result.end_pt = supera::Vertex(end.X() / 10., end.Y() / 10., end.Z() / 10., end.T());		///< (x,y,z,t) at which particle disappeared from G4WorldVolume
		//result.process = edepsim_part.Points.GetProcess();											///< string identifier of the particle's creation process from Geant4
		result.energy_init = edepsim_part.GetInitialMomentum().E();										///< initial energy of the particle
		result.parent_trackid = edepsim_part.GetParentId();
		//
		// Code below fills parent/ancestor information but commented out
		// because this will be implemented in a separate function that analyzes 
		// the hierarchy tree of all particles.
		//
		/*																		   ///< Geant4 track id of the parent particle
		result.parent_pdg = parentTraj.GetTrackId();																		   ///< PDG code of the parent particle
		result.parent_vtx = supera::Vertex(parent_start.X() / 10., parent_start.Y() / 10., parent_start.Z() / 10., parent_start.T()); ///< (x,y,z,t) of parent's vertex information

		result.ancestor_trackid = ancestorTraj.GetParentId();																			 ///< Geant4 track id of the ancestor particle
		result.ancestor_pdg = ancestorTraj.GetTrackId();																				 ///< PDG code of the ancestor particle
		result.ancestor_vtx = Vertex(ancestor_start.X() / 10., ancestor_start.Y() / 10., ancestor_start.Z() / 10., ancestor_start.T()); ///< (x,y,z,t) of ancestor's vertex information
		std::string ancestor_process = ancestorTraj.Points.GetProcess();															 ///< string identifier of the ancestor particle's creation process from Geant4
		std::string parent_process = parentTraj.Points.GetProcess();  ///< string identifier of the parent particle's creation process from Geant4
		*/
		return result;
	}

	supera::ProcessType SuperaDriver::InferProcessType(const TG4Trajectory& edepsim_part)
	{


		auto pdg_code = edepsim_part.GetPDGCode();
		auto g4type_main = edepsim_part.Points.front().GetProcess();
		auto g4type_sub  = edepsim_part.Points.front().GetSubprocess();


		if(pdg_code == 22) {
			return supera::kPhoton;
		}else if(pdg_code == 11) {
			std::cout << "PDG 11 G4ProcessType " << g4type_main 
			<< " SubProcessType " << g4type_sub
			<< std::endl;
			/*
			std::string prc = mcpart.Process();
			if( prc == "muIoni" || prc == "hIoni" || prc == "muPairProd" )
				grp.type = supera::kDelta;
			else if( prc == "muMinusCaptureAtRest" || prc == "muPlusCaptureAtRest" || prc == "Decay" )
				grp.type = supera::kDecay;
			else if( prc == "compt"  )
				grp.type = supera::kCompton;
			else if( prc == "phot"   )
				grp.type = supera::kPhotoElectron;
			else if( prc == "eIoni"  )
				grp.type = supera::kIonization;
			else if( prc == "conv"   )
				grp.type = supera::kConversion;
			else if( prc == "primary")
				grp.type = supera::kPrimary;
			else
				grp.type = supera::kOtherShower;
			*/
			return supera::kOtherShower;			
		}
		else {
			if(pdg_code == 2112)
				return supera::kNeutron;
			return supera::kTrack;
		}

	} 
}
