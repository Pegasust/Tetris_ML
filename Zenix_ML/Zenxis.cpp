#include "Zenxis.h"

TetrisML::Zenxis::Zenxis(Seed const& initial_seed):
	current_best(), population(), current_best_score(std::numeric_limits<double>::lowest()),
	generation(0)
{
	seed[0] = initial_seed;
	population.reserve(N_INDIVIDUALS_PER_GEN);
	for (unsigned long long i = 0; i < N_INDIVIDUALS_PER_GEN; i++)
	{
		//Generate DNA
		DNAArray dna;

		//Unsupervised
		for (unsigned int i = 0; i < (sizeof(DNAArray) / sizeof(double)); i++)
		{
			dna.arr[i] = SeedHelper::get_value(seed, -1.0, 1.0);
		}

		dna.dna.do_normalization(); //need for normalization since it affects offsprings
		Zenx new_zenx = Zenx(dna);
		population.push_back(new_zenx);
	}
	//std::cout << "aa" << std::endl;
}

//Deprecated. Use experiment()
void TetrisML::Zenxis::big_bang()
{
	
	//double ranked_scores[SURVIVORS_PER_GEN];
	double best_scores[2];
	unsigned long long best_zenx[2];
	double avg_hscore = 0;
	double avg_burns = 0;
	double avg_level = 0;


	for (unsigned char i = 0; i < 2; i++) //Assign the first two as the two best zenxis
	{
		ZenixAgent::RawObservation obs = 
		population[i].play_once(seed);
		best_scores[i] = population[i].lifetime_record.highest_score;
		best_zenx[i] = i;
		avg_hscore += population[i].lifetime_record.highest_score;
		avg_burns += obs.burn;
		avg_level += (double)population[i].lifetime_record.level;
	}
	//Because it is a hasty assignment, reassign if we have to.
	if (best_scores[0] < best_scores[1])
	{
		swap(best_scores[0], best_scores[1]);
		swap(best_zenx[0], best_zenx[1]);
	}
	for (unsigned long long i = 2; i < N_INDIVIDUALS_PER_GEN; i++)
	{
		ZenixAgent::RawObservation obs = 
		population[i].play_once(seed);
		//Score is the highest priority
		if (population[i].lifetime_record.highest_score > best_scores[1])
		{
			if (population[i].lifetime_record.highest_score > best_scores[0])
			{
				//Reassign the highest position
				best_scores[1] = best_scores[0];
				best_zenx[1] = best_zenx[0];
				best_scores[0] = population[i].lifetime_record.highest_score;
				best_zenx[0] = i;
			}
			else
			{
				//Reassign the second highest position
				best_scores[1] = population[i].lifetime_record.highest_score;
				best_zenx[1] = i;
			}
		}

		avg_hscore += population[i].lifetime_record.highest_score;
		avg_burns += obs.burn;
		avg_level += population[i].lifetime_record.level;
	}
	//Assign the best in this ecosystem
	if (best_scores[0] > current_best_score)
	{
		current_best_score = best_scores[0];
		current_best.dna_config.dna = population[best_zenx[0]].dna;
		current_best.generation = generation;
	}
	//Assign the best Zenxis
	std::vector<Zenx> new_gen_population;
	new_gen_population.reserve(N_INDIVIDUALS_PER_GEN);
	new_gen_population.push_back(population[best_zenx[0]]);
	new_gen_population.push_back(population[best_zenx[1]]);

	//Pickout the rest survivors
	//unsigned long long survivor_indexes[SURVIVORS_PER_GEN - 2];
	std::unordered_set<unsigned long long> occupied_indexes;
	occupied_indexes.insert(best_scores[0]);
	occupied_indexes.insert(best_scores[1]);
	for (unsigned long long i = 0; i < SURVIVORS_PER_GEN - 2; i++)
	{
		unsigned long long survivor = SURVIVORS_PER_GEN + 1;
		//NOT RELIABLE
		do
		{
			survivor = seed[0].long_expr % SURVIVORS_PER_GEN;
			SeedHelper::xorshift64(seed);
		} while (occupied_indexes.count(survivor) > 0); //while there that survival is already chosen
		occupied_indexes.insert(survivor);
		new_gen_population.push_back(population[survivor]);
	}
	population = new_gen_population;

	//Reproduce
	for (unsigned long long i = 0; i < SURVIVORS_PER_GEN; i += 2)
	{
		auto new_dna = Zenx::reproduce(population[i], population[i + 1]);
		new_dna.do_normalization(); //Have to normalize it so that it wouldn't go out of bounds in the long run.
		Zenx child(*(DNAArray*)&new_dna); //Lazy cast
		population.push_back(child);
	}

	//H: There are unwanted children who live in the wild too!
	for (unsigned long long i = 0; i < N_COMPLETE_RANDOM; i++)
	{
		DNAArray dna;
		for (unsigned int i = 0; i < (sizeof(DNAArray) / sizeof(double)); i++)
		{
			dna.arr[i] = SeedHelper::get_value(seed, -1.0, 1.0);
		}
		dna.dna.do_normalization();
		population.push_back(Zenx(dna));
	}

#ifdef _DEBUG
	if (population.size() != N_INDIVIDUALS_PER_GEN)
	{
		std::cout << std::to_string(population.size()) << std::endl;
		throw "invalid population.size()!" ;
	}
#endif

	//Print out to the console to keep track of progression
	std::cout << "Generation " << generation << "\tb_coef:{ "; //Generation
	//print best coefficient in this generation
	DNAArray* best_coef = (DNAArray*)&population[0];
	for (unsigned char i = 0; i < (sizeof(DNAArray) / sizeof(double)); i++)
	{
		std::cout << best_coef->arr[i] << ' ';
	}
	//best score
	std::cout << "}\tb_score: " << best_scores[0]<<'\t';
	//avg high score
	std::cout << "avg_hscore: " << (avg_hscore / (double)N_INDIVIDUALS_PER_GEN) << '\t';
	std::cout << "avg_burns: " << (avg_burns / (double)N_INDIVIDUALS_PER_GEN) << '\t';
	std::cout << "avg_level: " << (avg_level / (double)N_INDIVIDUALS_PER_GEN) << '\t' << std::endl;



	std::cout << std::endl;
	generation++;
}

