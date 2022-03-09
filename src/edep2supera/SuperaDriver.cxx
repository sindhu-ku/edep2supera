

#include "SuperaDriver.h"
#include "supera/data/Particle.h"
#include "supera/base/meatloaf.h"
#include <iostream>

namespace edep2supera { 

	void SuperaDriver::ReadEvent(const TG4Event *ev) // returns a supera.Event to be used in SuperaAtomic
	{
		supera::EventInput result;
		result.reserve(ev->Trajectories.size());

		for (auto const &traj : ev->Trajectories)
		{
			supera::ParticleInput part_input;
			part_input.valid   = true;
			part_input.part    = this->TG4TrajectoryToParticle(traj);
			part_input.part.id = result.size();
			result.push_back(part_input);
		}

		// Infer particle hierarchy
		_hierarchy_alg.SetParentInfo(result);

		// Infer creation process type
		for(unsigned int idx=0; idx<result.size(); ++idx) {
			auto const& edepsim_part = ev->Trajectories[idx];
			auto& part_input = result[idx];
			part_input.type = this->InferProcessType(edepsim_part, part_input.part);
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

	supera::ProcessType 
	SuperaDriver::InferProcessType(const TG4Trajectory& edepsim_part, 
		const supera::Particle& supera_part)
	{

		auto pdg_code    = supera_part.pdg;
		auto g4type_main = edepsim_part.Points.front().GetProcess();
		auto g4type_sub  = edepsim_part.Points.front().GetSubprocess();

		unsigned int semantic_type;

		if(pdg_code == 22) {
			return supera::kPhoton;
		}else if(std::abs(pdg_code) == 11) {
			if( supera_part.parent_trackid == -1 ){
				return supera::kPrimary;
			}
			else if( g4type_main == TG4TrajectoryPoint::G4ProcessType::kProcessElectromagetic ) {
				if( g4type_sub == TG4TrajectoryPoint::G4ProcessSubtype::kSubtypeEMPhotoelectric ) {
					return supera::kPhotoElectron;
				}
				else if( g4type_sub == TG4TrajectoryPoint::G4ProcessSubtype::kSubtypeEMComptonScattering ) {
					return supera::kCompton;
				}
				else if( g4type_sub == TG4TrajectoryPoint::G4ProcessSubtype::kSubtypeEMGammaConversion ) {
					return supera::kConversion;
				}else if( g4type_sub == TG4TrajectoryPoint::G4ProcessSubtype::kSubtypeEMIonization ) {
					if( std::abs(supera_part.parent_pdg) == 11 ) {
						return supera::kIonization;
					}else if(std::abs(supera_part.parent_pdg) == 211 || 
						std::abs(supera_part.parent_pdg) == 13 || 
						std::abs(supera_part.parent_pdg) == 2212) {
						return supera::kDelta;
					}else{
						std::cout << "UNEXPECTED CASE for IONIZATION " << std::endl
						<< "PDG " << pdg_code 
						<< " TrackId " << edepsim_part.TrackId
						<< " Energy " << supera_part.energy_init 
						<< " Parent PDG " << supera_part.parent_pdg 
						<< " Parent TrackId " << edepsim_part.ParentId
						<< " G4ProcessType " << g4type_main 
						<< " SubProcessType " << g4type_sub
						<< std::endl;
						throw supera::meatloaf();
					}
				}else{
					std::cout << "UNEXPECTED EM SubType " << std::endl
					<< "PDG " << pdg_code 
					<< " TrackId " << edepsim_part.TrackId
					<< " Energy " << supera_part.energy_init 
					<< " Parent PDG " << supera_part.parent_pdg 
					<< " Parent TrackId " << edepsim_part.ParentId
					<< " G4ProcessType " << g4type_main 
					<< " SubProcessType " << g4type_sub
					<< std::endl;
					throw supera::meatloaf();
				}
			}
			else if( g4type_main == TG4TrajectoryPoint::G4ProcessType::kProcessDecay ) {
				return supera::kDecay;
			}else{
				std::cout << "Cannot classify this shower" << std::endl 
				<< "PDG " << pdg_code 
				<< " TrackId " << edepsim_part.TrackId
				<< " Energy " << supera_part.energy_init 
				<< " Parent PDG " << supera_part.parent_pdg 
				<< " Parent TrackId " << edepsim_part.ParentId
				<< " G4ProcessType " << g4type_main 
				<< " SubProcessType " << g4type_sub
				<< std::endl;
				return supera::kOtherShower;
			}
		}
		else {
			if(pdg_code == 2112)
				return supera::kNeutron;
			return supera::kTrack;
		}

	} 
}
