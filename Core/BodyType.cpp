#include "BodyType.h"
template<typename T>
LGEngine::BodyType LGEngine::body_type_val(T input)
{
	return (LGEngine::BodyType) ((input % 7) + 1);
}

template<>
LGEngine::BodyType LGEngine::body_type_val<TMath::GameRNG::RNGSeed>(TMath::GameRNG::RNGSeed input)
{
	//TODO: can definitely make this a less biased algorithm
	return (LGEngine::BodyType) ((input % 7) + 1);
}