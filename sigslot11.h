#ifndef SIGSLOT11_H
#define SIGSLOT11_H

#include <list>

namespace sigslot11
{

    template<typename ReturnType> class Function;
    
    template<typename ReturnType, typename... Args>
    class Function<ReturnType(Args...)>
    {
    public:
        template<ReturnType (*functionPtr)(Args...)>
        static inline Function bind()
        {
            return { nullptr, [] (void *, Args... args) {
                return (*functionPtr)(args...); }
            };
        }
    
        template<typename Class, ReturnType (Class::*methodPtr)(Args...)>
        static inline Function bind(Class *pointer)
        {
            return { pointer, [] (void *classPtr, Args... args) {
                return (static_cast<Class*>(classPtr)->*methodPtr)(args...); }
            };
        }
    
        template<typename Class, ReturnType (Class::*methodPtr)(Args...) const>
        static inline Function bind(Class *pointer)
        {
            return { pointer, [] (void *classPtr, Args... args) {
                return (static_cast<const Class*>(classPtr)->*methodPtr)(args...); }
            };
        }

        ReturnType operator() (Args&&... args) const
        {
            return (*functionPtr)(classPtr, args...);
        }
        
    private:
        void *classPtr;
        ReturnType (*functionPtr)(void *, Args...);

        template<typename ClassPtr, typename FunctionPtr>
        Function(ClassPtr &&classPtr, FunctionPtr &&functionPtr) :
                 classPtr(std::forward<ClassPtr>(classPtr)),
                 functionPtr(std::forward<FunctionPtr>(functionPtr)) {}
    };

    template <typename ReturnType> class Signal;
    
    template <typename ReturnType, typename... Args>
    class Signal<ReturnType(Args...)>
    {
    public:
        template <ReturnType (*functionPtr)(Args...)>
        void connect()
        {
            slots.push_back(Function::template bind<functionPtr>());
        }
    
        template <typename Class, ReturnType (Class::*methodPtr)(Args...)>
        void connect(Class *instance)
        {
            slots.push_back(Function::template bind<Class, methodPtr>(instance));
        }
    
        template <typename Class, ReturnType (Class::*methodPtr)(Args...) const>
        void connect(Class *instance)
        {
            slots.push_back(Function::template bind<Class, methodPtr>(instance));
        }
    
        void disconnectAll()
        {
            slots.clear();
        }

        void operator()(Args... args) const
        {
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
