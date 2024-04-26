/**
* @file Collision.cpp
*/
#include "Collision.h"
#include <algorithm>
/**
* AABB同士の交差判定
*
* @param a           判定対象のAABBその1
* @param b           判定対象のAABBその2
* @param penetration 貫通ベクトル
*
* @retval true  交差している
* @retval false 交差していない
*/
bool Intersect(const AABB& a, const AABB& b, vec3& penetration)
{
	// aの左側面がbの右側面より右にあるなら、交差していない
	const float dx0 = b.max.x - a.min.x;
	if (dx0 <= 0) {
		return false;

	}
	// aの右側面がbの左側面より左にあるなら、交差していない
	const float dx1 = a.max.x - b.min.x;
	if (dx1 <= 0) {
		return false;

	}

	// aの下面がbの上面より上にあるなら、交差していない
	const float dy0 = b.max.y - a.min.y;
	if (dy0 <= 0) {
		return false;

	}
	// aの上面がbの下面より下にあるなら、交差していない
	const float dy1 = a.max.y - b.min.y;
	if (dy1 <= 0) {
		return false;

	}

	// aの奥側面がbの手前側面より手前にあるなら、交差していない
	const float dz0 = b.max.z - a.min.z;
	if (dz0 <= 0) {
		return false;

	}
	// aの手前側面がbの奥側面より奥にあるなら、交差していない
	const float dz1 = a.max.z - b.min.z;
	if (dz1 <= 0) {
		return false;
		// この時点で交差が確定

	}

	// XYZの各軸について、重なっている距離が短い方向を選択
	vec3 length = { dx1, dy1, dz1 }; // 貫通距離の絶対値
	vec3 signedLength = length;      // 符号付きの貫通距離
	if (dx0 < dx1)
	{
		length.x = dx0;
		signedLength.x = -dx0;

	}
	if (dy0 < dy1) {
		length.y = dy0;
		signedLength.y = -dy0;

	}
	if (dz0 < dz1) {
		length.z = dz0;
		signedLength.z = -dz0;

	}

	// XYZのうち最も短い方向を選択
	if (length.x < length.y)
	{
		if (length.x < length.z)
		{
			penetration = { signedLength.x, 0, 0 };
			return true;

		}

	}
	else if (length.y < length.z) {
		penetration = { 0, signedLength.y, 0 };
		return true;

	}
	penetration = { 0, 0, signedLength.z };
	return true;
}

/**
* AABBと光線の交差判定
*
* @param[in]  aabb     判定対象のAABB
* @param[in]  ray      判定対象の光線
* @param[out] distance 光線がAABBと最初に交差する距離
*
* @retval true  交差している
* @retval false 交差していない
*/
bool Intersect(const AABB& aabb, const Ray& ray, float& distance)
{
	// 共通の交差範囲
	float tmin = 0;
	float tmax = FLT_MAX;

	// Xスラブとの交差判定
	if (!IntersectSlab(aabb.min.x, aabb.max.x,
		ray.start.x, ray.direction.x, tmin, tmax)) {
		return false; // 交差していない

	}

	// Yスラブとの交差判定
	if (!IntersectSlab(aabb.min.y, aabb.max.y,
		ray.start.y, ray.direction.y, tmin, tmax)) {
		return false; // 交差していない

	}

	// Zスラブとの交差判定
	if (!IntersectSlab(aabb.min.z, aabb.max.z,
		ray.start.z, ray.direction.z, tmin, tmax)) {
		return false; // 交差していない

	}

	// 交点までの距離を設定
	distance = tmin;
	return true; // 交差している
}

//始点が球体の外にあり、球体から離れていく方向に発射された光線を見分けて、
// 「交差していない」と判定することが大事
//負のtは光線の始点より後ろ側で交差している事になるため
//その時は、t=0にして「始点=交点」とする必要


