/**
* @file Random.h
*/
#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

/**
 ���������@�\���i�[���閼�O���
*/
namespace Random
{
	
	/**
	* ����������������
	*
	* @param seed �����̎�
	*/
	void Initialize(int seed);
	
	/**
	* 0.0�`1.0�͈̔͂̈�l�������擾����
	*
	* @return ������������
	*/
	float Value();
	
	/**
	* �w�肳�ꂽ�͈̔͂̈�l�������擾����
	*
	* @param min �����͈͂̍ŏ��l
	* @param max �����͈͂̍ő�l
	*
	* @return ������������
	*/
	float Range(float min, float max);
	
	
} // namespace Random

#endif // RANDOM_H_INCLUDED