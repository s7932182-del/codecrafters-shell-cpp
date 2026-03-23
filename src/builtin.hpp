#ifndef BUILTIN_HPP
#define BUILTIN_HPP

#include <iostream>
#include <unordered_map>
#include "executable.hpp"
#include "parser.hpp"
#include<algorithm>
#include "redirection.hpp"

template<typename T>
class Builtin
{
protected:
    std::string name;

public:
    Builtin(const std::string &n = "") : name(n) {
       
    }
    virtual void execute(T&) = 0;
    virtual std::string get_name() = 0;
    virtual ~Builtin() = default;

    static std::unordered_map<std::string, Builtin *> &getMap()
    {
        static std::unordered_map<std::string, Builtin *> instance;
        return instance;
    }

    static void register_command(const std::string &name, Builtin *cmd)
    {
        getMap()[name] = cmd;
    }
};


// ? TYPE Builtin
class TYPE final : public Builtin<Parser>
{
private:
    TYPE();

public:
    static TYPE &getInstance();
    void execute(Parser &ps) override;
    std::string get_name() override;
};



// ? ECHO Builtin

class ECHO final: public Builtin<Parser>
{
private:
    ECHO();

public:
    static ECHO &getInstance();
    void execute(Parser &ps) override;
    std::string get_name() override;
};



// ? EXIT Builtin

class EXIT final: public Builtin<Parser> 
{
private:
    EXIT();

public:
    static EXIT &getInstance();
    void execute(Parser& x) override;
    std::string get_name() override;
};






// ? PWD Builtin

class PWD final: public Builtin<Parser> 
{
private:
    PWD();

public:
    static PWD &getInstance();
    void execute(Parser& x) override;
    
    std::string get_name() override;
};




// ? CD Builtin

class CD final: public Builtin<Parser> 
{
private:
    CD();

public:
    static CD &getInstance();
    void execute(Parser& x) override;
    std::string get_name() override;
};





#endif
