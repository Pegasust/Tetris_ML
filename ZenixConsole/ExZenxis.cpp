#include "ExZenxis.h"

std::string ZenixConsole::ExZenxis::get_serialize_str() const
{
#define SERSTRNAME serialize_string
#define APPENDLN SERSTRNAME.append("\n");
	std::string SERSTRNAME = std::to_string(seed[0].long_expr);
	APPENDLN;
	SERSTRNAME += "P{";

	return SERSTRNAME;
#undef APPENDLN
#undef SERSTRNAME
}

void ZenixConsole::ExZenxis::experiment(unsigned int const& n_exp, std::ofstream& store_stream, const std::string serialize_location)
{
	Zenxis::experiment(n_exp, store_stream);
	std::ofstream zenxis_serialize_fstream(serialize_location);
}
