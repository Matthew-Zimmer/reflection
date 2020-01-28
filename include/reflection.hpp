#pragma once
#include <utility>
#include <meta/meta.hpp>
#include <concept/concept.hpp>
namespace Slate 
{	
	inline namespace Reflection
	{
		template <typename ... Types>
		class Variables
		{};

		template <template <typename> typename ... Types>
		class Features
		{};

		template <template <typename, typename...> typename ... Types>
		class Constructs
		{};
	}

	// namespace Imp::Meta
	// {
	// 	template <typename ... Types>
	// 	class Is_Container<Variables<Types...>> : public std::true_type
	// 	{};

	// 	template <template <typename> typename ... Types>
	// 	class Is_Container<Features<Types...>> : public std::true_type
	// 	{};

	// 	template <template <typename, typename...> typename ... Types>
	// 	class Is_Container<Constructs<Types...>> : public std::true_type
	// 	{};
	// }

	namespace Imp::Reflection
	{
		using namespace Slate::Meta;

		template <typename, typename>
		class Complete_Feature_I
		{};

		template <typename T, template <typename> typename ... Feature>
		class Complete_Feature_I<Features<Feature...>, T>
		{
		public:
			using Type = Wrap<Feature<T>...>;
		};

		template <typename Feature, typename Type>
		using Complete_Feature = typename Complete_Feature_I<Feature, Type>::Type;

		template <typename, typename>
		class Decay_Constructs_I
		{};

		template <typename T, template <typename, typename...> typename ... Types>
		class Decay_Constructs_I<T, Constructs<Types...>>
		{
		public:
			using Type = Wrap<Types<T>...>;
		};

		template <typename Constructs, typename Type>
		using Decay_Constructs = typename Decay_Constructs_I<Type, Constructs>::Type;

		template <typename, typename, typename>
		class Upgrade_Constructs 
		{};

		template <typename T, typename ... Variable_Types, typename ... Feature_Types>
		class Upgrade_Constructs<T, Wrap<Variable_Types...>, Wrap<Feature_Types...>>
		{
		public:
			template <typename>
			class Builder
			{};

			template <template <typename, typename...> typename Construct>
			class Builder<Construct<T>>
			{
			public:
				using Type = Wrap<Construct<T, Variable_Types..., Feature_Types...>>;
			};
		};

		template <typename Main_Type>
		class Required_Types
		{
		public:
			template <typename, typename=void>
			class Variable_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename T>
			class Variable_Checker<T, std::void_t<typename T::Required_Variables>>
			{
			public:
				using Type = typename T::Required_Variables;
			};

			template <typename, typename=void>
			class Feature_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename T>
			class Feature_Checker<T, std::void_t<typename T::Required_Features>>
			{
			public:
				using Type = Complete_Feature<typename T::Required_Features, Main_Type>;
			};

			template <typename, typename=void>
			class Construct_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename T>
			class Construct_Checker<T, std::void_t<typename T::Required_Constructs>>
			{
			public:
				using Type = Decay_Constructs<typename T::Required_Constructs, Main_Type>;
			};

			template <typename, typename = void>
			class All_Variables_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename T>
			class All_Variables_Checker<T, std::void_t<typename T::All_Variables>>
			{
			public:
				using Type = typename T::All_Variables;
			};

			template <typename T>
			class All_Variables_Builder
			{
			public:
				using Type = typename All_Variables_Checker<T>::Type;
			};

			template <typename, typename = void>
			class All_Features_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename T>
			class All_Features_Checker<T, std::void_t<typename T::All_Features>>
			{
			public:
				using Type = typename T::All_Features;
			};

			template <typename T>
			class All_Features_Builder
			{
			public:
				using Type = typename All_Features_Checker<T>::Type;
			};

			template <typename, typename = void>
			class All_Constructs_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename T>
			class All_Constructs_Checker<T, std::void_t<typename T::All_Constructs>>
			{
			public:
				using Type = typename T::All_Constructs;
			};

