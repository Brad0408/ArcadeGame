#pragma once
#include <cstdint>
#include <cstdlib>

//Object.h is created with a UUID

namespace AG
{

	class UUID
	{
	public:
		//Constructor class
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};

}

namespace std
{
	template <typename T> struct hash;

	template<>
	struct hash<AG::UUID>
	{
		std::size_t operator()(const AG::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};

}
