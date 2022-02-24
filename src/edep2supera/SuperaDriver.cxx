

#include "SuperaDriver.h"
#include "supera/data/Particle.h"
#include <iostream>

namespace edep2supera { 

	void SuperaDriver::ReadEvent(const TG4Event *ev) // returns a supera.Event to be used in SuperaAtomic
	{
		supera::EventInput result;

		//std::cout << ev << std::endl;

		//
		// Below std::map collections are defined in the version of this code
		// where parentage tracking was attempted within this function.
		// Kazu decided to move that portion elsewhere as the code that 
		// already works with Geant4 tracking information exists.
		//
		/*
		std::map<int, std::vector<int> > descendent_map;
		std::map<int, Particle *> Particle_map;
		std::map<int, int> primToInt;
		*/

		for (auto const &traj : ev->Trajectories)
		{
			supera::ParticleInput part_input;
			/*
			if (traj.GetParentId()==0)
			{
				primToInt[traj.GetTrackId()]=numprim;
				numprim+=1;
			}
			if (traj.GetTrackId()!=traj.GetParentId()&&traj.GetParentId()>0)
			{
				if (Particle_map.find(traj.GetParentId()) == Particle_map.end()) Particle_map.[traj.GetParentId()]= std::vector<int> vect{traj.GetTrackId()};
				else Particle_map.[traj.GetParentId()].push_back(traj.GetTrackId());
			}
			*/
			supera::Particle p;
			auto const& start = traj.Points.front().GetPosition();
			auto const& end   = traj.Points.back().GetPosition();

			// first, last steps, distance travel, energy_deposit, num_voxels

			p.id = result.size(); ///< "ID" of this particle in ParticleSet collection
			p.trackid = traj.GetTrackId(); ///< Geant4 track id
			p.pdg = traj.GetPDGCode();	 ///< PDG code
			p.px = traj.GetInitialMomentum().Px();
			p.py = traj.GetInitialMomentum().Py();
			p.pz = traj.GetInitialMomentum().Pz();										///< (x,y,z) component of particle's initial momentum
			p.vtx = supera::Vertex(start.X() / 10., start.Y() / 10., start.Z() / 10., start.T()); ///< (x,y,z,t) of particle's vertex information
			p.end_pt = supera::Vertex(end.X() / 10., end.Y() / 10., end.Z() / 10., end.T());		///< (x,y,z,t) at which particle disappeared from G4WorldVolume
			//p.process = traj.Points.GetProcess();											///< string identifier of the particle's creation process from Geant4
			p.energy_init = traj.GetInitialMomentum().E();										///< initial energy of the particle
			p.parent_trackid = traj.GetParentId();
			//
			// Code below fills parent/ancestor information but commented out
			// because this will be implemented in a separate function that analyzes 
			// the hierarchy tree of all particles.
			//
			/*																		   ///< Geant4 track id of the parent particle
			p.parent_pdg = parentTraj.GetTrackId();																		   ///< PDG code of the parent particle
			p.parent_vtx = supera::Vertex(parent_start.X() / 10., parent_start.Y() / 10., parent_start.Z() / 10., parent_start.T()); ///< (x,y,z,t) of parent's vertex information

			p.ancestor_trackid = ancestorTraj.GetParentId();																			 ///< Geant4 track id of the ancestor particle
			p.ancestor_pdg = ancestorTraj.GetTrackId();																				 ///< PDG code of the ancestor particle
			p.ancestor_vtx = Vertex(ancestor_start.X() / 10., ancestor_start.Y() / 10., ancestor_start.Z() / 10., ancestor_start.T()); ///< (x,y,z,t) of ancestor's vertex information
			std::string ancestor_process = ancestorTraj.Points.GetProcess();															 ///< string identifier of the ancestor particle's creation process from Geant4
			std::string parent_process = parentTraj.Points.GetProcess();  ///< string identifier of the parent particle's creation process from Geant4
			*/

			//
			// Code below is meant to fill energy deposition information from segments
			// It is incomplete and commented out as this is not supposed to happen within this function.
			//
			/*
			for (const auto &sensitiveDetPair : ev->SegmentDetectors)
			{
				for (const auto &sedep : sensitiveDetPair.second)
				{
					if std::end(sedep.Contrib)!=(std::find(std::begin(sedep.Contrib), std::end(sedep.Contrib),p.trackid)
					{
						p.energy_deposit+=sedep.GetEnergyDeposit()/sedep.Contrib.size();
						p.dist_travel+=sedep.GetTrackLength();
						if (p.first_step.time<sedep.GetStart().T()) p.first_step=sedep.GetStart();
						if (p.last_step.time>sedep.GetStart().T()) p.first_step=sedep.GetStart();
					}
				}
			}
			*/
			part_input.part=p;

			result.push_back(part_input);
		}
	}
}
