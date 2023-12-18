#pragma once

#include"Data.hpp"
#include"Common.hpp"
#include<fstream>
#include<iostream>
#include<random>
#include<queue>
#include<Windows.h>

using std::cout;
using std::cin;

/*
决赛现场模拟：首先进行决赛分组，生成决赛秩序册，供参赛队查询。根据赛事类别将参赛队伍分配到17个决赛室（编号为1~17）。秩序册中每个决赛室的进场顺序为初赛成绩降序排列。（排序算法从选择排序、插入排序、希尔排序、归并排序、堆排序中选择一种，并为选择该算法的原因做出说明）然后，模拟决赛秩序。比赛现场会设置大型候赛区，场地中有大屏以时间线动态展示各决赛室中正在决赛的队伍，侯赛的队伍及比赛结束的队伍信息。请编写程序模拟候赛区大屏上动态展示各参赛队候场、比赛中、比赛结束的状态。
*/



class Presenter
{
    inline static const char* OutputFormat2 = "(id: %d,entryName: %s,shoolName: %s,category: %s,competitor: %s,advisor : %s)\n";

    inline static const char* OutputFormat1 = "(id: %d,entryName: %s,shoolName: %s,category: %s,competitor: %s,advisor: %s,score: %d)\n";

public:

    Presenter(Queryer queryer) :queryer_(queryer) {}



    void queryRequest(bool& isQuit) {
        stream_.clear();
        cout << ">>";
        system("color 0e");
        std::string input;
        cin >> input;
        if (input == "-q") {
            isQuit = true;
            return;
        }

        id_t id;
        stream_ << input;stream_ >> id;

        if (queryer_.exist(id)) {
            const BasicInformation& infor = queryer_.queryInformation(id);
            int score = queryer_.queryScore(id);
            printf(OutputFormat1,id,infor.entryName.c_str(),infor.school.c_str(),infor.category.c_str(),infor.competitor.c_str(),infor.advisor.c_str(),score);
        } else {
            printf("\n the id has not exist\n");
        }
    }



    void queryFeatures() {
        FreeConsole();
        AllocConsole();
        system("mode con cols=100 lines=30");
        system("color 0e");
        printf("input the id you want to query or input -q to quit\n");
        bool isQuit = false;
        while (!isQuit) {
            queryRequest(isQuit);
        }
        FreeConsole();
        return;
    }

    void entryFinalFeatures() {
        FreeConsole();
        AllocConsole();
        system("mode con cols=100 lines=80");
        system("color 0c");

        std::array<GroupInfor,GroupNumber> groups;

        for (int i = 1;i <= GroupNumber;i++) {
            const auto& group = queryer_.getGroup(i);
            auto& currentGroup = groups[i - 1];
            for (const auto& id : group) {
                int time = 10 + std::rand() % 6;
                currentGroup.teams.push_back({ id,time });
            }
            _sort(currentGroup.teams);
            currentGroup.currentTeam = 0;
        }


        int minutes = 0;
        int hours = 0;
        for (;hours < 8;minutes++) {
            if (minutes == 60) {
                minutes = 0;
                hours++;
            }
            printf("time: %d:%d \n",hours + 8,minutes);


            for (int i = 1;i <= GroupNumber;i++) {
                auto& group = groups[i - 1];
                int &currentTeam = group.currentTeam;
                if (currentTeam < group.teams.size()) {
                    if (--group.teams[currentTeam].time == 0) {
                        currentTeam++;
                    }
                }

                printf("CurrentRoom:%d\n",i);
                printf("CurrentTeam:\n");
                _displayInfor(group.teams[currentTeam].id);

                printf("\nHas over Teams\n");
                for (int i = 0;i < currentTeam;i++) {
                    _displayInfor(group.teams[i].id);
                }

                printf("\nWaitting Teams\n");
                for (int i = currentTeam + 1;i < group.teams.size();i++) {
                    _displayInfor(group.teams[i].id);
                }
                system("pause");
            }
            
            Sleep(1000);
        }
    }





    void handleInput(bool& isQuit,DWORD processID) {
        std::string input;
        cout << ">>";
        cin >> input;
        cout << '\n';
        if (input == "-q") {
            isQuit = true;
            return;
        }

        if (input == "1") {
            queryFeatures();
        }
        else if (input == "2") {
            entryFinalFeatures();
        }
        else if (input == "3") {
            
        } else {
            
        }
        AttachConsole(ATTACH_PARENT_PROCESS);
        return;
    }


    
    void run() {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD id = GetProcessId(handle);
        _displayMenu();
        bool isQuit = false;
        while (!isQuit) {
            handleInput(isQuit,id);
        }
    }

private:

    void _displayMenu() const {
        printf("welcome to use the system\n");
        printf("There are several features for you to use\n");
        printf("input 1 to use the query features\n");
        printf("input 2 to entry the final competition\n");
        printf("input 3 to use the navigation features\n");
    }

    
    void _sort(std::vector<TeamInfor>& teams) const {
        for (int i = 1;i < teams.size();i++) {
            const int currentID = teams[i].id;
            const int score1 = queryer_.queryScore(currentID);
            int j = i - 1;
            for (;j >= 0;j--) {
                const int score2 = queryer_.queryScore(teams[j].id);
                if (score1 <= score2) {
                    break;
                } else {
                    teams[j + 1].id = teams[j].id;
                }
            }
            teams[j + 1].id = currentID;
        }
    }


    
    void _displayInfor(const id_t& id) const {
        const BasicInformation infor = queryer_.queryInformation(id);
        printf(OutputFormat2,id,infor.entryName.c_str(),infor.school.c_str(),infor.category.c_str(),infor.competitor.c_str(),infor.advisor.c_str());
    };


private:
    std::stringstream stream_;
    Queryer queryer_;

};