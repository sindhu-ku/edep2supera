

#include "SuperaDriver.h"
#include <iostream>

namespace supera
{

	EventInput SuperaDriver::ReadEvent(const TG4Event *ev) // returns a supera.Event to be used in SuperaAtomic
	{
		EventInput EI=EventInput();

		//std::cout << ev << std::endl;

		std::map<int, std::vector<int> > descendent_map;
		std::map<int, Particle *> Particle_map;
		std::map<int, int> primToInt;
		int partCounter=0;
		int numprim=0;

		for (auto const &traj : ev->Trajectories)
		{
			ParticleInput PI = ParticleInput();
			if (traj->GetParentId()==0)
			{
				primToInt[traj->GetTrackId()]=numprim;
				numprim+=1;
			}
			if (traj->GetTrackId()!=traj->GetParentId()&&traj->GetParentId()>0)
			{
				if (Particle_map.find(traj->GetParentId()) == Particle_map.end()) Particle_map.[traj->GetParentId()]= std::vector<int> vect{traj->GetTrackId()};
				else Particle_map.[traj->GetParentId()].push_back(traj->GetTrackId());
			}
			++partCounter;
			const TG4Trajectory *parentTraj = nullptr;
			const TG4Trajectory *ancestorTraj = nullptr;

			const TG4Trajectory *t = &traj;
			while (t->GetParentId() >= 0 && t->GetParentId() < static_cast<int>(ev->Trajectories.size()))
			{
				const TG4Trajectory *p = &ev->Trajectories[t->GetParentId()];
				if (!parentTraj) 
				{
					parentTraj = p;
					t = p;
					continue
				}
				if (!ancestorTraj)
				{
					ancestorTraj = p;
					t = p;
					continue
				}
				t = p;
			}

			Particle res;
			
			auto const start = traj.Points.front().GetPosition();
			auto const end = traj.Points.back().GetPosition();

			auto const parent_start = parentTraj.Points.front().GetPosition();
			auto const ancestor_start = ancestorTraj.Points.front().GetPosition();

			// first, last steps, distance travel, energy_deposit, num_voxels

			res.id = partCounter;			 ///< "ID" of this particle in ParticleSet collection
			res.shape=void;//??????? 		 ///< shows if it is (e+/e-/gamma) or other particle types
			res.trackid = traj.GetTrackId(); ///< Geant4 track id
			res.pdg = traj.GetPDGCode();	 ///< PDG code
			res.px = traj.GetInitialMomentum().Px();
			res.py = traj.GetInitialMomentum().Py();
			res.pz = traj.GetInitialMomentum().Pz();										///< (x,y,z) component of particle's initial momentum
			res.vtx = Vertex(start.X() / 10., start.Y() / 10., start.Z() / 10., start.T()); ///< (x,y,z,t) of particle's vertex information
			res.end_pt = Vertex(end.X() / 10., end.Y() / 10., end.Z() / 10., end.T());		///< (x,y,z,t) at which particle disappeared from G4WorldVolume
			res.process = traj.Points.GetProcess();											///< string identifier of the particle's creation process from Geant4
			res.energy_init = res.GetMomentum().E();										///< initial energy of the particle
			res.parent_trackid = traj.GetParentId();																		   ///< Geant4 track id of the parent particle
			res.parent_pdg = parentTraj.GetTrackId();																		   ///< PDG code of the parent particle
			res.parent_vtx = Vertex(parent_start.X() / 10., parent_start.Y() / 10., parent_start.Z() / 10., parent_start.T()); ///< (x,y,z,t) of parent's vertex information

			res.ancestor_trackid = ancestorTraj.GetParentId();																			 ///< Geant4 track id of the ancestor particle
			res.ancestor_pdg = ancestorTraj.GetTrackId();																				 ///< PDG code of the ancestor particle
			res.ancestor_vtx = Vertex(ancestor_start.X() / 10., ancestor_start.Y() / 10., ancestor_start.Z() / 10., ancestor_start.T()); ///< (x,y,z,t) of ancestor's vertex information
			std::string ancestor_process = ancestorTraj.Points.GetProcess();															 ///< string identifier of the ancestor particle's creation process from Geant4

			std::string parent_process = parentTraj.Points.GetProcess();  ///< string identifier of the parent particle's creation process from Geant4
			InstanceID_t parent_id = -1;//??????			  ///< "ID" of the parent particle in ParticleSet collection
			std::vector<supera::InstanceID_t> children_id = void ; //???????? ///< "ID" of the children particles in ParticleSet collection
			InstanceID_t group_id = -1;//??????						  ///< "ID" to group multiple particles together (for clustering purpose)
			res.interaction_id=primToInt[t->GetTrackId()];

			res.first_step = Vertex(0,0,0,std::numeric_limits<double>::infinity());  //??????? ;													///< (x,y,z,t) of the first energy deposition point in the detector
			res.last_step = Vertex(0,0,0,-std::numeric_limits<double>::infinity());  //??????? ;												///< (x,y,z,t) of the last energy deposition point in the detector
			res.dist_travel = 0; 											///< filled only if MCTrack origin: distance measured along the trajectory
			res.energy_deposit = 0; 										///< deposited energy of the particle in the detector

			for (const auto &sensitiveDetPair : ev->SegmentDetectors)
			{
				for (const auto &sedep : sensitiveDetPair.second)
				{
					if std::end(sedep.Contrib)!=(std::find(std::begin(sedep.Contrib), std::end(sedep.Contrib),res.trackid)
					{
						res.energy_deposit+=sedep.GetEnergyDeposit()/sedep.Contrib.size();
						res.dist_travel+=sedep.GetTrackLength();
						if (res.first_step.time<sedep.GetStart().T()) res.first_step=sedep.GetStart();
						if (res.last_step.time>sedep.GetStart().T()) res.first_step=sedep.GetStart();
					}
				}
			}
			PI.part=res;

			PI.pcloud=void;//??????
			
			EI.push_back(PI);
		}
	}
}