#include "reflection.hpp"
#include "variables.hpp"

using namespace Slate;

namespace V 
{
    using Variable::Base;

    class G : public Base<int>
    {
    public:
        auto& g() { return this->variable(); };
        auto const& g() const { return this->variable(); };
    };
    class H : public Base<float>
    {
    public:
        auto& h() { return this->variable(); };
        auto const& h() const { return this->variable(); };
    };
    class I : public Base<long>
    {
    public:
        auto& i() { return this->variable(); };
        auto const& i() const { return this->variable(); };
    };
}

template <typename T>
class A
{
public:
    using Required_Variables = Variables<::V::G>;
    void a(){}
};

template <typename T, typename ... Types>
class F
{
public:
    using Required_Features = Features<A>;
    void f(){}
};

template <typename T>
class B
{
public:
    using Required_Constructs = Constructs<F>;
    void b(){}
};

template <typename T>
class C
{
public:
    void c(){}
};

template <typename T, typename ... Types>
class D
{
public:
    void d(){}
};

template <typename T, typename ... Types>
class E
{
public:
    using Required_Variables = Variables<::V::H>;
    void e(){}
};

class Test : public Is<Test, Variables<::V::I>, Features<B, C>, Constructs<D, E>>
{};


int main()
{
    Test t;
    t.a();
    t.b();
    t.c();
    t.d();
    t.e();
    t.f();
    t.g();
    t.h();
    t.i();
}