#pragma once
#include <utility>
#include <Meta/Meta.hpp>
#include <Concept/Concept.hpp>

namespace COMPANY_NAME 
{	
	inline namespace Reflection
	{
		/*
			Constraits:
				Types...: Is_Variable<Types>... is true
			Summary:
				Compile time container of variables
		*/
		template <typename ... Types>
		class Variables
		{};

		/*
			Constraits:
				Types...: Is_Feature<Types<void>>... is true
			Summary:
				Compile time container of features
		*/
		template <template <typename> typename ... Types>
		class Features
		{};

		/*
			Constraits:
				Types...: Is_Construct<Types<void>>... is true
			Summary:
				Compile time container of constructs
		*/
		template <template <typename, typename...> typename ... Types>
		class Constructs
		{};
	}

	namespace Imp::Meta
	{
		template <typename ... Types>
		class Is_Container<Variables<Types...>> : public std::true_type
		{};

		template <template <typename> typename ... Types>
		class Is_Container<Features<Types...>> : public std::true_type
		{};

		template <template <typename, typename...> typename ... Types>
		class Is_Container<Constructs<Types...>> : public std::true_type
		{};
	}

	namespace Imp::Reflection
	{
		using COMPANY_NAME::Meta::Wrap;
		using COMPANY_NAME::Meta::Join;
		using COMPANY_NAME::Meta::Unique;
		using COMPANY_NAME::Meta::For_Each;

		namespace Build_Feature_
		{
			template <typename, typename>
			class Build_Feature
			{};

			template <template <template <typename> typename...> typename Container, template <typename> typename ... Feature, typename Type>
			class Build_Feature<Container<Feature...>, Type>
			{
			public:
				using _Type = Wrap<Feature<Type>...>;
			};
		}

		template <typename Feature, typename Type>
		using Build_Feature = typename Build_Feature_::Build_Feature<Feature, Type>::_Type;

		namespace Build_Construct_
		{
			template <typename, typename, typename, typename>
			class Build_Construct
			{};

			template <template <template <typename, typename...> typename...> typename Container, template <typename, typename...> typename ... Construct, typename Type, template <typename ...> typename Variable_Container, typename ... Variable_Types, template <typename...> typename Feature_Container, typename ... Feature_Types>
			class Build_Construct<Container<Construct...>, Type, Variable_Container<Variable_Types...>, Feature_Container<Feature_Types...>>
			{
			public:
				using _Type = Wrap<Construct<Type, Variable_Types..., Feature_Types...>...>;
			};
		}

		template <typename Construct, typename Type, typename Variable_Types, typename Feature_Types>
		using Build_Construct = typename Build_Construct_::Build_Construct<Construct, Type, Variable_Types, Feature_Types>::_Type;

		namespace Decay_Constructs_
		{
			template <typename, typename>
			class Decay_Constructs
			{};

			template <typename _Type, template <typename, typename...> typename ... Types>
			class Decay_Constructs<Constructs<Types...>, _Type>
			{
			public:
				using Type = Wrap<Types<_Type>...>;
			};
		}

		template <typename Container, typename Type>
		using Decay_Constructs = typename Decay_Constructs_::Decay_Constructs<Container, Type>::Type;

		template <typename, typename, typename>
		class Upgrade_Constructs 
		{};

		template <typename _Type, template <typename...> typename Variable_Container, typename ... Variable_Types, template <typename...> typename Feature_Container, typename ... Feature_Types>
		class Upgrade_Constructs<_Type, Variable_Container<Variable_Types...>, Feature_Container<Feature_Types...>>
		{
		public:
			template <typename>
			class Builder
			{};

			template <template <typename, typename...> typename Construct>
			class Builder<Construct<_Type>>
			{
			public:
				using Type = Wrap<Construct<_Type, Variable_Types..., Feature_Types...>>;
			};
		};

		template <typename Main_Type>
		class Required_Types
		{
		public:
			template <typename, typename>
			class Variable_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename _Type>
			class Variable_Checker<_Type, std::void_t<typename _Type::Required_Variables>>
			{
			public:
				using Type = typename _Type::Required_Variables;
			};

			template <typename, typename>
			class Feature_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename _Type>
			class Feature_Checker<_Type, std::void_t<typename _Type::Required_Features>>
			{
			public:
				using Type = Build_Feature<typename _Type::Required_Features, Main_Type>;
			};

