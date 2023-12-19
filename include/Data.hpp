#pragma once

#include"Common.hpp"
#include<unordered_map>
#include<string>
#include<functional>
#include<fstream>
#include<array>
#include<random>


/*��1��	������Ϣ������team.txt�ж�ȡ��������Ļ�����Ϣ����ƺ��ʵ����ݽṹ�洢����ʵ�ֶԲ�����������ӡ��޸ĺ������Ϊ�����������һ������Ϊ60~100֮��ĳ����ɼ�������ʵ�ֲ�������ĳɼ���ѯ��ʵ�ֻ��ڶ����������Ĳ��ң�����ƺ��ʵ����������������ʾ��������ӱ�ţ���ѯ����ĳ����ɼ��������ҳɹ����������������Ӧ�Ļ�����Ϣ��������Ʒ���ơ�����ѧУ��������𡢲����ߺͳ����ɼ���Ϣ�������⣬���ȫ�������ӵ�ƽ�����ҳ���ASL��*/

class BSTree final
{

    using key_t = id_t;
    using value_t = int;

    struct Node final
    {
        key_t key;
        value_t value;
        Node*left,*right;
    };

public:

    explicit BSTree() noexcept :root_(nullptr) {}



    ~BSTree() {
        _clear(root_);
    }



    void insert(const id_t& key,int value) noexcept {
        root_ = _insert(root_,key,value);
    }



    int* find(const id_t& key) const noexcept {
        Node* res = _find(root_,key);
        if (res == nullptr) {
            return nullptr;
        } else {
            return &res->value;
        }
    }


private:

    Node* _insert(Node* root,const key_t& key,const value_t& value) const noexcept {
        if (root == nullptr)return new Node{ key,value,nullptr,nullptr };
        if (key < root->key) {
            root->left = _insert(root->left,key,value);
        } else {
            root->right = _insert(root->right,key,value);
        }
        return root;
    }



    Node* _find(Node* root,const key_t& key) const noexcept {
        if (root == nullptr)return nullptr;
        if (key == root->key) {
            return root;
        } else if (key < root->key) {
            return _find(root->left,key);
        } else {
            return _find(root->right,key);
        }
    }



    void _clear(Node* root) const noexcept {
        if (root) {
            _clear(root->left),_clear(root->right);
            delete root;
        }
    }


private:
    Node* root_;
};














class InformationTable final
{

    friend class Queryer;
    using key_t = id_t;
    using value_t = BasicInformation;

private:

    class OrderBookTable final
    {

    public:

        using group_t = std::vector<id_t>;



        bool addTeam(const id_t& id,int groupIndex) {
            bool legal = groupIndex <= GroupNumber && groupIndex >= 1;
            assertm("out of range",legal);
            if (!legal) {
                return false;
            }
            groups_[groupIndex - 1].push_back(id);
            teamsMap_.insert(std::make_pair(id,groupIndex));
            return true;
        }



        int queryGroupIndex(const id_t& id) const noexcept {
            if (auto it = teamsMap_.find(id);it != teamsMap_.end()) {
                return it->second;
            }

            return -1;
        }



        const group_t& getGroup(int groupIndex) const {
            bool legal = groupIndex <= GroupNumber && groupIndex >= 1;
            assertm("out of range",legal);
            if (!legal) {
                throw CException{ "groupIndex must in (1-17)" };
            }
            return groups_[groupIndex - 1];
        }



        group_t& getGroup(int groupIndex) {
            bool legal = groupIndex <= GroupNumber && groupIndex >= 1;
            assertm("out of range",legal);
            if (!legal) {
                throw CException{ "groupIndex must in (1-17)" };
            }
            return groups_[groupIndex - 1];
        }


    private:
        std::array<group_t,GroupNumber> groups_;
        std::unordered_map<key_t,int> teamsMap_;
    };


private:
    using basicInforTable_t = std::unordered_map<key_t,value_t>;
    using scoreInforTable_t = BSTree;
    using orderBookTable_t = OrderBookTable;
    using buildingInforTable_t = std::unordered_map<std::string,BuildingInformation>;
    basicInforTable_t basicInfors_;
    scoreInforTable_t scoreInfors;
    orderBookTable_t orderBook_;
    buildingInforTable_t buildingInfors_;
};