void TetrisML::Zenxis::experiment(unsigned int const& n_exp, std::ofstream& store_stream)
{
#ifndef MULTI_THREAD
	double best_avgscores[2] = { std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest() };
	unsigned long long best_zenx[2] = { 0, 0 }; //Does not matter what value it is assigned to, as long as memory is alloated
	
	ZenixAgent::RawObservation best_zenx_avgobsv = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	//double best_zenx_avg_hscore = 0.0; //This is the same as best_avgscores[0]
	double best_zenx_avg_level = 0.0;

	ZenixAgent::RawObservation gen_avgobsv = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	double gen_avg_hscore = 0.0;
	double gen_avg_level =0.0;


	//Do experiment and get the meta data ready.
	for (unsigned long long i = 0; i < N_INDIVIDUALS_PER_GEN; i++)
	{
		//For every individual

		//(For average calculation)
		double zenx_avg_level = 0.0;
		ZenixAgent::RawObservation avg_obsv = { 0.0, 0.0, 0.0, 0.0, 0.0 };
		for (unsigned int exp = 0; exp < n_exp; exp++)
		{
			//For every experiment
			auto observation = population[i].experiment(seed, render, render);
			//observation contains average aggregate height, bulk, holes, and total # burns
			//One can access its highest_score_sum with population[i].sum_hscore
			//And its level with population[i].lifetime_record.level
			//SeedHelper::xorshift64(seed);
			avg_obsv += observation;
			zenx_avg_level += static_cast<double>(population[i].lifetime_record.level);
		}
		double avg_hscore = population[i].sum_hscore / static_cast<double>(population[i].times_played);
		zenx_avg_level /=(double) n_exp;
		avg_obsv /= (double) n_exp;

		//Check if it can be the best Zenx
		if (avg_hscore > best_avgscores[1])
		{
			if (avg_hscore > best_avgscores[0]) //Getting that crown and demote the previous king
			{
				//Demotion
				best_avgscores[1] = best_avgscores[0];
				best_zenx[1] = best_zenx[0];
				//Assignment
				best_avgscores[0] = avg_hscore;
				best_zenx[0] = i;
				best_zenx_avgobsv = avg_obsv;
				//best_zenx_avg_hscore = avg_hscore;
				best_zenx_avg_level = zenx_avg_level;
			}
			else //Take off that crown from the second position
			{
				best_avgscores[1] = avg_hscore;
				best_zenx[1] = i;
			}
		}
		gen_avgobsv += avg_obsv;
		gen_avg_hscore += avg_hscore;
		gen_avg_level += zenx_avg_level;
	}
	gen_avgobsv /= N_INDIVIDUALS_PER_GEN;
	gen_avg_hscore /= N_INDIVIDUALS_PER_GEN;
	gen_avg_level /= N_INDIVIDUALS_PER_GEN;

	//store data

	//if ( best_avgscores[0] > current_best_score && current_best.dna_config.dna != population[best_zenx[0]].dna)
	//	//Only when there is a new dna configuration that beats the current_best_score
	//{
	//	//They might be less consistent... but I think that's okay
	//	current_best_score = best_avgscores[0];
	//	current_best.dna_config.dna = population[best_zenx[0]].dna;
	//	current_best.generation = generation;
	//}

	ZenxMetaInfo best_inf = { best_zenx_avgobsv, best_zenx_avg_level, best_avgscores[0] };
	ZenxMetaInfo gen_inf = { gen_avgobsv, gen_avg_level, gen_avg_hscore };
	if (current_best.dna_config.dna != population[best_zenx[0]].dna) //Only show coefs when they are different. Quality of life.
	{
		push_data(store_stream, *(DNAArray*)(&population[best_zenx[0]].dna),
			best_inf,
			gen_inf);

		push_data(std::cout, *(DNAArray*)(&population[best_zenx[0]].dna),
			best_inf,
			gen_inf);
		current_best.dna_config.dna = population[best_zenx[0]].dna;
	}
	else
	{
		push_data(store_stream, 
			best_inf,
			gen_inf);

		push_data(std::cout, 
			best_inf,
			gen_inf);
	}


	//Get ready for a new generation
	std::vector<Zenx> next_gen;
	//Best two species get to continue the experiment
	next_gen.reserve(N_INDIVIDUALS_PER_GEN);
	next_gen.push_back(population[best_zenx[0]]);
	next_gen.push_back(population[best_zenx[1]]);

	//Pickout the rest survivors
	//unsigned long long survivor_indexes[SURVIVORS_PER_GEN - 2];
	std::unordered_set<unsigned long long> occupied_indexes;
	occupied_indexes.insert(best_zenx[0]);
	occupied_indexes.insert(best_zenx[1]);
	for (unsigned long long i = 0; i < SURVIVORS_PER_GEN - 2; i++)
	{
		unsigned long long survivor = SURVIVORS_PER_GEN + 1;
		//NOT RELIABLE
		do
		{
			survivor = seed[0].long_expr % SURVIVORS_PER_GEN;
			SeedHelper::xorshift64(seed);
		} while (occupied_indexes.count(survivor) > 0); //while there that survival is already chosen
		occupied_indexes.insert(survivor);
		next_gen.push_back(population[survivor]);
	}
	population = next_gen;
	//Reproduce
	for (unsigned long long i = 0; i < SURVIVORS_PER_GEN; i += 2)
	{
		auto new_dna = Zenx::reproduce(population[i], population[i + 1], SeedHelper::get_value(seed, -1.0, 1.0));
		new_dna.do_normalization(); //Have to normalize it so that it wouldn't go out of bounds in the long run.
		Zenx child(*(DNAArray*)& new_dna); //Lazy cast
		population.push_back(child);
	}

	//H: Evolution happens. Some other species evolve to Zenxis.
	for (unsigned long long i = 0; i < N_COMPLETE_RANDOM; i++)
	{
		DNAArray dna;
		for (unsigned int i = 0; i < (sizeof(DNAArray) / sizeof(double)); i++)
		{
			dna.arr[i] = SeedHelper::get_value(seed, -1.0, 1.0);
		}
		dna.dna.do_normalization();
		population.push_back(Zenx(dna));
	}
	generation++;
#else
	experiment_multi_thread(n_exp, store_stream);
#endif
}