			template <typename, typename>
			class Construct_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename _Type>
			class Construct_Checker<_Type, std::void_t<typename _Type::Required_Constructs>>
			{
			public:
				using Type = Decay_Constructs<typename _Type::Required_Constructs, Main_Type>;
			};

			template <typename _Type, typename = void>
			class All_Variables_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename _Type>
			class All_Variables_Checker<_Type, std::void_t<typename _Type::All_Variables>>
			{
			public:
				using Type = typename _Type::All_Variables;
			};

			template <typename _Type>
			class All_Variables_Builder
			{
			public:
				using Type = typename All_Variables_Checker<_Type>::Type;
			};

			template <typename _Type, typename = void>
			class All_Features_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename _Type>
			class All_Features_Checker<_Type, std::void_t<typename _Type::All_Features>>
			{
			public:
				using Type = typename _Type::All_Features;
			};

			template <typename _Type>
			class All_Features_Builder
			{
			public:
				using Type = typename All_Features_Checker<_Type>::Type;
			};

			template <typename _Type, typename = void>
			class All_Constructs_Checker
			{
			public:
				using Type = Wrap<>;
			};

			template <typename _Type>
			class All_Constructs_Checker<_Type, std::void_t<typename _Type::All_Constructs>>
			{
			public:
				using Type = typename _Type::All_Constructs;
			};

			template <typename _Type>
			class All_Constructs_Builder
			{
			public:
				using Type = typename All_Constructs_Checker<_Type>::Type;
			};

			template <typename _Type>
			class Builder
			{
			private:
				using Base_Variables = typename Variable_Checker<_Type, void>::Type;
				using Base_Features = typename Feature_Checker<_Type, void>::Type;
				using Base_Constructs = typename Construct_Checker<_Type, void>::Type;

				using Recursive_Features = For_Each<Base_Features, Required_Types<Main_Type>::template Builder>;

				using Recursive_Constructs = For_Each<Base_Constructs, Required_Types<Main_Type>::template Builder>;
			public:
				using All_Variables = Join<Base_Variables, For_Each<Recursive_Features, All_Variables_Builder>, For_Each<Recursive_Constructs, All_Variables_Builder>>;
				using All_Features = Join<Base_Features, For_Each<Recursive_Features, All_Features_Builder>, For_Each<Recursive_Constructs, All_Features_Builder>>;
				using All_Constructs = Join<Base_Constructs, For_Each<Recursive_Features, All_Constructs_Builder>, For_Each<Recursive_Constructs, All_Constructs_Builder>>;

				using Type = Builder<_Type>;
			};
		};

		template <typename>
		class Inherit
		{};

		template <template <typename...> typename Container, typename ... Types>
		class Inherit<Container<Types...>> : public Types...
		{
		public:
			using Base = Inherit<Container<Types...>>;

			Inherit() = default;

			template <typename ... I_Types, typename = std::enable_if_t<static_cast<bool>(sizeof...(I_Types))>>
			Inherit(I_Types&& ... args) : I_Types{ [&]()
			{
				if constexpr(std::is_copy_constructible_v<I_Types>)
					return args;
				else
					return std::move(args);
			}() }...
			{}
		};

		template <typename, typename, typename, typename>
		class Is
		{};

		template <typename Type, typename ... Variable_Types>
		class Is<Type, Variables<Variable_Types...>, void, void>
		{
			using Base_Variables = Wrap<Variable_Types...>;
		public:
			using _Type = Inherit<Base_Variables>;
		};

		template <typename Type, template <typename> typename ... Feature_Types>
		class Is<Type, Features<Feature_Types...>, void, void>
		{
			using Base_Features = Build_Feature<Features<Feature_Types...>, Type>;
			using All_Types_From_Features = For_Each<Base_Features, Required_Types<Type>::template Builder>;

			using All_Variables = Unique<
				For_Each<All_Types_From_Features, Required_Types<Type>::template All_Variables_Builder>>;
			using All_Features = Unique<
				Base_Features,
				For_Each<All_Types_From_Features, Required_Types<Type>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
				For_Each<All_Types_From_Features, Required_Types<Type>::template All_Constructs_Builder>>;

			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<Type, All_Variables, All_Features>::template Builder>;
		public:
			using _Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
		};

		template <typename Type, template <typename, typename...> typename ... Construct_Types>
		class Is<Type, Constructs<Construct_Types...>, void, void>
		{
			using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, Type>;
			using All_Types_From_Constructs = For_Each<Base_Constructs, Required_Types<Type>::template Builder>;

