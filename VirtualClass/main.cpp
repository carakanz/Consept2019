#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <functional>
#include <cassert>

#define VIRTUAL_CLASS(class_name)                                                  \
class class_name {                                                                 \
public:                                                                            \
    class_name();                                                                  \
private:                                                                           \
    using __this_class_type = class_name;					                       \

#define END(class_name)                                                            \
public:                                                                            \
    std::unordered_map<std::string, std::function<void(void)>> virtual_table;           \
    static_assert(std::is_same<class_name, __this_class_type>::value,			   \
                  "Bad END of class");                                             \
};                                                                                 \
                                                                   
#define DECLARE_METHOD(class_name, method_name, function)                          \
virtual_table[#method_name] = function;                                            \

#define CONSTRUCTOR_BEGIN(class_name)                                              \
class_name::class_name()                                                           \
{                                                                                  \

#define CONSTRUCTOR_END                                                            \
};

#define VIRTUAL_CLASS_DERIVED(derived_class_name, base_class_name)                 \
class derived_class_name : public base_class_name {                                \
public:                                                                            \
    derived_class_name();														   \
    using __this_class_type = derived_class_name;					               \
    using __base_class_type = base_class_name;					                   \

#define END_DERIVE(derived_class_name, base_class_name)                            \
    static_assert(std::is_same<__this_class_type, derived_class_name>::value,	   \
                  "Bad END of class");											   \
    static_assert(std::is_same<__base_class_type, base_class_name>::value,		   \
                  "Bad END of class");\
};

#define VIRTUAL_CALL(class_name, method_name)									   \
class_name->virtual_table[#method_name]();

VIRTUAL_CLASS(Base)
int a = 0;
END(Base)

CONSTRUCTOR_BEGIN(Base)
DECLARE_METHOD(Base, Both, []() { std::cout << "Base::Both" << std::endl; })
DECLARE_METHOD(Base, OnlyBase, []() { std::cout << "Base::OnlyBase" << std::endl; })
CONSTRUCTOR_END

VIRTUAL_CLASS_DERIVED(Derived, Base)
int a = 1;
END_DERIVE(Derived, Base)

CONSTRUCTOR_BEGIN(Derived)
DECLARE_METHOD(Derived, Both, []() { std::cout << "Derived::Both" << std::endl; })
DECLARE_METHOD(Derived, OnlyDerived, []() { std::cout << "Derived::OnlyDerived" << std::endl; })
CONSTRUCTOR_END

int main() {

    Base base;
    Derived derived;

    Base* reallyDerived = reinterpret_cast<Base*>(&derived);
    
    VIRTUAL_CALL((&base), Both);
    VIRTUAL_CALL((&base), OnlyBase);

    VIRTUAL_CALL((&derived), Both);
    VIRTUAL_CALL((&derived), OnlyBase);
    VIRTUAL_CALL((&derived), OnlyDerived);

    VIRTUAL_CALL(reallyDerived, Both);
    VIRTUAL_CALL(reallyDerived, OnlyBase);
    VIRTUAL_CALL(reallyDerived, OnlyDerived);

    return 0;
}