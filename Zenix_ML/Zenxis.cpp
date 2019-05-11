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
}

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