			using All_Variables = Unique<
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Variables_Builder>>;
			using All_Features = Unique<
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
				Base_Constructs,
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Constructs_Builder>>;

			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<Type, All_Variables, All_Features>::template Builder>;
		public:
			using _Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
		};

		template <typename Type, typename ... Variable_Types, template <typename> typename ... Feature_Types>
		class Is<Type, Variables<Variable_Types...>, Features<Feature_Types...>, void>
		{
			using Base_Variables = Variables<Variable_Types...>;
			using Base_Features = Build_Feature<Features<Feature_Types...>, Type>;
			using All_Types_From_Features = For_Each<Base_Features, Required_Types<Type>::template Builder>;

			using All_Variables = Unique<
			 	Base_Variables,
			 	For_Each<All_Types_From_Features, Required_Types<Type>::template All_Variables_Builder>>;
			using All_Features = Unique<
			 	Base_Features,
			 	For_Each<All_Types_From_Features, Required_Types<Type>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
			 	For_Each<All_Types_From_Features, Required_Types<Type>::template All_Constructs_Builder>>;
			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<Type, All_Variables, All_Features>::template Builder>;
		public:
			using _Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
		};

		template <typename Type, typename ... Variable_Types, template <typename, typename...> typename ... Construct_Types>
		class Is<Type, Variables<Variable_Types...>, Constructs<Construct_Types...>, void>
		{
			using Base_Variables = Variables<Variable_Types...>;
			using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, Type>;
			using All_Types_From_Constructs = For_Each<Base_Constructs, Required_Types<Type>::template Builder>;

			using All_Variables = Unique<
				Base_Variables,
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Variables_Builder>>;
			using All_Features = Unique<
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
				Base_Constructs,
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Constructs_Builder>>;

			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<Type, All_Variables, All_Features>::template Builder>;
		public:
			using _Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
		};

		template <typename Type, template <typename> typename ... Feature_Types, template <typename, typename...> typename ... Construct_Types>
		class Is<Type, Features<Feature_Types...>, Constructs<Construct_Types...>, void>
		{
			using Base_Features = Build_Feature<Features<Feature_Types...>, Type>;
			using All_Types_From_Features = For_Each<Base_Features, Required_Types<Type>::template Builder>;
			using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, Type>;
			using All_Types_From_Constructs = For_Each<Base_Constructs, Required_Types<Type>::template Builder>;

			using All_Variables = Unique<
				For_Each<All_Types_From_Features, Required_Types<Type>::template All_Variables_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Variables_Builder>>;
			using All_Features = Unique<
				Base_Features,
				For_Each<All_Types_From_Features, Required_Types<Type>::template All_Features_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
				Base_Constructs,
				For_Each<All_Types_From_Features, Required_Types<Type>::template All_Constructs_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Constructs_Builder>>;

			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<Type, All_Variables, All_Features>::template Builder>;
		public:
			using _Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
		};

		template <typename Type, typename ... Variable_Types, template <typename> typename ... Feature_Types, template <typename, typename...> typename ... Construct_Types>
		class Is<Type, Variables<Variable_Types...>, Features<Feature_Types...>, Constructs<Construct_Types...>>
		{
			using Base_Variables = Variables<Variable_Types...>;
			using Base_Features = Build_Feature<Features<Feature_Types...>, Type>;
			using All_Types_From_Features = For_Each<Base_Features, Required_Types<Type>::template Builder>;
			using Base_Constructs = Decay_Constructs<Constructs<Construct_Types...>, Type>;
			using All_Types_From_Constructs = For_Each<Base_Constructs, Required_Types<Type>::template Builder>;

			using All_Variables = Unique<
				Base_Variables,
				For_Each<All_Types_From_Features, Required_Types<Type>::template All_Variables_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Variables_Builder>>;
			using All_Features = Unique<
				Base_Features,
				For_Each<All_Types_From_Features, Required_Types<Type>::template All_Features_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Features_Builder>>;
			using All_Decayed_Constructs = Unique<
				Base_Constructs,
				For_Each<All_Types_From_Features, Required_Types<Type>::template All_Constructs_Builder>,
				For_Each<All_Types_From_Constructs, Required_Types<Type>::template All_Constructs_Builder>>;

			using All_Constructs = For_Each<All_Decayed_Constructs, Upgrade_Constructs<Type, All_Variables, All_Features>::template Builder>;
		public:
			using _Type = Inherit<Join<All_Variables, All_Features, All_Constructs>>;
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
		using Is = typename Imp::Reflection::Is<Type1, Type2, Type3, Type4>::_Type;
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
