#pragma once
#include <meta/meta.hpp>

namespace Slate
{
	namespace Variable
	{
		/*
			Summary:
				Helper class to reduce the boiler plate off of variable classes
		*/
		template <typename Type>
		class Base 
		{
		public:
			/*
				Summary:
					The type of the variable being stored
			*/
			using Variable_Type = Type;
		private:
			Variable_Type data;
		public:
			/*
				Summary:
					Contructor with the variable type
				Complexity:
					O(k) where k is the complexity of constructing the type, Variable_Type
			*/
			template <typename _Type>
			Base(_Type&& data) : data{ std::forward<_Type>(data) } 
			{}

			/*
				Summary:
					Default constructor if the variable_type supports it
				Complexity:
					O(1)
			*/
			Base() = default;

			/*
				Summary:
					Basic copy constructor
				Complexity:
					O(k) where k is the complexity of constructing the type, Variable_Type
			*/
			Base(Base<Type> const& b) : data{b.data}
			{}
			
			/*
				Summary:
					Basic move constructor does not steal data from b
				Complexity:
					O(k) where k is the complexity of constructing the type, Variable_Type
			*/
			Base(Base<Type>&& b) : data{ b.data }
			{}

			/*
				Return:
					The object being assigned
				Summary:
					Basic copy assignment operator
				Complexity:
					O(k) where k is the complexity of copy assigning the type, Variable_Type
			*/
			Base<Type>& operator=(Base<Type> const& b) 
			{
				data = b.data;
				return *this;
			}

			/*
				Return:
					The object being assigned
				Summary:
					Basic move assignment operator does not steal data from b
				Complexity:
					O(k) where k is the complexity of move assigning the type, Variable_Type
			*/
			Base<Type>& operator=(Base<Type>&& b)
			{
				data = b.data;
				return *this;
			}

			/*
				Return:
					The variable of this class
				Summary:
					General way of accessing the variable non const
				Complexity:
					O(1)
			*/
			Variable_Type& variable()
			{
				return data;
			}

			/*
				Return:
					The variable of this class
				Summary:
					General way of accessing the variable const
				Complexity:
					O(1)
			*/
			Variable_Type const& variable() const
			{
				return data;
			}
		};
	}
	namespace V = Variable;
}