class Queryer final
{

public:

    explicit Queryer(InformationTable& table) noexcept :informations_(table) {}



    void addTeam(const id_t& id,const BasicInformation& infor) noexcept {
        informations_.basicInfors_.insert(std::make_pair(id,infor));
        return;
    }



    void addTeamFromTxtFile(const std::string& path) noexcept {
        std::fstream fs;

        try {
            fs.open(path);
        } catch (...) {
            printf("fail to open the file: %s",path);
        }

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



    void loadBuildingInforsFromTexfile(const std::string& path) noexcept {
        std::fstream fs;
        try {
            fs.open(path);
        } catch (...) {
            printf("fail to open the file: %s",path);
        }

        std::string str = "";
        char symbol{};
        while (fs >> str >> symbol) {
            informations_.buildingInfors_.insert(
                std::make_pair(str,BuildingInformation{ str }));

        }

        fs.close();
        return;
    }



    void spawnOrderBookFromTxtFile(const std::string& path) noexcept {
        std::fstream fs;
        try {
            fs.open(path);
        } catch (...) {
            printf("fail to open the file: %s",path);
        }

        int groupIndex;
        id_t id;
        std::string str;
        std::getline(fs,str);
        while (fs >> groupIndex >> id) {
            informations_.orderBook_.addTeam(id,groupIndex);
            std::getline(fs,str);
        }

        fs.close();
    }



    void distributeScore() noexcept {
        for (auto &[key,value] : informations_.basicInfors_) {
            int score = 40 + std::rand() % 61;
            informations_.scoreInfors.insert(key,score);
        }
    }



    bool exist(const id_t& id) const noexcept {
        if (auto it = informations_.basicInfors_.find(id);
            it != informations_.basicInfors_.end()) {
            return true;
        }
        return false;
    }



    const BasicInformation* queryBasicInformation(const id_t& id) const noexcept {
        if (auto it = informations_.basicInfors_.find(id);
            it != informations_.basicInfors_.end()) {
            const InformationTable::value_t& value = it->second;
            return &value;
        } else {
            return nullptr;
        }
    }



    BasicInformation* queryBasicInformation(const id_t& id) noexcept {
        if (auto it = informations_.basicInfors_.find(id);
            it != informations_.basicInfors_.end()) {
            InformationTable::value_t& value = it->second;
            return &value;
        } else {
            return nullptr;
        }
    }



    int* queryScore(const id_t& id) const noexcept {
        if (auto it = informations_.scoreInfors.find(id);it) {
            return it;
        } else {
            return nullptr;
        }
    }



    const int* queryScore(const id_t& id) noexcept {
        if (auto it = informations_.scoreInfors.find(id);it) {
            return it;
        } else {
            return nullptr;
        }
    }



    int queryGroupIndex(const id_t& id) const noexcept {
        return informations_.orderBook_.queryGroupIndex(id);
    }



    const BuildingInformation* queryBuildingInformation(const std::string& buildName) const noexcept {
        if (auto it = informations_.buildingInfors_.find(buildName);it != informations_.buildingInfors_.end()) {
            return &it->second;
        } else {
            return nullptr;
        }
    }



    BuildingInformation* queryBuildingInformation(const std::string& buildName) noexcept {
        if (auto it = informations_.buildingInfors_.find(buildName);it != informations_.buildingInfors_.end()) {
            return &it->second;
        } else {
            return nullptr;
        }
    }



    const std::vector<id_t>& getGroup(int groupIndex) const {
        const auto& res = informations_.orderBook_.getGroup(groupIndex);
        return res;
    }



    std::vector<id_t>& getGroup(int groupIndex) {
        auto& res = informations_.orderBook_.getGroup(groupIndex);
        return res;
    }



    void for_each(std::function<void (const id_t& id,BasicInformation& infor)>&& func) {
        for (auto&[key,value] : informations_.basicInfors_) {
            func(key,value);
        }
    }



    void for_each(std::function<void (const id_t& id,const BasicInformation& infor)>&& func) const {
        for (const auto&[key,value] : informations_.basicInfors_) {
            func(key,value);
        }
    }



    
    size_t getSize() const noexcept {
        return informations_.basicInfors_.size();
    }


private:
    InformationTable& informations_;
};



