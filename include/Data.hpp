#pragma once

#include"Common.hpp"
#include<unordered_map>
#include<string>
#include<functional>
#include<fstream>
#include<array>


/*（1）	赛事信息管理：从team.txt中读取参赛队伍的基本信息，设计合适的数据结构存储，能实现对参赛队伍的增加、修改和浏览。为参赛队伍分配一个分数为60~100之间的初赛成绩，并能实现参赛队伍的成绩查询（实现基于二叉排序树的查找）。设计合适的输入输出，根据提示输入参赛队编号，查询队伍的初赛成绩，若查找成功，输出该赛事类别对应的基本信息（参赛作品名称、参赛学校、赛事类别、参赛者和初赛成绩信息）。另外，输出全部参赛队的平均查找长度ASL。*/


class TeamsInformationTable final
{
    friend class Queryer;
    using key_t = id_t;
    using value_t = BasicInformation;
private:
    
    class OrderBookTable final
    {
    
    public:
        
        using group_t = std::vector<id_t>;

        void addTeam(const id_t& id,int groupIndex) {
            assertm("out of range",groupIndex <= GroupNumber && groupIndex>=1);
            groups_[groupIndex - 1].push_back(id);
            teamsMap_.insert(std::make_pair(id,groupIndex));
        }

        int queryGroupIndex(const id_t& id) const noexcept {
            if (auto it = teamsMap_.find(id);it != teamsMap_.end()) {
                return it->second;
            }
            return -1;
        }

        const group_t& getGroup(int groupIndex) const noexcept {
            assertm("out of range",groupIndex <= GroupNumber && groupIndex>=1);
            return groups_[groupIndex];
        }

    private:
        std::array<group_t,GroupNumber> groups_;
        std::unordered_map<id_t,int> teamsMap_;
    };

private:
    using basicInforTable_t = std::unordered_map<key_t,value_t>;
    using orderBookTable_t = OrderBookTable;
    basicInforTable_t basicInfors_;
    orderBookTable_t orderBook_;
};




class Queryer final
{

public:

    Queryer(TeamsInformationTable& table) :informations_(table) {}

    void addTeam(const id_t& id,const BasicInformation& infor) noexcept {
        informations_.basicInfors_.insert(std::make_pair(id,infor));
        return;
    }

    void addTeamFromTxtFile(const std::string& path) {
        std::fstream fs;
        fs.open(path);

        id_t id;
        BasicInformation value;
        std::string name;
        char sym;
        std::getline(fs,name);
        while (fs >> id >> sym) {
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

            addTeam(id,value);
        }

        fs.close();
    }

    
    void spawnOrderBookFromTxtFile(const std::string& path) {
        std::fstream fs;
        fs.open(path);
        int groupIndex;
        id_t id;
        std::string str;
        std::getline(fs,str);
        while (fs >> groupIndex>> id) {
            informations_.orderBook_.addTeam(id,groupIndex);
            std::getline(fs,str);
        }
        fs.close();
    }
    

    bool exist(const id_t& id) const noexcept {
        if (auto it = informations_.basicInfors_.find(id);
            it != informations_.basicInfors_.end()) {
            return true;
        }
        return false;
    }


    BasicInformation queryInformation(const id_t& id) const {
        if (auto it = informations_.basicInfors_.find(id);
            it != informations_.basicInfors_.end()) {
            const TeamsInformationTable::value_t& value = it->second;
            return value;
        }
        assertm("the id not found",false);
        throw CException{"the id not found"};
        return {};
    }


    void modifyInformation(const id_t& id,const BasicInformation& infor) {
        if (auto it = informations_.basicInfors_.find(id);
            it != informations_.basicInfors_.end()) {
            it->second = infor;
        } else {
            assertm("the id not found",false);
            throw CException{ "the id not found" };
        }
        return;
    }

    int queryGroupIndex(const id_t& id) const {
        return informations_.orderBook_.queryGroupIndex(id);
    }

    const std::vector<id_t>& getGroup(int groupIndex) const {
        return informations_.orderBook_.getGroup(groupIndex);
    }

    void for_each(std::function<void(const id_t& id)>&& func) noexcept {
        for (auto it = informations_.basicInfors_.begin();
            it != informations_.basicInfors_.end();++it) {
            func(it->first);
        }
    }

    size_t getSize() const noexcept {
        return informations_.basicInfors_.size();
    }
private:
    TeamsInformationTable& informations_;
};