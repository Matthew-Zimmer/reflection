#pragma once
#include "reflection.hpp"

namespace Slate::Operators
{
	template <typename Type>
	class Addable
	{};

	template <typename Type>
	class Subtractable
	{};

	template <typename Type>
	class Multipliable
	{};

	template <typename Type>
	class Dividable
	{};

	template <typename Type>
	class Modulable
	{};

	template <typename Type>
	class Comparable
	{};

	template <typename Type>
	class Mathematically_Operatorable
	{
	public:
		using Required_Features = Features<Addable, Subtractable, Multipliable, Dividable, Comparable>;
	};

	template <typename Type>
	auto operator>(Type const& obj1, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Comparable<Type>, Type>, bool>	
	{
		return obj2 < obj1;
	}
	
	template <typename Type>
	auto operator==(Type const& obj1, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Comparable<Type>, Type>, bool>
	{
		return !(obj1 < obj2 || obj2 < obj1);
	}
	
	template <typename Type>
	auto operator!=(Type const& obj1, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Comparable<Type>, Type>, bool>
	{
		return !(obj1 == obj2);
	}
	
	template <typename Type>
	auto operator<=(Type const& obj1, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Comparable<Type>, Type>, bool>
	{
		return obj1 < obj2 || obj1 == obj2;
	}
	
	template <typename Type>
	auto operator>=(Type const& obj1, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Comparable<Type>, Type>, bool> 
	{
		return obj1 > obj2 || obj1 == obj2;
	}

	template <typename Type>
	auto operator+=(Type&& obj1, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Addable<std::remove_reference_t<Type>>, std::remove_reference_t<Type>>, Type&&>
	{
		return static_cast<Type&&>(obj1 += obj2);
	}

	template <typename Type>
	auto operator-=(Type&& obj1, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Subtractable<std::remove_reference_t<Type>>, std::remove_reference_t<Type>>, Type&&>
	{
		return static_cast<Type&&>(obj1 -= obj2);
	}

	template <typename Type>
	auto operator*=(Type&& obj1, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Multipliable<std::remove_reference_t<Type>>, std::remove_reference_t<Type>>, Type&&>
	{
		return static_cast<Type&&>(obj1 *= obj2);
	}

	template <typename Type>
	auto operator/=(Type&& obj1, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Dividable<std::remove_reference_t<Type>>, std::remove_reference_t<Type>>, Type&&>
	{
		return static_cast<Type&&>(obj1 /= obj2);
	}

	template <typename Type>
	auto operator%=(Type&& obj1, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Modulable<std::remove_reference_t<Type>>, std::remove_reference_t<Type>>, Type&&>
	{
		return static_cast<Type&&>(obj1 %= obj2);
	}

	template <typename Type>
	auto operator+(Type obj, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Addable<Type>, Type>, Type> 
	{
		return obj += obj2;
	}

	template <typename Type>
	auto operator-(Type obj, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Subtractable<Type>, Type>, Type> 
	{
		return obj -= obj2;
	}

	template <typename Type>
	auto operator*(Type obj, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Multipliable<Type>, Type>, Type> 
	{
		return obj *= obj2;
	}

	template <typename Type>
	auto operator/(Type obj, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Dividable<Type>, Type>, Type> 
	{
		return obj /= obj2;
	}

	template <typename Type>
	auto operator%(Type obj, Type const& obj2) -> std::enable_if_t<std::is_base_of_v<Modulable<Type>, Type>, Type> 
	{
		return obj %= obj2;
	}
} // namespace Slate::Rvalue