/**
* 球体と光線の交差判定
*
* @param[in]  sphere   判定対象の球体
* @param[in]  ray      判定対象の光線
* @param[out] distance 光線が球体と最初に交差する距離
*
* @retval true  交差している
* @retval false 交差していない
*/
bool Intersect(const Sphere& sphere, const Ray& ray, float& distance)
{
	const vec3 m = ray.start - sphere.position;
	const float b = dot(m, ray.direction);
	const float c = dot(m, m) - sphere.radius * sphere.radius;

	// 光線の始点が球体の外にあり(c > 0)、光線が球体から離れていく方向に
	// 発射された(b > 0)場合、球体と光線は交差しない
	if (c > 0 && b > 0) {
		return false;

	}

	// 判別式が負の場合は解なし
	const float d = b * b - c; // 判別式
	if (d < 0) {
		return false;

	}

	// 最初に交差する位置を計算
	distance = -b - sqrt(d);

	// 負の位置は始点より手前を指し、光線が球体内から発射されたことを意味する
	// この場合、始点を「最初に交差する位置」とする
	if (distance < 0) {
		distance = 0;

	}
	return true;
}

/**
* スラブ(ある軸に垂直な2平面に囲まれた範囲)と光線の交差判定
*
* @param[in]  axis      スラブの軸ベクトル
* @param[in]  scale     スラブの幅
* @param[in]  start     光線の発射点(スラブの中心を原点とする)
* @param[in]  direction 光線の向き
* @param[out] tmin      AABBと光線の交差開始距離
* @param[out] tmax      AABBと光線の交差終了距離
*
* @retval true  交差している
* @retval false 交差していない
*/
bool IntersectSlab(const vec3& axis, float scale,
	const vec3& start, const vec3& direction,
	float& tmin, float& tmax)
{

	// 向きベクトルと発射点について、軸ベクトル方向の成分を求める
	//軸ベクトルの内積を求めたとき、値が0のとき
	//光線は軸ベクトルと直角で、光線はスラブと平行であることを意味する
	const float e = dot(axis, direction);

	const float f = dot(axis, start);

	//e:光線の向きベクトルをスラブの軸ベクトルに射影した値
	  // 光線がスラブと平行な場合
	  // 発射点がスラブ内にあれば交差している、外にあれば交差していない
	if (abs(e) < 0.0001f) {
		return (f >= -scale) && (f <= scale);

		//スラブまでの距離を、eで割ることで、
		  // 光線とスラブが交わる範囲の開始時刻と終了時刻を求める
		float t0 = (-scale - f) / e;
		float t1 = (scale - f) / e;


		// 時刻の早い側を開始時刻とする
		if (t0 > t1) {
			const float tmp = t0;
			t0 = t1;
			t1 = tmp;

		}

		// 以前の開始時刻と今回の開始時刻を比較し、遅いほうを選択
		if (t0 > tmin) {
			tmin = t0;

		}

		// 以前の終了時刻と今回の終了時刻を比較し、早いほうを選択
		if (t1 < tmax) {
			tmax = t1;

		}

		// 「開始時刻 <= 終了時刻」の場合は交差している
		return tmin <= tmax;
	}
}


/**
* OBBと光線の交差判定
*
* @param[in]  box      判定対象のOBB
* @param[in]  ray      判定対象の光線
* @param[out] distance 光線がOBBと最初に交差する距離
*
* @retval true  交差している
* @retval false 交差していない
*/
bool Intersect(const Box& box, const Ray& ray, float& distance)
{
	// スラブ中心を原点とした場合の光線の発射点を計算
	const vec3 start = ray.start - box.position;

	// スラブとの交差判定
	float tmin = 0;
	float tmax = FLT_MAX;

	//ベクトル型の添字演算子のオーバーロードにより、forループの追加！！
	for (int i = 0; i < 3; ++i) {
		if (!IntersectSlab(box.axis[i], box.scale[i],
			start, ray.direction, tmin, tmax)) {
			return false; // 交差していない

		}

	}

	// 交点までの距離を設定
	distance = tmin;
	return true; // 交差している
}
/**
* 球体と球体の交差判定
*
* @param a           判定対象の球体A
* @param b           判定対象の球体B
* @param penetration 貫通ベクトル
*
* @retval true  交差している
* @retval false 交差していない
*/
bool Intersect(const Sphere& a, const Sphere& b, vec3& penetration)
{
	// 中心の間の距離の2乗を計算
	const vec3 v = b.position - a.position; // aの中止からbの中心に向かうベクトル
	const float d2 = dot(v, v); // vの長さの2乗

	// d2が半径の合計より長い場合は交差していない
	const float r = a.radius + b.radius; // aとbの半径の合計
	if (d2 > r * r) { // 平方根を避けるため、2乗同士で比較する
		return false;

	}

	// 交差しているので貫通ベクトルを求める
	const float d = sqrt(d2); // 「長さの2乗」を「長さ」に変換
	const float t = (r - d) / d; // 「半径の合計 - 長さ」の「長さに対する比率」を求める
	penetration = v * t; // 貫通ベクトルを計算
	return true;
}

