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

		template <template <typename> typename ...>
		class Features
		{};

		template <template <typename, typename...> typename ...>
		class Constructs
		{};

		namespace Detail
		{
			namespace Detail::Complete_Features
			{
				template <typename, typename>
				class Complete_Features
				{};

				template <typename T, template <typename> typename ... Feature>
				class Complete_Features<Features<Feature...>, T>
				{
				public:
					using Type = Meta::Wrap<Feature<T>...>;
				};
			}

			template <typename Feature, typename Type>
			using Complete_Features = typename Detail::Complete_Features::Complete_Features<Feature, Type>::Type;

			namespace Detail::Decay_Constructs
			{
				template <typename, typename>
				class Decay_Constructs
				{};

				template <typename T, template <typename, typename...> typename ... Types>
				class Decay_Constructs<T, Constructs<Types...>>
				{
				public:
					using Type = Meta::Wrap<Types<T>...>;
				};
			}

			template <typename Constructs, typename Type>
			using Decay_Constructs = typename Detail::Decay_Constructs::Decay_Constructs<Type, Constructs>::Type;

			template <typename, typename, typename>
			class Complete_Construct
			{};

			template <typename T, typename ... Variable_Types, typename ... Feature_Types>
			class Complete_Construct<T, Meta::Wrap<Variable_Types...>, Meta::Wrap<Feature_Types...>>
			{
			public:
				template <typename>
				class Builder
				{};

				template <template <typename, typename...> typename Construct>
				class Builder<Construct<T>>
				{
				public:
					using Type = Meta::Wrap<Construct<T, Variable_Types..., Feature_Types...>>;
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
					using Type = Meta::Wrap<>;
				};

				template <typename T>
				class Variable_Checker<T, std::void_t<typename T::Required_Variables>>
				{
				public:
					using Type = Meta::Convert<typename T::Required_Variables>;
				};

				template <typename, typename=void>
				class Feature_Checker
				{
				public:
					using Type = Meta::Wrap<>;
				};

				template <typename T>
				class Feature_Checker<T, std::void_t<typename T::Required_Features>>
				{
				public:
					using Type = Complete_Features<typename T::Required_Features, Main_Type>;
				};

				template <typename, typename=void>
				class Construct_Checker
				{
				public:
					using Type = Meta::Wrap<>;
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
					using Type = Meta::Wrap<>;
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
					using Type = Meta::Wrap<>;
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
					using Type = Meta::Wrap<>;
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

					using Recursive_Features = Meta::Join_For_Each<Base_Features, Required_Types<Main_Type>::template Builder>;

					using Recursive_Constructs = Meta::Join_For_Each<Base_Constructs, Required_Types<Main_Type>::template Builder>;
				public:
					using All_Variables = Meta::Join<Base_Variables, Meta::Join_For_Each<Recursive_Features, All_Variables_Builder>, Meta::Join_For_Each<Recursive_Constructs, All_Variables_Builder>>;
					using All_Features = Meta::Join<Base_Features, Meta::Join_For_Each<Recursive_Features, All_Features_Builder>, Meta::Join_For_Each<Recursive_Constructs, All_Features_Builder>>;
					using All_Constructs = Meta::Join<Base_Constructs, Meta::Join_For_Each<Recursive_Features, All_Constructs_Builder>, Meta::Join_For_Each<Recursive_Constructs, All_Constructs_Builder>>;

					using Type = Builder<T>;
				};
			};


			template <typename T, typename All>
			using Extract_Variables = Meta::Join_For_Each<All, Required_Types<T>::template All_Variables_Builder>;

			template <typename T, typename All>
			using Extract_Features = Meta::Join_For_Each<All, Required_Types<T>::template All_Features_Builder>;

			template <typename T, typename All>
			using Extract_Constructs = Meta::Join_For_Each<All, Required_Types<T>::template All_Constructs_Builder>;

			template <typename>
			class Inherit
			{};

			template <typename ... Types>
			class Inherit<Meta::Wrap<Types...>> : public Types...
			{
			public:
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
			public:
				using Type = Inherit<Meta::Wrap<Variable_Types...>>;
			};

			template <typename T, template <typename> typename ... Feature_Types>
			class Is<T, Features<Feature_Types...>, void, void>
			{
				using Base_Features = Complete_Features<Features<Feature_Types...>, T>;
				using All_Types_From_Features = Meta::Join_For_Each<Base_Features, Required_Types<T>::template Builder>;

				using All_Variables = Meta::Unique<Extract_Variables<T, All_Types_From_Features>>;
				using All_Features = Meta::Unique<Meta::Join<Base_Features, Extract_Features<T, All_Types_From_Features>>>;
				using All_Decayed_Constructs = Meta::Unique<Extract_Constructs<T, All_Types_From_Features>>;

				using All_Constructs = Meta::Join_For_Each<All_Decayed_Constructs, Complete_Construct<T, All_Variables, All_Features>::template Builder>;
			public:
				using Type = Inherit<Meta::Join<All_Variables, All_Features, All_Constructs>>;
			};

			template <typename T, template <typename, typename...> typename ... Construct_Types>
			class Is<T, Constructs<Construct_Types...>, void, void>
			{
				using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, T>;
				using All_Types_From_Constructs = Meta::Join_For_Each<Base_Constructs, Required_Types<T>::template Builder>;

				using All_Variables = Meta::Unique<Extract_Variables<T, All_Types_From_Constructs>>;
				using All_Features = Meta::Unique<Extract_Features<T, All_Types_From_Constructs>>;
				using All_Decayed_Constructs = Meta::Unique<Meta::Join<Base_Constructs, Extract_Constructs<T, All_Types_From_Constructs>>>;

				using All_Constructs = Meta::Join_For_Each<All_Decayed_Constructs, Complete_Construct<T, All_Variables, All_Features>::template Builder>;
			public:
				using Type = Inherit<Meta::Join<All_Variables, All_Features, All_Constructs>>;
			};

			template <typename T, typename ... Variable_Types, template <typename> typename ... Feature_Types>
			class Is<T, Variables<Variable_Types...>, Features<Feature_Types...>, void>
			{
				using Base_Variables = Meta::Wrap<Variable_Types...>;
				using Base_Features = Complete_Features<Features<Feature_Types...>, T>;
				using All_Types_From_Features = Meta::Join_For_Each<Base_Features, Required_Types<T>::template Builder>;

				using All_Variables = Meta::Unique<Meta::Join<Base_Variables, Extract_Variables<T, All_Types_From_Features>>>;
				using All_Features = Meta::Unique<Meta::Join<Base_Features, Extract_Features<T, All_Types_From_Features>>>;
				using All_Decayed_Constructs = Meta::Unique<Extract_Constructs<T, All_Types_From_Features>>;
				
				using All_Constructs = Meta::Join_For_Each<All_Decayed_Constructs, Complete_Construct<T, All_Variables, All_Features>::template Builder>;
			public:
				using Type = Inherit<Meta::Join<All_Variables, All_Features, All_Constructs>>;
			};

			template <typename T, typename ... Variable_Types, template <typename, typename...> typename ... Construct_Types>
			class Is<T, Variables<Variable_Types...>, Constructs<Construct_Types...>, void>
			{
				using Base_Variables = Meta::Wrap<Variable_Types...>;
				using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, T>;
				using All_Types_From_Constructs = Meta::Join_For_Each<Base_Constructs, Required_Types<T>::template Builder>;

				using All_Variables = Meta::Unique<Meta::Join<Base_Variables, Extract_Variables<T, All_Types_From_Constructs>>>;
				using All_Features = Meta::Unique<Extract_Features<T, All_Types_From_Constructs>>;
				using All_Decayed_Constructs = Meta::Unique<Meta::Join<Base_Constructs, Extract_Constructs<T, All_Types_From_Constructs>>>;

				using All_Constructs = Meta::Join_For_Each<All_Decayed_Constructs, Complete_Construct<T, All_Variables, All_Features>::template Builder>;
			public:
				using Type = Inherit<Meta::Join<All_Variables, All_Features, All_Constructs>>;
			};

			template <typename T, template <typename> typename ... Feature_Types, template <typename, typename...> typename ... Construct_Types>
			class Is<T, Features<Feature_Types...>, Constructs<Construct_Types...>, void>
			{
				using Base_Features = Complete_Features<Features<Feature_Types...>, T>;
				using All_Types_From_Features = Meta::Join_For_Each<Base_Features, Required_Types<T>::template Builder>;
				using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, T>;
				using All_Types_From_Constructs = Meta::Join_For_Each<Base_Constructs, Required_Types<T>::template Builder>;

				using All_Variables = Meta::Unique<Meta::Join<Extract_Variables<T, All_Types_From_Features>, Extract_Variables<T, All_Types_From_Constructs>>>;
				using All_Features = Meta::Unique<Meta::Join<Base_Features, Extract_Features<T, All_Types_From_Features>, Extract_Constructs<T, All_Types_From_Constructs>>>;
				using All_Decayed_Constructs = Meta::Unique<Meta::Join<Base_Constructs,	Extract_Constructs<T, All_Types_From_Features>, Extract_Constructs<T, All_Types_From_Constructs>>>;

				using All_Constructs = Meta::Join_For_Each<All_Decayed_Constructs, Complete_Construct<T, All_Variables, All_Features>::template Builder>;
			public:
				using Type = Inherit<Meta::Join<All_Variables, All_Features, All_Constructs>>;
			};

			template <typename T, typename ... Variable_Types, template <typename> typename ... Feature_Types, template <typename, typename...> typename ... Construct_Types>
			class Is<T, Variables<Variable_Types...>, Features<Feature_Types...>, Constructs<Construct_Types...>>
			{
				using Base_Variables = Meta::Wrap<Variable_Types...>;
				using Base_Features = Complete_Features<Features<Feature_Types...>, T>;
				using All_Types_From_Features = Meta::Join_For_Each<Base_Features, Required_Types<T>::template Builder>;
				using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, T>;
				using All_Types_From_Constructs = Meta::Join_For_Each<Base_Constructs, Required_Types<T>::template Builder>;

				using All_Variables = Meta::Unique<Meta::Join<Base_Variables, Extract_Variables<T, All_Types_From_Features>, Extract_Variables<T, All_Types_From_Constructs>>>;
				using All_Features = Meta::Unique<Meta::Join<Base_Features, Extract_Features<T, All_Types_From_Features>, Extract_Features<T, All_Types_From_Constructs>>>;
				using All_Decayed_Constructs = Meta::Unique<Meta::Join<Base_Constructs, Extract_Constructs<T, All_Types_From_Features>, Extract_Constructs<T, All_Types_From_Constructs>>>;

				using All_Constructs = Meta::Join_For_Each<All_Decayed_Constructs, Complete_Construct<T, All_Variables, All_Features>::template Builder>;
			public:
				using Type = Inherit<Meta::Join<All_Variables, All_Features, All_Constructs>>;
			};
		}

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
		using Is = typename Detail::Is<Type1, Type2, Type3, Type4>::Type;
	}

	// namespace Imp::Reflection 
	// {
	// 	class Is_Variable 
	// 	{
	// 	public:
	// 		template <typename Type>
	// 		static auto Test(Type&& t) -> std::void_t<typename std::decay_t<Type>::Variable_Type, decltype(t.variable())>;
	// 	};
	// }

	// inline namespace Reflection 
	// {
	// 	/*
	// 		Summary:
	// 			Checks if a type is a variable or not
	// 	*/
	// 	template <typename Type>
	// 	constexpr bool Is_Variable = Satisfies<Imp::Reflection::Is_Variable, Type>;
	// }
}