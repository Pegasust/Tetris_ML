#pragma once
#include "lite_api.h"
#include "Zenoy_coefficients.h"
namespace ZenoyAgent
{
	typedef Zenoy::CoefficientVector UObservation;
	namespace TEngine = LGEngine;
	namespace TModule = LiteGameModule;
	//Cannot assign hscore nor burn
	UObservation get_unscaled_observation(const TEngine::TetrisField& field);
}