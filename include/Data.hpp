#pragma once

#include"Common.hpp"
#include<unordered_map>
#include<string>
#include<functional>
#include<fstream>
#include<array>


/*��1��	������Ϣ������team.txt�ж�ȡ��������Ļ�����Ϣ����ƺ��ʵ����ݽṹ�洢����ʵ�ֶԲ�����������ӡ��޸ĺ������Ϊ�����������һ������Ϊ60~100֮��ĳ����ɼ�������ʵ�ֲ�������ĳɼ���ѯ��ʵ�ֻ��ڶ����������Ĳ��ң�����ƺ��ʵ����������������ʾ��������ӱ�ţ���ѯ����ĳ����ɼ��������ҳɹ����������������Ӧ�Ļ�����Ϣ��������Ʒ���ơ�����ѧУ��������𡢲����ߺͳ����ɼ���Ϣ�������⣬���ȫ�������ӵ�ƽ�����ҳ���ASL��*/


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