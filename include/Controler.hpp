#pragma once

#include"Data.hpp"
#include"Common.hpp"
#include"View.hpp"
#include<fstream>
#include<iostream>
#include<random>
#include<queue>

/*
�����ֳ�ģ�⣺���Ƚ��о������飬���ɾ�������ᣬ�������Ӳ�ѯ������������𽫲���������䵽17�������ң����Ϊ1~17�����������ÿ�������ҵĽ���˳��Ϊ�����ɼ��������С��������㷨��ѡ�����򡢲�������ϣ�����򡢹鲢���򡢶�������ѡ��һ�֣���Ϊѡ����㷨��ԭ������˵����Ȼ��ģ��������򡣱����ֳ������ô��ͺ��������������д�����ʱ���߶�̬չʾ�������������ھ����Ķ��飬�����Ķ��鼰���������Ķ�����Ϣ�����д����ģ������������϶�̬չʾ�������Ӻ򳡡������С�����������״̬��
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