			template <typename T>
			class All_Constructs_Builder
			{
			public:
				using Type = typename All_Constructs_Checker<T>::Type;
			};

			template <typename T>
			class Builder
			{
			private:
				using Base_Variables = typename Variable_Checker<T, void>::Type;
				using Base_Features = typename Feature_Checker<T, void>::Type;
				using Base_Constructs = typename Construct_Checker<T, void>::Type;

				using Recursive_Features = For_Each<Base_Features, Required_Types<Main_Type>::template Builder>;

				using Recursive_Constructs = For_Each<Base_Constructs, Required_Types<Main_Type>::template Builder>;
			public:
				using All_Variables = Join<Base_Variables, For_Each<Recursive_Features, All_Variables_Builder>, For_Each<Recursive_Constructs, All_Variables_Builder>>;
				using All_Features = Join<Base_Features, For_Each<Recursive_Features, All_Features_Builder>, For_Each<Recursive_Constructs, All_Features_Builder>>;
				using All_Constructs = Join<Base_Constructs, For_Each<Recursive_Features, All_Constructs_Builder>, For_Each<Recursive_Constructs, All_Constructs_Builder>>;

				using Type = Builder<T>;
			};
		};

		template <typename>
		class Inherit
		{};

		template <typename ... Types>
		class Inherit<Wrap<Types...>> : public Types...
		{
		public:
			using Base = Inherit<Wrap<Types...>>;

			Inherit() = default;

			template <typename ... Args, typename = std::enable_if_t<static_cast<bool>(sizeof...(Args))>>
			Inherit(Args&& ... args) : Args{ std::forward<Args>(args) }...
			{}
		};

		template <typename, typename, typename, typename>
		class Is
		{};

		template <typename T, typename ... Variable_Types>
		class Is<T, Variables<Variable_Types...>, void, void>
		{
			using Base_Variables = Wrap<Variable_Types...>;
		public:
			using Type = Inherit<Convert<Base_Variables>>;
		};

		template <typename T, template <typename> typename ... Feature_Types>
		class Is<T, Features<Feature_Types...>, void, void>
		{
			using Base_Features = Complete_Feature<Features<Feature_Types...>, T>;
			using All_Types_From_Features = For_Each<Base_Features, Required_Types<T>::template Builder>;

			using All_Variables = Unique<
				For_Each<All_Types_From_Features, Required_Types<T>::template All_Variables_Builder>>;
			using All_Features = Unique<
				Base_Features,
				For_Each<All_Types_From_Features, Required_Types<T>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
				For_Each<All_Types_From_Features, Required_Types<T>::template All_Constructs_Builder>>;

			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<T, All_Variables, All_Features>::template Builder>;
		public:
			using Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
		};

		template <typename T, template <typename, typename...> typename ... Construct_Types>
		class Is<T, Constructs<Construct_Types...>, void, void>
		{
			using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, T>;
			using All_Types_From_Constructs = For_Each<Base_Constructs, Required_Types<T>::template Builder>;

			using All_Variables = Unique<
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Variables_Builder>>;
			using All_Features = Unique<
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
				Base_Constructs,
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Constructs_Builder>>;

			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<T, All_Variables, All_Features>::template Builder>;
		public:
			using Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
		};

		template <typename T, typename ... Variable_Types, template <typename> typename ... Feature_Types>
		class Is<T, Variables<Variable_Types...>, Features<Feature_Types...>, void>
		{
			using Base_Variables = Wrap<Variable_Types...>;
			using Base_Features = Complete_Feature<Features<Feature_Types...>, T>;
			using All_Types_From_Features = For_Each<Base_Features, Required_Types<T>::template Builder>;

			using All_Variables = Unique<
			 	Base_Variables,
			 	For_Each<All_Types_From_Features, Required_Types<T>::template All_Variables_Builder>>;
			using All_Features = Unique<
			 	Base_Features,
			 	For_Each<All_Types_From_Features, Required_Types<T>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
			 	For_Each<All_Types_From_Features, Required_Types<T>::template All_Constructs_Builder>>;
			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<T, All_Variables, All_Features>::template Builder>;
		public:
			using Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
		};

