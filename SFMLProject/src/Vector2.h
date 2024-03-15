#pragma once
#include <arithmetic_concept.h>

namespace ArcadeGame
{

	template<typename T> requires arithmetic<T> 

	struct Vector2
	{
	public:

		//Define X and Y
		T x, y;

#pragma region constructors

		//Constructor class
		Vector2() : x(0), y(0) {}

		//Copy constructor
		Vector2(const Vector2&) = default;

		Vector2(T _x, T _y) : x(_x), y(_y) {}

		//New template U
		template<typename T> requires arithmetic<U>
		explicit Vector2(const Vector2<U>& in) : 
			x(static_cast<T>(in.x)), y(static_cast<T>(in.y)) {}

#pragma endregion

		//Squared Magnitude
		float GetSqarMagnitude() { return x * X + y * y; }

		//Regular Magnitude
		float GetMagnitude() { return sqrt(GetSqarMagnitude()); }

		//Normalise
		Vector2<T>& Normalise() { *this /= GetMagnitude(); return *this; }

		//Normalised
		Vector2<T> Normalised() { return *this / GetMagnitude(); }

		//Dot Product
		static float Dot(const Vector2<T>& lhs, const Vector2<T>& rhs)
			{ return (float)(lhs.x * rhs.x + rhs.x * rhs.y); }

		//Angle in Rads
		static float Angle(const Vector2<T>& lhs, const Vector2<T>& rhs)
			{ return acos(Dot(lhs.Normalised(), rhs.Normalised())); }



		static const Vector2 zero;
		static const Vector2 one;
		static const Vector2 up;
		static const Vector2 down;
		static const Vector2 left;
		static const Vector2 right;

	};

	//Negation - returns opposite 
	template<typename T> requires arithmetic<T>
	Vector2<T> operator-(const Vector2<T>& rhs) { return Vector2<T>(-rhs.x, rhs.y); }

	//Adding
	//+=
	template<typename T> requires arithmetic<T>
	Vector2<T>& operator+=(Vector2<T>& lhs, const Vector2<T>& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }

	//+
	template<typename T> requires arithmetic<T>
	Vector2<T> operator+(Vector2<T> lhs, const Vector2<T>& rhs) { return lhs += rhs; }



	//Subtract
	//-=
	template<typename T> requires arithmetic<T>
	Vector2<T>& operator-=(Vector2<T>& lhs, const Vector2<T>& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }

	//-
	template<typename T> requires arithmetic<T>
	Vector2<T> operator-(Vector2<T> lhs, const Vector2<T>& rhs) { return lhs -= rhs; }



	//Mutliply
	//*=
	template<typename T, typename U> requires arithmetic<T> and arithmetic<U>
	Vector2<T>& operator*=(Vector2<T>& v, const U a) { v.x *= a; v.y *= a; return v;}

	//* Vector on left
	template<typename T, typename U> requires arithmetic<T> and arithmetic<U>
	Vector2<T> operator*(Vector2<T>& v, const U a) { return v *= a; }

	//* Vector on right
	template<typename T, typename U> requires arithmetic<T> and arithmetic<U>
	Vector2<T> operator*(const U a, Vector2<T> v) { return v *= a; }


	//Divide
	//  div=
	template<typename T, typename U> requires arithmetic<T> and arithmetic<U>
	Vector2<T>& operator/=(Vector2<T>& v, const U a) { v.x /= a; v.y /= a; return v; }

	//   div Vector on left
	template<typename T, typename U> requires arithmetic<T> and arithmetic<U>
	Vector2<T> operator/(Vector2<T>& v, const U a) { return v /= a; }

	//  div Vector on right
	template<typename T, typename U> requires arithmetic<T> and arithmetic<U>
	Vector2<T> operator/(const U a, Vector2<T> v) { return v /= a; }


	//Check if 2 vectors are the same
	template<typename T> requires arithmetic<T>
	inline bool operator == (Vector2<T>& lhs, const Vector2<T>& rhs) 
	{ 
		Vector2<T> dist = lhs - rhs; 
		float mag = dist.x * dist.x + dist.y * dist.y;

		return mag < 9.99999944E-11f;
	}

	//Check if 2 vectors are not the same
	template<typename T> requires arithmetic<T>
	inline bool operator != (Vector2<T>& lhs, const Vector2<T>& rhs) { return !(lhs == rhs); }

	//Static const values
	//zero
	template<typename T> requires arithmetic<T>
	const Vector2<T> Vector2<T>::zero(static_cast<T>(0), static_cast<T>(0));

	//one
	template<typename T> requires arithmetic<T>
	const Vector2<T> Vector2<T>::one(static_cast<T>(1), static_cast<T>(1));

	//up
	template<typename T> requires arithmetic<T>
	const Vector2<T> Vector2<T>::up(static_cast<T>(0), static_cast<T>(1));

	//down
	template<typename T> requires arithmetic<T>
	const Vector2<T> Vector2<T>::down(static_cast<T>(0), static_cast<T>(-1));

	//left
	template<typename T> requires arithmetic<T>
	const Vector2<T> Vector2<T>::left(static_cast<T>(-1), static_cast<T>(0));

	//right
	template<typename T> requires arithmetic<T>
	const Vector2<T> Vector2<T>::right(static_cast<T>(1), static_cast<T>(0));


	//typedefs
	typedef Vector2<int>			Vector2i;
	typedef Vector2<unsigned int>	Vector2u;
	typedef Vector2<double>			Vector2d;
		
}

