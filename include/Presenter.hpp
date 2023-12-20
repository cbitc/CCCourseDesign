#pragma once


#include<thread>
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

    inline static const char* OutputFormat = "(id: %d,entryName: %s,shoolName: %s,category: %s,competitor: %s,advisor: %s,score: %d)\n";

public:

    explicit Presenter() noexcept {}



    void queryRequest(const Queryer queryer) noexcept {

        printf("\nThe query function is being used\n");
        std::string input;
        while (true) {
            stream_.clear();
            printf("input the id of team you want to query or -b to exit\n>>");
            cin >> input;
            if (input == "-b") {
                break;
            }
            id_t id;
            stream_ << input;stream_ >> id;
            const BasicInformation* infor = queryer.queryBasicInformation(id);
            const int* score = queryer.queryScore(id);
            _displayInfor(id,score,infor);
        }
        printf("\nExit query function\n");
        return;
    }



    void modifyRequest(Queryer queryer) noexcept {
        printf("\nThe modify function is being used\n");
        std::string input;
        while (true) {
            stream_.clear();
            printf("input the id of team you want to modify or -b to exit\n>>");
            cin >> input;
            if (input == "-b") {
                break;
            }
            id_t id;
            stream_ << input;stream_ >> id;

            BasicInformation* infor = queryer.queryBasicInformation(id);
            if (infor) {
                const int score = *queryer.queryScore(id);
                _displayInfor(id,score,*infor);
                modifyInformation(infor);
            } else {
                printf("\n the id has not exist\n");
            }
        }
        printf("\nExit modify function\n");
    }



    void modifyInformation(BasicInformation* oldInfor) noexcept {
        printf("input the name of information you want to modify and -c to complete\n");
        std::string input = "";
        while (true) {
            printf(">>");
            cin >> input;
            if (input == "-c") {
                break;
            }
            if (input == "entryName") {
                printf("\nold: %s\n>>",oldInfor->entryName.c_str());
                cin >> input;
                oldInfor->entryName = input;
            } else if (input == "schoolName") {
                printf("\nold: %s\n>>",oldInfor->school.c_str());
                cin >> input;
                oldInfor->school = input;
            } else if (input == "categoryName") {
                printf("\nold: %s\n>>",oldInfor->category.c_str());
                cin >> input;
                oldInfor->category = input;
            } else if (input == "competitor") {
                printf("\nold: %s\n>>",oldInfor->competitor.c_str());
                cin >> input;
                oldInfor->competitor = input;
            } else if (input == "advisor") {
                printf("\nold: %s\n>>",oldInfor->advisor.c_str());
                cin >> input;
                oldInfor->advisor = input;
            } else {
                printf("illegal input\n");
            }
        }
        printf("\nModification complete\n");
        return;
    }



    void browseRequest(const Queryer queryer) noexcept {

        printf("all of teams information\n");
        printf("----------------------------------------------");

        queryer.for_each([queryer](const id_t& id,const BasicInformation& infor) {
            printf("\n");
            const int score = *queryer.queryScore(id);
            Presenter::_displayInfor(id,score,infor);
            });

        printf("----------------------------------------------\n");

    }



    void inforManagermentFeatures(Queryer queryer) noexcept {

        printf("input:\n1: to query certain team information\n2: to modify certain team information\n3: browse all teams information\n-q: exit \n");


        bool isQuit = false;
        while (!isQuit) {
            stream_.clear();
            cout << ">>";
            std::string input;
            cin >> input;
            if (input == "-q") {
                isQuit = true;
            } else if (input == "1") {
                queryRequest(queryer);
            } else if (input == "2") {
                modifyRequest(queryer);
            } else if (input == "3") {
                browseRequest(queryer);
            } else {
                printf("");
                continue;
            }
        }

        printf("\nexist informationManagerment\n");
        return;
    }



    void entryFinalFeatures(const Queryer queryer) noexcept {
        std::array<GroupInfor,GroupNumber> groups;

        for (int i = 1;i <= GroupNumber;i++) {
            const auto& group = queryer.getGroup(i);
            auto& currentGroup = groups[i - 1];
            for (const auto& id : group) {
                int time = 10 + std::rand() % 6;
                currentGroup.teams.push_back({ id,time });
            }
            _sort(queryer,currentGroup.teams);
            currentGroup.currentTeam = 0;
        }


        int minutes = 0;
        int hours = 0;
        int delta = 0;
        for (;hours < 8;) {


            printf("time: %02d:%02d \n",hours + 8,minutes);


            for (int i = 1;i <= GroupNumber;i++) {
                int d = delta;
                printf("\nCurrentRoom:%d\n",i);

                auto& group = groups[i - 1];
                int &currentTeam = group.currentTeam;

                while (currentTeam < group.teams.size() &&
                    d >= group.teams[currentTeam].time) {

                    d -= group.teams[currentTeam].time;
                    group.teams[currentTeam].time = 0;
                    currentTeam++;
                }


                if (currentTeam < group.teams.size()) {
                    printf("\nCurrentTeam:\n");
                    const BasicInformation* infor = queryer.
                        queryBasicInformation(group.teams[currentTeam].id);
                    const int* score = queryer.queryScore(group.teams[currentTeam].id);
                    _displayInfor(group.teams[currentTeam].id,score,infor);
                } else {
                    printf("all of teams complete\n");
                }



                printf("\nHas over Teams\n");
                for (int i = 0;i < currentTeam;i++) {
                    const BasicInformation* infor = queryer.
                        queryBasicInformation(group.teams[i].id);
                    const int* score = queryer.queryScore(group.teams[i].id);
                    _displayInfor(group.teams[i].id,score,infor);
                }

                printf("\nWaitting Teams\n");
                for (int i = currentTeam + 1;i < group.teams.size();i++) {
                    const BasicInformation* infor = queryer.
                        queryBasicInformation(group.teams[i].id);
                    const int* score = queryer.queryScore(group.teams[i].id);
                    _displayInfor(group.teams[i].id,score,infor);
                }

                printf("\n\n\n\n");
            }
            
            printf("\ninput the delta\n");
            cin >> delta;
            minutes += delta;
            hours += minutes/60;
            minutes %= 60;
            system("cls");
        }

        printf("time: 16:00\n");
        printf("the final competition has over!!!\n");

        printf("\nexist final competition\n");
    }





    void menuFeatures(Queryer queryer,bool& isQuit) noexcept {
        stream_.clear();

        std::string input;
        cout << ">>";
        cin >> input;

        if (input == "-q") {
            isQuit = true;
            return;
        }

        if (input == "1") {
            inforManagermentFeatures(queryer);
        } else if (input == "2") {
            entryFinalFeatures(queryer);
        } else if (input == "3") {

        } else {

        }
        return;
    }



    void run(Queryer queryer) {
        _displayMenu();
        
        bool isQuit = false;
        while (!isQuit) {
            menuFeatures(queryer,isQuit);
        }
    }

