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
    using this_class_type = class_name;					                           

#define END(class_name)                                                            \
public:                                                                            \
    std::unordered_map<std::string, std::function<void(void*)>> virtual_table;     \
    std::unordered_map<std::string, bool> class_name##_##methods;                  \
    bool getMethod(std::string method_name) {                                      \
        auto method = class_name##_##methods.find(method_name);                    \
        return method != class_name##_##methods.end() && method->second;           \
    }                                                                              \
    static_assert(std::is_same<class_name, this_class_type>::value,			       \
                  "Bad END of class");                                             \
};                                                                                 \
                                                                   
#define DECLARE_METHOD(class_name, method_name, function)                          \
virtual_table[#method_name] = function;                                            \
class_name##_##methods[#method_name] = true;

#define VIRTUAL_CLASS_DERIVED(class_name, base_class_name)                         \
class class_name : public base_class_name {                                        \
public:                                                                            \
    class_name();											        			   \
    using this_class_type = class_name;					                           \
    using base_class_type = base_class_name;

#define END_DERIVE(class_name, base_class_name)                                    \
    std::unordered_map<std::string, bool> class_name##_##methods;                  \
    bool getMethod(std::string method_name) {                                      \
        auto method = class_name##_##methods.find(method_name);                    \
		if (method != class_name##_##methods.end()) {                              \
			return method->second;                                                 \
		} else {                                                                   \
			bool has_method = base_class_name::getMethod(method_name);             \
            class_name##_##methods[method_name] = has_method;                      \
            return has_method;                                                     \
		}                                                                          \
    }                                                                              \
    static_assert(std::is_same<this_class_type, class_name>::value,	               \
                  "Bad END of class");											   \
    static_assert(std::is_same<base_class_type, base_class_name>::value,		   \
                  "Bad END of class");\
};

#define CONSTRUCTOR_BEGIN(class_name)                                              \
class_name::class_name() {

#define CONSTRUCTOR_END                                                            \
};

#define VIRTUAL_CALL(class_name, method_name)									   \
assert(class_name->getMethod(#method_name));                                       \
class_name->virtual_table[#method_name](class_name);

VIRTUAL_CLASS(Base)
int a = 0;
END(Base)

CONSTRUCTOR_BEGIN(Base)
DECLARE_METHOD(Base, Both, [](void*) { std::cout << "Base::Both" << std::endl; })
DECLARE_METHOD(Base, OnlyBase, [](void*) { std::cout << "Base::OnlyBase" << std::endl; })
CONSTRUCTOR_END

VIRTUAL_CLASS_DERIVED(Derived, Base)
int a = 1;
int b = 2;
END_DERIVE(Derived, Base)

CONSTRUCTOR_BEGIN(Derived)
DECLARE_METHOD(Derived, Both, [](void* self) {
    auto deliver = reinterpret_cast<Derived*>(self);
    std::cout << "Derived::Both, b = " << deliver->b << std::endl;
})
DECLARE_METHOD(Derived, OnlyDerived, [](void*) { std::cout << "Derived::OnlyDerived" << std::endl; })
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
    // VIRTUAL_CALL(reallyDerived, OnlyDerived); // triggered assert

    return 0;
}