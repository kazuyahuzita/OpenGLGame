/**
* @file Random.h
*/
#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

/**
 乱数生成機能を格納する名前空間
*/
namespace Random
{
	
	/**
	* 乱数を初期化する
	*
	* @param seed 乱数の種
	*/
	void Initialize(int seed);
	
	/**
	* 0.0～1.0の範囲の一様乱数を取得する
	*
	* @return 生成した乱数
	*/
	float Value();
	
	/**
	* 指定されたの範囲の一様乱数を取得する
	*
	* @param min 生成範囲の最小値
	* @param max 生成範囲の最大値
	*
	* @return 生成した乱数
	*/
	float Range(float min, float max);
	
	
} // namespace Random

#endif // RANDOM_H_INCLUDED