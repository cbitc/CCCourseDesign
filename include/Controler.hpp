#pragma once

#include"Data.hpp"
#include"Common.hpp"
#include"View.hpp"
#include<fstream>
#include<iostream>
#include<random>
#include<queue>

/*
决赛现场模拟：首先进行决赛分组，生成决赛秩序册，供参赛队查询。根据赛事类别将参赛队伍分配到17个决赛室（编号为1~17）。秩序册中每个决赛室的进场顺序为初赛成绩降序排列。（排序算法从选择排序、插入排序、希尔排序、归并排序、堆排序中选择一种，并为选择该算法的原因做出说明）然后，模拟决赛秩序。比赛现场会设置大型候赛区，场地中有大屏以时间线动态展示各决赛室中正在决赛的队伍，侯赛的队伍及比赛结束的队伍信息。请编写程序模拟候赛区大屏上动态展示各参赛队候场、比赛中、比赛结束的状态。
*/



class Controler final
{

public:
    Controler(Queryer queryer,IViewer* viewer) :queryer_(queryer),viewer_(viewer) {}
    ~Controler() {
        delete viewer_;
    }
    

    void finalGroup(const std::string& path) {
        std::fstream fs;
        fs.open(path);

    }
    

    void query_request(std::stringstream& stream) {
        stream.clear();
        viewer_->waitingForInput(stream,"input the id of team");
        id_t id;stream >> id;
        if (queryer_.exist(id)) {
            const BasicInformation& infor = queryer_.queryInformation(id);
            viewer_->showTeamInfor(id,infor);
        } else {
            viewer_->showMessage("the id be not found\n");
        }
    }

    void entryFinal_request(std::stringstream& stream) {
        stream.clear();

        std::array<std::vector<id_t>,GroupNumber> groups;
        for (int i = 1;i <= GroupNumber;i++) {
            const auto& group = queryer_.getGroup(i);
            groups[i - 1] = group;
            //todo sort
        }

        
    }



    void handleInput(std::stringstream& stream) {
        stream.clear();
        viewer_->waitingForInput(stream,"input number to selcet operation");
        int number;stream >> number;

        if (number == 1) {
            query_request(stream);
        } else if (number == 2) {

        } else if (number == 3) {

        } else if (number == 4) {

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