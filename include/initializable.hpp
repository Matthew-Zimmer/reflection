#pragma once
#include <utility>
#include <meta/meta.hpp>
#include <concept/concept.hpp>

namespace Slate
{
	/*
		Summary:
			Provides a nice way to call Init on all feature types
	*/
	template <typename Type, typename ... Types>
	class Initializable
	{
		class Checker
		{
		public:
			template <typename _Type, typename ... Args>
			auto operator()(_Type&& t, Args&& ... args) -> decltype(t.init(std::forward<Args>(args)...));
		};

		template <typename _Type>
		using Init_Type = typename std::decay_t<_Type>::Init_Type;

		class Is_Complex_Initiazable
		{
		public:
			template <typename _Type>
			static auto Test(_Type&& t) -> decltype(t.init(std::declval<Init_Type<_Type>>()));
		};

		template <typename Type1, typename Type2>
		inline void do_init(Type1&& t1, Type2&& t2)
		{
			t1.init(std::forward<Type2>(t2));
		}
	public:
		/*
			Args:
				args...: the arguments to call any of the Types... Init functions
			Summary:
				Calls Init on all Types which have Init defined with 
				the correct arguments given in args...
			Complexity:
				O(n) where n is the number of objects in Types
		*/
		template <typename ... Args>
		void init_all(Args&& ... args)
		{
			([&]() {
				if constexpr (Satisfies<Is_Complex_Initiazable, Types>)
				{
					using Current_Type = Types;
					using Current_Init_Type = Init_Type<Current_Type>;
					if constexpr ((std::is_same_v<std::decay_t<Args>, Current_Init_Type> || ...))
						([&]() {
							using A = std::decay_t<Args>;
							if constexpr (std::is_same_v<A, Current_Init_Type>)
								do_init(Meta::cast<Current_Type, Type>(*this), std::forward<Args>(args));
						}(), ...);
					else if constexpr (Has_Function<Types, Checker>)
						Meta::cast<Types, Type>(*this).init();
				}
				else
				{
					if constexpr (Has_Function<Types, Checker>)
						Meta::cast<Types, Type>(*this).init();
				}
			}(), ...);
		}
	};
}