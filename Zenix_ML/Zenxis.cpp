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
		avg_level += population[i].lifetime_record.level;
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
			zenx_avg_level += population[i].lifetime_record.level;
		}
		double avg_hscore = population[i].sum_hscore / (double)population[i].times_played;
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
	ZenxMetaInfo best_inf = { best_zenx_avgobsv, best_zenx_avg_level, best_avgscores[0] };
	ZenxMetaInfo gen_inf = { gen_avgobsv, gen_avg_level, gen_avg_hscore };
	push_data(store_stream, *(DNAArray*)(&population[best_zenx[0]].dna),
		best_inf,
		gen_inf);

	push_data(std::cout, *(DNAArray*)(&population[best_zenx[0]].dna),
		best_inf,
		gen_inf);


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
}

void TetrisML::Zenxis::push_data(std::ostream & fstream, const DNAArray& dna, ZenxMetaInfo const& best_zenx_info, ZenxMetaInfo const& gen_avg_info)
{
#define PRINT_OBSV(obsv, fs) fs<<"{" << obsv.score<<','<<obsv.bulkiness<<','<<obsv.burn<<','<<obsv.aggregate_height<<','<<obsv.holes<<"}"
#define PRINT_META(meta, fs) PRINT_OBSV(meta.obsv, fs); fs<<","<<meta.level<<","<<meta.high_score;
	//fstream.precision(std::numeric_limits<double>::max_digits10);
	fstream << 'G' << generation << "\t:{";
	for (unsigned char i = 0; i < sizeof(DNAArray) / sizeof(double); i++)
	{
		fstream << (dna.arr[i]);
		if (i != sizeof(DNAArray) / sizeof(double) - 1)
		{
			fstream << ",";
		}
	}
	fstream << "};\tbest_inf:";
	PRINT_META(best_zenx_info, fstream);
	fstream << "\tgen_inf:";
	PRINT_META(gen_avg_info, fstream);
	fstream << std::endl;
#undef PRINT_META
#undef PRINT_OBSV
}

void TetrisML::Zenxis::render()
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

