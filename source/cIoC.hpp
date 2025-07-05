///************************* OUTS HOMEWORK ****************************************
#ifndef CIOC_HPP
#define CIOC_HPP

#include <string>
#include <memory>
#include <functional>
#include <map>

#include "iCommand.hpp"
#include "cException.hpp"

// cIoCImpl-container
// with only entry
// T *Resolve(std::string s, Args... args);
// if s is "Register" this means query to register factory or factory method
// Registering a factory is performing by the function 
// iCommand *registerFactory(std::string scope, const cFactory &);
// class cFactory  is a wrapper of std::map<string,void*> by key = object name and 
// void * is a factory method.
// User should run Execute() to perform real registering

extern void DBG(int t);
extern void DBG1( void * method);
extern void DBG2(const char *sz );

class cIoCImpl;
class cFactory;

struct iRegisterFactory : public iCommand
{
    iRegisterFactory(cIoCImpl& ioc, const std::string& scope, const cFactory& f);

    void Execute() override;

    const char* Type()  override { return typeid(*this).name(); }

    cIoCImpl* ioc;
    const std::string scope;
    std::shared_ptr<cFactory> shptrFactory;
};

struct iRegisterFactoryMethod : public iCommand
{
    iRegisterFactoryMethod(cIoCImpl& ioc, const std::string& scope, const std::string& objName, const void* f)
        : ioc(&ioc), scope(scope), objName(objName), f(f) {
    }

    void Execute() override;

    const char* Type()  override { return typeid(*this).name(); }

    cIoCImpl* ioc;
    const std::string scope, objName;
    const void* f;
};


// Factory 
// keep factory methond in a map<string,function pointer>
class cFactory
{
    friend struct iRegisterFactoryMethod;

public:
    template< typename T, typename... Args>
    using funcPointer = T * (*)(Args... args);

    // register a factory method
    template< typename T, typename... Args>
    void Register(const std::string& objName, T* (*f)(Args... args))
    {
        doRegister(objName, (const void*)f);
    }

    // get a factory method
    // throw cException if there is not a requested factory method
    template< typename T, typename... Args>
    funcPointer<T, Args...> getFactoryMethod(const std::string& objName) const
    {
        try
        {
            return funcPointer<T, Args...>(factoryMethods.at(objName));
        }
        catch (const std::out_of_range&) // Ups.... 
        {
            throw cException("There isn't such factory method.");
        }
    }

    int size() const { return int(factoryMethods.size()); }

protected:
    // keeps the pointer as old plain pointers.
    void doRegister(const std::string& objName, const void* f)
    {
        factoryMethods[objName] = f;
    }

public:
    std::map<std::string, const void*> factoryMethods;
};

// cIoCImpl is a container class for a factory pattern.  
// The Resolve function with first parameter "Register" registers a factory or a factory method  
// within a scope and returns a pointer to an instance of iCommand. The command must be executed  
// to perform the actual registration, for example:  
// cIoCImpl t;  
// cFactory f;  
// extern int* generateInt(double, string);  
// f.Register("", function);  
// t.Resolve<iCommand>("Register", "ScopeName", f)->Execute();  
// t.Resolve<iCommand>("Register", "ScopeName", "get int for me", generateInt)->Execute();  
// If the first parameter is not "Register", the function looks for a factory method by scope  
// and object name. The following parameters are passed to the factory method. Example:  
// auto objPointer = t.Resolve<ObjType>("ScopeName", "get int for me", 42, "example");  
// Invalid parameters (e.g., registering a non-factory or non-factory method) will throw an exception.  
// If no factory method is found for the given parameters, an exception is also thrown.  

class cIoCImpl
{
    // class 
protected:

    friend struct iRegisterFactory;
    friend struct iRegisterFactoryMethod;

private:
    cIoCImpl(const cIoCImpl&) = delete;
    cIoCImpl& operator=(const cIoCImpl&) = delete;

protected:
    cIoCImpl() = default;
    ~cIoCImpl() = default;

protected:
    template< typename T, typename... Args>
    void* getMethod(const std::string& scope, const std::string& objName)
    {
        // find scope factory
        auto factoryIt = factories.find(scope);
        if (factoryIt == factories.end())
            throw cException("There isn't such factory.");
        return reinterpret_cast<void*>(factoryIt->second->getFactoryMethod<T, Args...>(objName));
    }

    template< typename R, typename... Args>
    iCommand* doRegisterFactoryMethod(const std::string& scope, const std::string& objName, R* (*f)(Args... args))
    {
        return new iRegisterFactoryMethod(*this, scope, objName, (const void*)f);
    }

    template<typename F>
    iCommand* doRegisterFactory(const std::string& scope, const F& f)
    {
        throw cException("Wrong registration type.");
        return nullptr;
    }


    template< typename T, typename... Args>
    T* doResolve(const std::string s1, const std::string s2, Args... args)
    {
        DBG(0);
        auto method = getMethod<T, Args... >(s1, s2);
        DBG1(method);
        using f = T * (*)(Args...);
        DBG(1);
        auto my_tuple = std::make_tuple(std::forward<Args>(args)...);
        DBG2(typeid(my_tuple).name());

        return nullptr;

        return ((f)method)(args...);
        return nullptr;
        //return (*f(method))(args...);
    }

    template< typename T, typename... Args>
    T* ssResolve(const std::string s1, const std::string s2, const Args... args)
    {
        if (s1 == "Register")
        {
            if constexpr (sizeof...(Args) == 1)
            {
                const auto& f = std::get<0>(std::forward_as_tuple(args...));
                //const std::string s = typeid(f).name();
                return (T*)doRegisterFactory(s2, f);
            }

            if constexpr (sizeof...(Args) == 2)
            {
                auto objName = std::get<0>(std::forward_as_tuple(args...));
                auto fm = std::get<1>(std::forward_as_tuple(args...));
                return doRegisterFactoryMethod(s2, objName, fm);
            }

            throw(cException("Wrong registering type in resolving."));
        }

        return doResolve<T, Args...>(s1, s2, std::forward<const Args>(args)...);
    }

protected:
    std::map<std::string, std::shared_ptr<cFactory> > factories;
};

class cIoC : public cIoCImpl
{
public:
    cIoC() = default;
    ~cIoC() = default;

    template< typename T, typename S1, typename S2, typename... Args>
    T* Resolve(S1 s1, S2 s2, Args... args)
    {
        return ssResolve<T>(std::string(s1), std::string(s2), std::forward<Args>(args)...);
    }

};

template<>
inline iCommand* cIoCImpl::doRegisterFactory(const std::string& scope, const cFactory& f)
{
    return new iRegisterFactory(*this, scope, f);
}


inline iRegisterFactory::iRegisterFactory(cIoCImpl& ioc, const std::string& scope, const cFactory& f) : ioc(&ioc), scope(scope), shptrFactory(new cFactory(f)) {}

inline void iRegisterFactory::Execute()
{
    ioc->factories[scope] = shptrFactory;
}

inline void iRegisterFactoryMethod::Execute()
{
    if (ioc->factories.find(scope) == ioc->factories.end())
        throw cException("There isn't such factory.");

    auto& m = ioc->factories[scope];
    m->doRegister(objName, f);
}

#endif //#ifndef CIOC_HPP
