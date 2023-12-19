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
�����ֳ�ģ�⣺���Ƚ��о������飬���ɾ�������ᣬ�������Ӳ�ѯ������������𽫲���������䵽17�������ң����Ϊ1~17�����������ÿ�������ҵĽ���˳��Ϊ�����ɼ��������С��������㷨��ѡ�����򡢲�������ϣ�����򡢹鲢���򡢶�������ѡ��һ�֣���Ϊѡ����㷨��ԭ������˵����Ȼ��ģ��������򡣱����ֳ������ô��ͺ��������������д�����ʱ���߶�̬չʾ�������������ھ����Ķ��飬�����Ķ��鼰���������Ķ�����Ϣ�����д����ģ������������϶�̬չʾ�������Ӻ򳡡������С�����������״̬��
*/



class Presenter
{

    inline static const char* OutputFormat = "(id: %d,entryName: %s,shoolName: %s,category: %s,competitor: %s,advisor: %s,score: %d)\n";

public:

    explicit Presenter(Queryer queryer) noexcept {}



    void queryRequest(const Queryer queryer) noexcept {
        stream_.clear();
        cout << ">>";
        std::string input;
        cin >> input;
        if (input == "-b") {
            return;
        }

        id_t id;
        stream_ << input;stream_ >> id;


        const BasicInformation* infor = queryer.queryBasicInformation(id);
        const int* score = queryer.queryScore(id);
        _displayInfor(id,score,infor);
        return;
    }



    void modifyRequest(Queryer queryer) noexcept {
        stream_.clear();
        cout << ">>";
        std::string input;
        cin >> input;
        if (input == "-b") {
            return;
        }

        id_t id;
        stream_ << input;stream_ >> id;

        BasicInformation* infor = queryer.queryBasicInformation(id);
        if (infor) {

        } else {
            printf("\n the id has not exist\n");
        }
    }



    void browseRequest(const Queryer queryer) noexcept {
        queryer.for_each([](const id_t& id,const BasicInformation& infor) {

            });
    }



    void inforManagermentFeatures(Queryer queryer) noexcept {

        FreeConsole();
        AllocConsole();
        system("mode con cols=100 lines=30");
        system("color 0e");
        printf("input:\n1: to query certain team information\n2: to modify certain team information\n3: browse all teams information\n-q: exit \n");


        bool isQuit = false;
        while (!isQuit) {
            stream_.clear();
            cout << ">>";
            std::string input;
            cin >> input;
            if (input == "-q") {
                isQuit = true;
            } else {
                int number;stream_ >> number;
                if (number == 1) {
                    queryRequest(queryer);
                } else if (number == 2) {
                    modifyRequest(queryer);
                } else if (number == 3) {
                    browseRequest(queryer);
                } else {
                    printf("");
                    continue;
                }
            }
        }

        FreeConsole();

        return;
    }



    void entryFinalFeatures(const Queryer queryer) noexcept {

        FreeConsole();
        AllocConsole();
        system("mode con cols=100 lines=80");
        system("color 0c");


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
                const BasicInformation* infor = queryer.
                    queryBasicInformation(group.teams[currentTeam].id);
                const int* score = queryer.queryScore(group.teams[currentTeam].id);
                _displayInfor(group.teams[currentTeam].id,score,infor);

                printf("\nHas over Teams\n");
                for (int i = 0;i < currentTeam;i++) {
                    const BasicInformation* infor = queryer.
                        queryBasicInformation(group.teams[currentTeam].id);
                    const int* score = queryer.queryScore(group.teams[currentTeam].id);
                    _displayInfor(group.teams[i].id,score,infor);
                }

                printf("\nWaitting Teams\n");
                for (int i = currentTeam + 1;i < group.teams.size();i++) {
                    const BasicInformation* infor = queryer.
                        queryBasicInformation(group.teams[currentTeam].id);
                    const int* score = queryer.queryScore(group.teams[currentTeam].id);
                    _displayInfor(group.teams[i].id,score,infor);
                }
                system("pause");
            }

            Sleep(1000);
        }
    }





    void menuFeatures(Queryer queryer,bool& isQuit) noexcept {
        stream_.clear();
        std::string input;
        cout << ">>";
        cin >> input;
        cout << '\n';
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
        AttachConsole(ATTACH_PARENT_PROCESS);
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
        printf(OutputFormat,id,infor->entryName.c_str(),infor->school.c_str(),infor->category.c_str(),infor->competitor.c_str(),infor->advisor.c_str(),*score);
    };


private:
    inline static std::stringstream stream_;

};