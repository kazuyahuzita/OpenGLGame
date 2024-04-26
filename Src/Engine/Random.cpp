/**
* @file Random.cpp
*/
#include "Random.h"
 #include <random>

namespace Random {

	// 乱数生成オブジェクト
	std::random_device rd;//指定した範囲の一様乱数を取得するのに必要
	std::mt19937 randomEngine(rd());

	void Initialize(int seed)
	{
		randomEngine.seed(seed);
	}

	//指定した範囲の一様乱数を取得するにはuniform_real_distribution
	//uniform「一様な」、real「実数の」、distribution「分布」
	//uniform_real_distribution->「一様な実数の分布」
	//コンストラクタで最小値と最大値を指定


	float Value()
	{
		return std::uniform_real_distribution<float>(0, 1)(randomEngine);
	}

	float Range(float min, float max)
	{
		return std::uniform_real_distribution<float>(min, max)(randomEngine);
	}


} // namespace Random