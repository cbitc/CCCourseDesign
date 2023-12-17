#pragma once

#include"Data.hpp"
#include"Common.hpp"
#include"View.hpp"
#include<fstream>
#include<iostream>
#include<random>





class Controler final
{
    


public:
    Controler(Queryer queryer,IViewer* viewer) :queryer_(queryer),viewer_(viewer){}

    void addTeamFromTxtFile(const std::string& path) {
        std::fstream fs;
        fs.open(path);

        id_t id;
        BasicInformation value;
        std::string name;
        char sym;
        std::getline(fs,name);
        while ((fs >> id,fs >> sym)) {
            name = "";
            for (std::string str = "";fs >> str && str != "#";) {
                name += str;
            }
            value.entryName = name;
            fs >> name >> sym;
            value.school = name;

            fs >> name >> sym;
            value.category = name;

            fs >> name >> sym;
            value.competitor = name;

            fs >> name;
            value.advisor = name;

            queryer_.addTeam(id,value);
        }

        fs.close();
    }


    void distributeScores() {

        queryer_.for_each([this](const id_t& id) {
            int score = 60 + std::rand() % 41;
            this->queryer_.distributeScore(id,score);
            });
    }

    void queryRequest(std::stringstream& stream) {
        stream.clear();
        viewer_->waitingForInput(stream,"input the id of team");
        id_t id;stream >> id;
        const BasicInformation& infor = queryer_.queryInformation(id);
        viewer_->showTeamInfor(id,infor);
    }


    void handleInput(std::stringstream& stream) {
        stream.clear();
        viewer_->waitingForInput(stream,"input number to selcet operation");
        int number;stream >> number;

        if (number == 1) {
            queryRequest(stream);
        }
        else if (number == 2) {
            
        }
        else if (number == 3) {
            
        }
        else if (number == 4) {
            
        } else {
            viewer_->showMessage("error: illegal number;iuput again\n");
            handleInput(stream);
        }
        return;
    }


    void run() {
        viewer_->showMenu();
        bool isQuit = false;
        std::stringstream stream;
        while (!isQuit) {
            handleInput(stream);
        }
    }

private:
    Queryer queryer_;
    IViewer* viewer_;
};