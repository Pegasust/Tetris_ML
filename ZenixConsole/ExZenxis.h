#pragma once
#include "../Zenix_ML/Zenxis.h"
namespace ZenixConsole
{
	class ExZenxis: public TetrisML::Zenxis
	{
	public:
		std::string get_serialize_str() const;
		void experiment(unsigned int const& n_exp, std::ofstream& store_stream, const std::string serialize_location);
	};
}