		template <typename T, typename ... Variable_Types, template <typename, typename...> typename ... Construct_Types>
		class Is<T, Variables<Variable_Types...>, Constructs<Construct_Types...>, void>
		{
			using Base_Variables = Wrap<Variable_Types...>;
			using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, T>;
			using All_Types_From_Constructs = For_Each<Base_Constructs, Required_Types<T>::template Builder>;

			using All_Variables = Unique<
				Base_Variables,
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Variables_Builder>>;
			using All_Features = Unique<
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
				Base_Constructs,
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Constructs_Builder>>;

			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<T, All_Variables, All_Features>::template Builder>;
		public:
			using Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
		};

		template <typename T, template <typename> typename ... Feature_Types, template <typename, typename...> typename ... Construct_Types>
		class Is<T, Features<Feature_Types...>, Constructs<Construct_Types...>, void>
		{
			using Base_Features = Complete_Feature<Features<Feature_Types...>, T>;
			using All_Types_From_Features = For_Each<Base_Features, Required_Types<T>::template Builder>;
			using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, T>;
			using All_Types_From_Constructs = For_Each<Base_Constructs, Required_Types<T>::template Builder>;

			using All_Variables = Unique<
				For_Each<All_Types_From_Features, Required_Types<T>::template All_Variables_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Variables_Builder>>;
			using All_Features = Unique<
				Base_Features,
				For_Each<All_Types_From_Features, Required_Types<T>::template All_Features_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
				Base_Constructs,
				For_Each<All_Types_From_Features, Required_Types<T>::template All_Constructs_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Constructs_Builder>>;

			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<T, All_Variables, All_Features>::template Builder>;
		public:
			using Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
		};

		template <typename T, typename ... Variable_Types, template <typename> typename ... Feature_Types, template <typename, typename...> typename ... Construct_Types>
		class Is<T, Variables<Variable_Types...>, Features<Feature_Types...>, Constructs<Construct_Types...>>
		{
			using Base_Variables = Wrap<Variable_Types...>;
			using Base_Features = Complete_Feature<Features<Feature_Types...>, T>;
			using All_Types_From_Features = For_Each<Base_Features, Required_Types<T>::template Builder>;
			using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, T>;
			using All_Types_From_Constructs = For_Each<Base_Constructs, Required_Types<T>::template Builder>;

			using All_Variables = Unique<
				Base_Variables,
				For_Each<All_Types_From_Features, Required_Types<T>::template All_Variables_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Variables_Builder>>;
			using All_Features = Unique<
				Base_Features,
				For_Each<All_Types_From_Features, Required_Types<T>::template All_Features_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
				Base_Constructs,
				For_Each<All_Types_From_Features, Required_Types<T>::template All_Constructs_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<T>::template All_Constructs_Builder>>;

			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<T, All_Variables, All_Features>::template Builder>;
		public:
			using Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
		};
	}
	
	inline namespace Reflection
	{
		/*
			Notes:
				Template Types:
					T : Type of the object inheriting Is
					V : V is a compile time container of Variables
					F : F is a compile time container of Features
					C : C is a compile time container of Constructs
				Valid combinations:
					1. T V
					2. T F
					3. T C
					4. T V F 
					5. T V C
					6. T F C
					7. T V F C
			Summary:
				Helper type to use the reflection system
		*/
		template <typename Type1, typename Type2 = void, typename Type3 = void, typename Type4 = void>
		using Is = typename Imp::Reflection::Is<Type1, Type2, Type3, Type4>::Type;
	}

	namespace Imp::Reflection 
	{
		class Is_Variable 
		{
		public:
			template <typename Type>
			static auto Test(Type&& t) -> std::void_t<typename std::decay_t<Type>::Variable_Type, decltype(t.Variable())>;
		};
	}

	inline namespace Reflection 
	{
		/*
			Summary:
				Checks if a type is a variable or not
		*/
		template <typename Type>
		constexpr bool Is_Variable = Satisfies<Imp::Reflection::Is_Variable, Type>;
	}
}