/**
* AABBから点への最近接点
*
* @param aabb  判定対象のAABB
* @param point 判定対象の点
*
* @return AABBの中で最も点に近い座標
*/
vec3 ClosestPoint(const AABB& aabb, const vec3& point)
{
	vec3 result;
	for (int i = 0; i < 3; ++i) {
		//clamp(クランプ)関数は、指定された範囲に制限された値を返します
		//二個目に指定した奴～三個目に指定した奴
		result[i] = std::clamp(point[i], aabb.min[i], aabb.max[i]);

	}
	return result;
}

/**
* AABBと球体の交差判定
*
* @param aabb        判定対象のAABB
* @param sphere      判定対象の球体
* @param penetration 貫通ベクトル
*
* @retval true  交差している
* @retval false 交差していない
*/
bool Intersect(const AABB& aabb, const Sphere& sphere, vec3& penetration)
{
	// 最近接点までの距離が球体の半径より長ければ、交差していない
	const vec3 p = ClosestPoint(aabb, sphere.position);
	const vec3 v = sphere.position - p;
	const float d2 = dot(v, v);
	if (d2 > sphere.radius * sphere.radius) {
		return false;

	}

	// 交差しているので、貫通ベクトルを求める
	//この時の貫通ベクトルは、球体の中心から再近設定に向かう直線
	if (d2 > 0) {
		// 距離が0より大きい場合、球体の中心はAABBの外側にある
			// 球体の中心座標から最近接点へ向かう方向から衝突したとみなす
		const float d = sqrt(d2);
		penetration = v * ((sphere.radius - d) / d);
		
	}
	else {
		// 距離が0の場合、球体の中心はAABBの内部にある
			// 貫通距離が最も短い面から衝突したとみなす
		int faceIndex = 0; // 貫通方向を示すインデックス
		float distance = FLT_MAX; // 貫通距離

		//forループによってX, Y, Z軸方向の面との距離を比較し、距離が最小となる面を選ぶ
		for (int i = 0; i < 3; ++i) {
			float t0 = p[i] - aabb.min[i];
			if (t0 < distance) {
				faceIndex = i * 2;
				distance = t0;

			}
			float t1 = aabb.max[i] - p[i];
			if (t1 < distance) {
				faceIndex = i * 2 + 1;
				distance = t1;

			}

		}

		// 「AABBが球体に対してどれだけ貫通しているか」を示すベクトルが欲しいので
		// 面の外向きのベクトルを使う
		static const vec3 faceNormals[] = {
		{-1, 0, 0 }, { 1, 0, 0 }, // -X, +X,
		{ 0,-1, 0 }, { 0, 1, 0 }, // -Y, +Y,
		{ 0, 0,-1 }, { 0, 0, 1 }, // -Z, +Z,
		};
		penetration = faceNormals[faceIndex] * distance;

	}
	return true;
}


