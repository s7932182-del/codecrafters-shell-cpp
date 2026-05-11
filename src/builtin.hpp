#ifndef BUILTIN_HPP
#define BUILTIN_HPP

#include <iostream>
#include <unordered_map>
#include "executable.hpp"
#include "parser.hpp"
#include<algorithm>
#include "redirection.hpp"

template<typename T>
class Builtin {
protected:
    std::string name;

public:
    Builtin(const std::string &n = "") : name(n) {
    }

    virtual void execute(const std::vector<std::string> &args) = 0;

    virtual std::string get_name() = 0;

    virtual ~Builtin() = default;

    static std::unordered_map<std::string, Builtin *> &getMap() {
        static std::unordered_map<std::string, Builtin *> instance;
        return instance;
    }

    static void register_command(const std::string &name, Builtin *cmd) {
        getMap()[name] = cmd;
    }
};


// ? TYPE Builtin
class TYPE final : public Builtin<Parser> {
private:
    TYPE();

public:
    static TYPE &getInstance();

    void execute(const std::vector<std::string> &args) override;

    std::string get_name() override;
};


// ? ECHO Builtin

class ECHO final : public Builtin<Parser> {
private:
    ECHO();

public:
    static ECHO &getInstance();

    void execute(const std::vector<std::string> &args) override;

    std::string get_name() override;
};


// ? EXIT Builtin

class EXIT final : public Builtin<Parser> {
private:
    EXIT();

public:
    static EXIT &getInstance();

    void execute(const std::vector<std::string> &args) override;

    std::string get_name() override;
};


// ? PWD Builtin

class PWD final : public Builtin<Parser> {
private:
    PWD();

public:
    static PWD &getInstance();

    void execute(const std::vector<std::string> &args) override;

    std::string get_name() override;
};


// ? CD Builtin

class CD final : public Builtin<Parser> {
private:
    CD();

public:
    static CD &getInstance();

    void execute(const std::vector<std::string> &args) override;

    std::string get_name() override;
};


// ? History Builtin


class HISTORY final : public Builtin<Parser> {
private:
    HISTORY();

public:
    static HISTORY &getInstance();

    void execute(const std::vector<std::string> &args) override;

    std::string get_name() override;
};


class JOB final : public Builtin<Parser> {
private:
    JOB();



public:
    static JOB &getInstance();

    void execute(const std::vector<std::string>&) override;

    std::string get_name() override;

    struct job_info {
         uint32_t rank;
        std::string name;
        pid_t pid;
        char marker = '\0';


        enum Status { RUNNING, EXITED };

        Status status = RUNNING; // Default to RUNNING

        job_info( const uint32_t rank,std::string &n, const pid_t pid, const char marker) : rank(rank), name(n), pid(pid), marker(marker) {
        }

        void print(const bool isRunning = true) const {
            if (isRunning) {
                std::cout << "[" << this->rank << "]" << this->marker << "  " << "Running " << std::setw(24) << this->name <<
                    std::endl;
            } else {
                std::cout << "[" << rank << "]" << marker << "  " << "Done" << std::setw(24)  << name.substr(0,name.size() -1) << std::endl;
            }
        }


    };

    static std::vector<job_info> background_jobs;


};


class COMPLETE final : public Builtin<Parser> {
    private:
    COMPLETE();
    public:
    static COMPLETE &getInstance();
    void execute(const std::vector<std::string> &args) override;
    std::string get_name() override;
};
#endif