//H: Not everyone has to play at the same time, there is no queue here. At this point in time, Zenxis were able to purchase
//more arcade machines to reduce the queue to play Tetris.
void TetrisML::Zenxis::experiment_multi_thread(unsigned int const& n_exp, std::ofstream& store_stream)
{
#ifdef MULTI_THREAD
	std::vector<std::thread> thread_vecs;
//#define RESERVE(local_name) thread_##local_name##.reserve(THREADS)
	double thread_best_avgscores[THREADS*2];
	unsigned long long thread_best_zenx [THREADS* 2];
	ZenixAgent::RawObservation thread_best_zenx_avgobsv[THREADS];
	double thread_best_zenx_avg_level[THREADS];
	ZenixAgent::RawObservation thread_gen_avgobsv[THREADS];
	double thread_gen_avg_hscore[THREADS];
	double thread_gen_avg_level[THREADS];
//#undef RESERVE
	for (unsigned char t = 0; t < THREADS; t++)
	{
		std::function<void()> thread_exp = [this, &n_exp,  t,&thread_best_avgscores, &thread_best_zenx, &thread_best_zenx_avgobsv,
		&thread_best_zenx_avg_level, &thread_gen_avgobsv, &thread_gen_avg_hscore, &thread_gen_avg_level]()
		{
			Seed seed[1] = { (this->seed[0].long_expr) >> 2 + (TMath::GameRNG::generate_random_seed() >> 2) };
			thread_best_avgscores[2 * t] = std::numeric_limits<double>::lowest();
			thread_best_avgscores[2 * t + 1] = std::numeric_limits<double>::lowest();
			thread_best_zenx[2 * t] = 0; thread_best_zenx[2 * t + 1] = 0;
			thread_best_zenx_avgobsv[t] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
			thread_best_zenx_avg_level[t] = 0.0;
			thread_gen_avg_hscore[t] = 0.0;
			thread_gen_avg_level[t] = 0.0;
			for (unsigned long long i = t; i < N_INDIVIDUALS_PER_GEN; i+=THREADS)
			{
				//For every individual

				//(For average calculation)
				double zenx_avg_level = 0.0;
				ZenixAgent::RawObservation avg_obsv = { 0.0, 0.0, 0.0, 0.0, 0.0 };
				for (unsigned int exp = 0; exp < n_exp; exp++)
				{
					//For every experiment
					auto observation = population[i].experiment(seed, render, render);
					//observation contains average aggregate height, bulk, holes, and total # burns
					//One can access its highest_score_sum with population[i].sum_hscore
					//And its level with population[i].lifetime_record.level
					//SeedHelper::xorshift64(seed);
					avg_obsv += observation;
					zenx_avg_level += static_cast<double>(population[i].lifetime_record.level);
				}
				double avg_hscore = population[i].sum_hscore / static_cast<double>(population[i].times_played);
				zenx_avg_level /= (double)n_exp;
				avg_obsv /= (double)n_exp;

				//Check if it can be the best Zenx
				if (avg_hscore > thread_best_avgscores[2*t+1])
				{
					if (avg_hscore > thread_best_avgscores[2*t+0]) //Getting that crown and demote the previous king
					{
						//Demotion
						thread_best_avgscores[2*t+1] = thread_best_avgscores[2*t+0];
						thread_best_zenx[2*t+1] = thread_best_zenx[2*t+0];
						//Assignment
						thread_best_avgscores[2*t+0] = avg_hscore;
						thread_best_zenx[2*t+0] = i;
						thread_best_zenx_avgobsv[t] = avg_obsv;
						//best_zenx_avg_hscore = avg_hscore;
						thread_best_zenx_avg_level[t] = zenx_avg_level;
					}
					else //Take off that crown from the second position
					{
						thread_best_avgscores[2*t+1] = avg_hscore;
						thread_best_zenx[2*t+1] = i;
					}
				}
				thread_gen_avgobsv[t] += avg_obsv;
				thread_gen_avg_hscore[t] += avg_hscore;
				thread_gen_avg_level[t] += zenx_avg_level;
			}
			/*
			thread_gen_avgobsv[t] /= N_INDIVIDUALS_PER_GEN;
			thread_gen_avg_hscore[t] /= N_INDIVIDUALS_PER_GEN;
			thread_gen_avg_level[t] /= N_INDIVIDUALS_PER_GEN;*/
		};
		thread_vecs.push_back(std::thread(thread_exp));
	}
	for (std::thread& thread : thread_vecs)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}

	double best_avgscores[2] = { thread_best_avgscores[0], thread_best_avgscores[1]};
	unsigned long long best_zenx[2]= { thread_best_zenx[0], thread_best_zenx[1] }; //Does not matter what value it is assigned to, as long as memory is alloated
		
	ZenixAgent::RawObservation best_zenx_avgobsv = thread_best_zenx_avgobsv[0];
	//double best_zenx_avg_hscore = 0.0; //This is the same as best_avgscores[0]
	double best_zenx_avg_level = thread_best_zenx_avg_level[0];

	ZenixAgent::RawObservation gen_avgobsv = thread_gen_avgobsv[0];
	double gen_avg_hscore = thread_gen_avg_hscore[0];
	double gen_avg_level = thread_gen_avg_level[0];

	for (unsigned char i = 1; i < THREADS; i ++) //Linear search but with pairs, more like even_search
	{
		//Do mergesort to search for the best two avg scores
		if (thread_best_avgscores[i * 2 + 0] > best_avgscores[1])
		{
			if (thread_best_avgscores[i * 2 + 0] > best_avgscores[0])
			{
				best_avgscores[1] = best_avgscores[0];
				best_zenx[1] = best_zenx[0];
				best_avgscores[0] = thread_best_avgscores[i * 2];
				best_zenx[0] = thread_best_zenx[i * 2];
				best_zenx_avgobsv = thread_best_zenx_avgobsv[i];
				best_zenx_avg_level = thread_best_zenx_avg_level[i];
				if (thread_best_avgscores[i * 2 + 1] > best_avgscores[1])
				{
					best_avgscores[1] = thread_best_avgscores[i * 2 + 1];
					best_zenx[1] = thread_best_zenx[i * 2 + 1];
				}
			}
			else
			{
				best_avgscores[1] = thread_best_avgscores[i * 2 + 0];
				best_zenx[1] = thread_best_zenx[i * 2 + 0];
			}
		}
		gen_avgobsv += thread_gen_avgobsv[i];
		gen_avg_hscore += thread_gen_avg_hscore[i];
		gen_avg_level += thread_gen_avg_level[i];
	}
	//Do experiment and get the meta data ready.
	gen_avgobsv /= N_INDIVIDUALS_PER_GEN;
	gen_avg_hscore /= N_INDIVIDUALS_PER_GEN;
	gen_avg_level /= N_INDIVIDUALS_PER_GEN;

	//store data

	//if ( best_avgscores[0] > current_best_score && current_best.dna_config.dna != population[best_zenx[0]].dna)
	//	//Only when there is a new dna configuration that beats the current_best_score
	//{
	//	//They might be less consistent... but I think that's okay
	//	current_best_score = best_avgscores[0];
	//	current_best.dna_config.dna = population[best_zenx[0]].dna;
	//	current_best.generation = generation;
	//}

	ZenxMetaInfo best_inf = { best_zenx_avgobsv, best_zenx_avg_level, best_avgscores[0] };
	ZenxMetaInfo gen_inf = { gen_avgobsv, gen_avg_level, gen_avg_hscore };
	if (current_best.dna_config.dna != population[best_zenx[0]].dna) //Only show coefs when they are different. Quality of life.
	{
		push_data(store_stream, *(DNAArray*)(&population[best_zenx[0]].dna),
			best_inf,
			gen_inf);

		push_data(std::cout, *(DNAArray*)(&population[best_zenx[0]].dna),
			best_inf,
			gen_inf);
		current_best.dna_config.dna = population[best_zenx[0]].dna;
	}
	else
	{
		push_data(store_stream,
			best_inf,
			gen_inf);

		push_data(std::cout,
			best_inf,
			gen_inf);
	}


	//Get ready for a new generation
	std::vector<Zenx> next_gen;
	//Best two species get to continue the experiment
	next_gen.reserve(N_INDIVIDUALS_PER_GEN);
	next_gen.push_back(population[best_zenx[0]]);
	next_gen.push_back(population[best_zenx[1]]);

	//Pickout the rest survivors
	//unsigned long long survivor_indexes[SURVIVORS_PER_GEN - 2];
	std::unordered_set<unsigned long long> occupied_indexes;
	occupied_indexes.insert(best_zenx[0]);
	occupied_indexes.insert(best_zenx[1]);
	for (unsigned long long i = 0; i < SURVIVORS_PER_GEN - 2; i++)
	{
		unsigned long long survivor = SURVIVORS_PER_GEN + 1;
		//NOT RELIABLE
		do
		{
			survivor = seed[0].long_expr % SURVIVORS_PER_GEN;
			SeedHelper::xorshift64(seed);
		} while (occupied_indexes.count(survivor) > 0); //while there that survival is already chosen
		occupied_indexes.insert(survivor);
		next_gen.push_back(population[survivor]);
	}
	population = next_gen;
	//Reproduce
	for (unsigned long long i = 0; i < SURVIVORS_PER_GEN; i += 2)
	{
		auto new_dna = Zenx::reproduce(population[i], population[i + 1], SeedHelper::get_value(seed, -1.0, 1.0));
		new_dna.do_normalization(); //Have to normalize it so that it wouldn't go out of bounds in the long run.
		Zenx child(*(DNAArray*)& new_dna); //Lazy cast
		population.push_back(child);
	}

	//H: Evolution happens. Some other species evolve to Zenxis.
	for (unsigned long long i = 0; i < N_COMPLETE_RANDOM; i++)
	{
		DNAArray dna;
		for (unsigned int i = 0; i < (sizeof(DNAArray) / sizeof(double)); i++)
		{
			dna.arr[i] = SeedHelper::get_value(seed, -1.0, 1.0);
		}
		dna.dna.do_normalization();
		population.push_back(Zenx(dna));
	}
	generation++;