/**
* OBBから点への最近接点
*
* @param box   判定対象のOBB
* @param point 判定対象の点
*
* @return 長方形の中で最も点に近い座標
*/
vec3 ClosestPoint(const Box& box, const vec3& point)
{
	/*
	*OBBと球体の交差判定
	* １．OBBの中心から球体中心ｈのベクトルを求める
	* ２．ベクトルをOBBのX,Y,Z軸に射影する
	* ３．射影した長さをOBBのスケールに制限する
	* 
	* 内積により、射影を求めるたりするため、あらかじめ定義
	*/

	// OBBから点に向かうベクトル
	const vec3 v = point - box.position;

	vec3 result = box.position;
	for (int i = 0; i < 3; ++i) {
		// ベクトルをOBBの軸に射影
		//OBBの場合ならaxis配列が利用できる
		float d = dot(v, box.axis[i]);

		// 射影で得られた値をOBBの範囲内に制限
		d = std::clamp(d, -box.scale[i], box.scale[i]);

		// 最近接点を更新
		result += d * box.axis[i];

	}
	return result;
}

/**
* OBBと球体の交差判定
*
* @param box         判定対象のOBB
* @param sphere      判定対象の球体
* @param penetration 貫通ベクトル
*
* @retval true  交差している
* @retval false 交差していない
*/
bool Intersect(const Box& box, const Sphere& sphere, vec3& penetration)
{
	// 最近接点から球体の中心までの距離が、球体の半径より大きければ衝突していない
	const vec3 p = ClosestPoint(box, sphere.position);
	const vec3 v = sphere.position - p;
	const float d2 = dot(v, v);
	if (d2 > sphere.radius * sphere.radius) {
		return false;

	}

	if (d2 > 0.00001f) {
		// 距離が0より大きい場合、球体の中心はOBBの外側にある
		// この場合、最近接から球体の中心へ向かう方向から衝突したとみなす
		const float d = sqrt(d2);
		penetration = v * ((sphere.radius - d) / d);

	}
	else {
		// 距離が0の場合、球体の中心はOBBの内部にある
		// この場合、貫通距離が最も短い面から衝突したとみなす
		const vec3 a = p - box.position; // OBB中心から球体中心へのベクトル
		int faceIndex = 0; // 貫通方向を示すインデックス
		float distance = FLT_MAX; // 貫通距離
		float sign = 1; // 貫通ベクトルの符号
		for (int i = 0; i < 3; ++i) {
			const float f = dot(box.axis[i], a); // aを軸ベクトルに射影
			const float t0 = f - (-box.scale[i]);
			if (t0 < distance) {
				faceIndex = i;
				distance = t0;
				sign = -1;
				
			}
			 const float t1 = box.scale[i] - f;
			if (t1 < distance) {
				faceIndex = i;
				distance = t1;
				sign = 1;
				
			}
			
		}
		 penetration = box.axis[faceIndex] * (distance * sign);

	}
	return true;
}
/**
* スラブ(ある軸に垂直な2平面に囲まれた範囲)と光線の交差判定
*
* @param[in]  min       スラブの開始距離
* @param[in]  max       スラブの終了距離
* @param[in]  start     光線の発射点
* @param[in]  direction 光線の向き
* @param[out] tmin      AABBと光線の交差開始距離
* @param[out] tmax      AABBと光線の交差終了距離
*
* @retval true  交差している
* @retval false 交差していない
*/
bool IntersectSlab(float min, float max, float start, float direction,
	float& tmin, float& tmax)
{
	// 光線がスラブと平行な場合
	// 発射点がスラブ内にあれば交差している、外にあれば交差していない
	if (abs(direction) < 0.0001f) {
		return (start >= min) && (start <= max);

	}

	// 光線とスラブが交差する開始時刻と終了時刻を求める
	float t0 = (min - start) / direction;
	float t1 = (max - start) / direction;

	// 時刻の早い側を開始時刻とする
	if (t0 > t1) {
		std::swap(t0, t1);

	}

	// 共通の公差範囲を求める
	// 以前の開始時刻と今回の開始時刻を比較し、遅いほうを選択
	if (t0 > tmin) {
		tmin = t0;

	}

	// 共通の公差範囲を求める
	// 以前の終了時刻と今回の終了時刻を比較し、早いほうを選択
	if (t1 < tmax) {
		tmax = t1;

	}

	// 「開始時刻 <= 終了時刻」の場合は交差している
	return tmin <= tmax;
}

