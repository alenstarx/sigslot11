#ifndef INCLUDED_SIGSLOT11_HPP
#define INCLUDED_SIGSLOT11_HPP

#include <list>

namespace sigslot11 {

    template<typename ReturnType>
    class Function;

    template<typename ReturnType, typename... Args>
    class Function<ReturnType(Args...)> {
    public:
        template<ReturnType (*function_ptr)(Args...)>
        static inline Function bind() {
            return { nullptr, [] (void *, Args... args) {
                return (*function_ptr)(args...); }
            };
        }

        template<typename Class, ReturnType (Class::*method_ptr)(Args...)>
        static inline Function bind(Class *pointer) {
            return { pointer, [] (void *class_ptr, Args... args) {
                return (static_cast<Class*>(class_ptr)->*method_ptr)(args...); }
            };
        }

        template<typename Class, ReturnType (Class::*method_ptr)(Args...) const>
        static inline Function bind(Class *pointer) {
            return { pointer, [] (void *class_ptr, Args... args) {
                return (static_cast<const Class*>(class_ptr)->*method_ptr)(args...); }
            };
        }

        ReturnType operator()(Args&&... args) const {
            return (*function_ptr)(class_ptr, args...);
        }

    private:
        void *class_ptr;
        ReturnType (*function_ptr)(void *, Args...);

        template<typename ClassPtr, typename FunctionPtr>
        Function(ClassPtr &&class_ptr, FunctionPtr &&function_ptr) :
            class_ptr(std::forward<ClassPtr>(class_ptr)),
            function_ptr(std::forward<FunctionPtr>(function_ptr)) {}
    };

    template <typename ReturnType>
    class Signal;

    template <typename ReturnType, typename... Args>
    class Signal<ReturnType(Args...)> {
    public:
        template <ReturnType (*function_ptr)(Args...)>
        void connect() {
            slots.push_back(Function::template bind<function_ptr>());
        }

        template <typename Class, ReturnType (Class::*method_ptr)(Args...)>
        void connect(Class *instance) {
            slots.push_back(Function::template bind<Class, method_ptr>(instance));
        }

        template <typename Class, ReturnType (Class::*method_ptr)(Args...) const>
        void connect(Class *instance) {
            slots.push_back(Function::template bind<Class, method_ptr>(instance));
        }

        void disconnect_all() {
            slots.clear();
        }

        void operator()(Args... args) const {
            for (const auto &slot : slots) {
                slot(std::forward<Args>(args)...);
            }
        }

    private:
        typedef sigslot11::Function<ReturnType(Args...)> Function;
        std::list<Function> slots;
    };

}

#endif