#endif
}

void TetrisML::Zenxis::push_data(std::ostream & fstream, const DNAArray& dna, ZenxMetaInfo const& best_zenx_info, ZenxMetaInfo const& gen_avg_info)
{
#define PRINT_OBSV(obsv, fs) fs<<"{" <<obsv.bulkiness<<'|'<<obsv.burn<<'|'<<obsv.aggregate_height<<'|'<<obsv.holes<<"}"
#define PRINT_META(meta, fs) fs<<meta.high_score<<'|'<<meta.level<<','; PRINT_OBSV(meta.obsv, fs);
	//fstream.precision(std::numeric_limits<double>::max_digits10);
	fstream << "#G" << generation << "#\t:{";
	for (unsigned char i = 0; i < sizeof(DNAArray) / sizeof(double); i++)
	{
		fstream << (dna.arr[i]);
		if (i != sizeof(DNAArray) / sizeof(double) - 1)
		{
			fstream << ",";
		}
	}
	fstream << "};\tRecord:";
	PRINT_META(best_zenx_info, fstream);
	fstream << "\tGen:";
	PRINT_META(gen_avg_info, fstream);
	fstream << std::endl;
#undef PRINT_META
#undef PRINT_OBSV
}

void TetrisML::Zenxis::push_data(std::ostream& fstream, ZenxMetaInfo const& best_zenx_info, ZenxMetaInfo const& gen_avg_info)
{
#define PRINT_OBSV(obsv, fs) fs<<"{" <<obsv.bulkiness<<'|'<<obsv.burn<<'|'<<obsv.aggregate_height<<'|'<<obsv.holes<<"}"
#define PRINT_META(meta, fs) fs<<meta.high_score<<'|'<<meta.level<<','; PRINT_OBSV(meta.obsv, fs);
	//fstream.precision(std::numeric_limits<double>::max_digits10);
	fstream << "_G" << generation << "\t:{";
	for (unsigned char i = 0; i < sizeof(DNAArray) / sizeof(double); i++)
	{
		fstream << "____";
		if (i != sizeof(DNAArray) / sizeof(double) - 1)
		{
			fstream << "___";
		}
	}
	fstream << "};\t\tRecord:";
	PRINT_META(best_zenx_info, fstream);
	fstream << "\tGen:";
	PRINT_META(gen_avg_info, fstream);
	fstream << std::endl;
#undef PRINT_META
#undef PRINT_OBSV
}

void TetrisML::Zenxis::render(const LiteGameModule::LiteModule & mod)
{
}



//std::string TetrisML::Zenxis::serialize_string(const DNAArray& dna, ZenxMetaInfo const& best_zenx_info, ZenxMetaInfo const& gen_avg_info)
//{
//#define TO_STR(doubl) static_cast<ostringstream*>( &(ostringstream() << doubl) )->str();
//	std::string str = "G";
//	str += current_best_score;
//	str += ":{";
//	for (unsigned char i = 0; i < sizeof(DNAArray) / sizeof(double); i++)
//	{
//		str += TO_STR(dna.arr[i]);
//	}
//	str += "}";
//}