private:

    static void _displayMenu() {
        printf("welcome to use the system\n");
        printf("There are several features for you to use\n");
        printf("input 1 to use the query features\n");
        printf("input 2 to entry the final competition\n");
        printf("input 3 to use the navigation features\n");
        printf("input -q to exit program\n");
    }


    static void _sort(const Queryer queryer,std::vector<TeamInfor>& teams) noexcept {
        for (int i = 1;i < teams.size();i++) {
            const int currentID = teams[i].id;
            const int* score1 = queryer.queryScore(currentID);
            int j = i - 1;
            for (;j >= 0;j--) {
                const int* score2 = queryer.queryScore(teams[j].id);
                if (*score1 <= *score2) {
                    break;
                } else {
                    teams[j + 1].id = teams[j].id;
                }
            }
            teams[j + 1].id = currentID;
        }
    }



    static void _displayInfor(const id_t& id,const int* score,const BasicInformation* infor) noexcept {
        if (infor == nullptr) {
            printf("no found id\n");
            return;
        }
        _displayInfor(id,*score,*infor);
    };



    static void _displayInfor(const id_t& id,const int score,const BasicInformation& infor) noexcept {
        printf(OutputFormat,id,infor.entryName.c_str(),infor.school.c_str(),infor.category.c_str(),infor.competitor.c_str(),infor.advisor.c_str(),score);
    };


private:
    std::stringstream stream_;
};