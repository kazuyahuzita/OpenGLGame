/**
* @file Random.cpp
*/
#include "Random.h"
 #include <random>

namespace Random {

	// ���������I�u�W�F�N�g
	std::random_device rd;//�w�肵���͈͂̈�l�������擾����̂ɕK�v
	std::mt19937 randomEngine(rd());

	void Initialize(int seed)
	{
		randomEngine.seed(seed);
	}

	//�w�肵���͈͂̈�l�������擾����ɂ�uniform_real_distribution
	//uniform�u��l�ȁv�Areal�u�����́v�Adistribution�u���z�v
	//uniform_real_distribution->�u��l�Ȏ����̕��z�v
	//�R���X�g���N�^�ōŏ��l�ƍő�l���w��


	float Value()
	{
		return std::uniform_real_distribution<float>(0, 1)(randomEngine);
	}

	float Range(float min, float max)
	{
		return std::uniform_real_distribution<float>(min, max)(randomEngine);
	}


} // namespace Random