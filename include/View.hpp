#pragma once

#include"Common.hpp"


class IViewer
{
public:
    virtual void waitingForInput(std::stringstream& stream,std::string_view message) = 0;
    virtual void showMenu() = 0;
    virtual void showTeamInfor(const id_t& id,const BasicInformation& infor) = 0;
    virtual void showMessage(std::string_view message) = 0;
};

class ConsoleView:public IViewer 
{

public:
    ConsoleView(){}
    
    void waitingForInput(std::stringstream& stream,std::string_view message) override {
        std::cout << message << '\n' << ">>";
        std::string input = "";
        std::cin >> input;
        stream << input;
    }
    
    void showMenu() override {
        std::cout << "welcome to use the system\n" << "there are three functions to use\n"
            << "1.queryInformation\n" << "2.Navigation\n" << "3.enter the final:\n";
    }

    void showTeamInfor(const id_t& id,const BasicInformation& infor) override {
        std::cout << "id: " << id << " entryName: " << infor.entryName << " shoolName: "
            << infor.school << " category: " << infor.category << " competitor: "
            << infor.competitor << " advisor: " << infor.advisor << '\n';
        
    }


    void showMessage(std::string_view message) override {
        std::cout << "message: " << message;
    }


private:
    
};