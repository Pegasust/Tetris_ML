#pragma once
namespace TetrisML
{
	namespace Zenoy
	{
		typedef TMath::GameRNG::RNGUnion Seed;
		typedef TMath::GameRNG SeedHelper;
		class Zenoy
		{
			unsigned long long generation;
			Seed seed[1];
			//Number of threads that run the "hardwork" such as simulating or breeding
			unsigned char n_sim_threads; //Default should be 2
			MetaInfo current_most_intelligent;

			//Serialization and deserilization functions
			const int (*try_serialize)(const Zenoy& obj);
			const int (*try_deserialize)(Zenoy& to_be_this); //Assign to to_be_this



		};
	}
}