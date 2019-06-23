#include "Reflection.hpp"
#include "Variables.hpp"

using namespace COMPANY_NAME;
using namespace Variable;
using namespace Meta;

using Var1 = class : public Base<int>{};
using Var2 = class : public Base<int>{};
using Var3 = class : public Base<int>{};

template <typename Type> class F1 {};
template <typename Type> class F2 {};

template <typename Type, typename ... Types> class C1 {};

class Obj : Is<Obj, Variables<Var1, Var2, Var3>, Features<F1>, Constructs<C1>>{};




int main()
{
	static_assert(std::is_same_v<Unique<Wrap<int>, Wrap<double>>, Wrap<int, double>>);
	//Obj o;
	return 0;
}