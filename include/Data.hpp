#pragma once

#include"Common.hpp"
#include<unordered_map>
#include<string>
#include<functional>
#include<fstream>
#include<array>
#include<random>
#include<queue>


/*（1）	赛事信息管理：从team.txt中读取参赛队伍的基本信息，设计合适的数据结构存储，能实现对参赛队伍的增加、修改和浏览。为参赛队伍分配一个分数为60~100之间的初赛成绩，并能实现参赛队伍的成绩查询（实现基于二叉排序树的查找）。设计合适的输入输出，根据提示输入参赛队编号，查询队伍的初赛成绩，若查找成功，输出该赛事类别对应的基本信息（参赛作品名称、参赛学校、赛事类别、参赛者和初赛成绩信息）。另外，输出全部参赛队的平均查找长度ASL。*/

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


    int searchLen(const id_t& key) const noexcept {
        return _searchLen(root_,key);
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



    int _searchLen(Node* root,const id_t& key) const noexcept {
        if (root == nullptr)return 1;
        if (key == root->key) {
            return 1;
        } else if (key < root->key) {
            return 1 + _searchLen(root->left,key);
        } else {
            return 1 + _searchLen(root->right,key);
        }
    }


private:
    Node* root_;
};




class Graph final
{
    inline static constexpr uint16_t MAXSIZE = 16;
    using dist_t = uint16_t;
public:

    Graph() {
        map_ = std::unordered_map<std::string,uint16_t>{
            {"三号组团D楼",1},{"二号组团A楼",2},{"后勤服务楼",3},
            {"五号组团B楼",4},{"四号组团A楼",5},{"南辅门",6},
            {"七号组团C楼",7},{"西门",8},{"大学生活动中心",9},
            {"明德楼",10},{"文理大楼",11},{"图书馆",12}
        };

        vertexs_.resize(map_.size() + 1);

        vertexs_[1].name = "三号组团D楼";vertexs_[2].name = "二号组团A楼";vertexs_[3].name = "后勤服务楼";

        vertexs_[4].name = "五号组团B楼";vertexs_[5].name = "四号组团A楼";vertexs_[6].name = "南辅门";

        vertexs_[7].name = "七号组团C楼";vertexs_[8].name = "西门";vertexs_[9].name = "大学生活动中心";

        vertexs_[10].name = "明德楼";vertexs_[11].name = "文理大楼";vertexs_[12].name = "图书馆";

        for (auto& arr : edgs_) {
            arr.fill(0);
        }

        edgs_[1][2] = 120;edgs_[1][3] = 50;edgs_[1][4] = 100;

        edgs_[2][1] = 120;edgs_[2][4] = 50;edgs_[2][5] = 70;

        edgs_[3][1] = 50;edgs_[3][4] = 100;edgs_[3][6] = 40;

        edgs_[4][1] = 100;edgs_[4][2] = 50;edgs_[4][3] = 100;edgs_[4][5] = 110;edgs_[4][6] = 120;edgs_[4][7] = 90;edgs_[4][9] = 220;

        edgs_[5][2] = 70;edgs_[5][4] = 110;edgs_[5][7] = 30;edgs_[5][8] = 100;

        edgs_[6][3] = 40;edgs_[6][4] = 120;edgs_[6][7] = 150;edgs_[6][9] = 160;

        edgs_[7][4] = 90;edgs_[7][5] = 30;edgs_[7][6] = 150;edgs_[7][8] = 100;edgs_[7][9] = 180;edgs_[7][10] = 130;

        edgs_[8][5] = 100;edgs_[8][7] = 100;edgs_[8][10] = 100;edgs_[8][11] = 200;

        edgs_[9][4] = 220;edgs_[9][6] = 160;edgs_[9][7] = 180;edgs_[9][10] = 160;

        edgs_[10][7] = 130;edgs_[10][8] = 100;edgs_[10][9] = 160;edgs_[10][11] = 120;edgs_[10][12] = 200;

        edgs_[11][8] = 200;edgs_[11][10] = 120;edgs_[11][12] = 130;

        edgs_[12][10] = 200;edgs_[12][11] = 130;
    }



    std::pair<uint16_t,std::vector<std::string>> getPathTwoPoint(const std::string& begin,const std::string& end) const {
        auto it1 = map_.find(begin),it2 = map_.find(end);
        if (it1 == map_.end() || it2 == map_.end()) {
            throw CException{ "not found building" };
        }
        uint16_t p1 = it1->second,p2 = it2->second;
        auto [dist,paths] = dijkstra(p2,p1);

        std::vector<std::string> res{};
        for (uint16_t node : paths) {
            res.push_back(vertexs_[node].name);
        }

        return std::make_pair(dist,res);
    }



    BuildingInformation* find(const std::string& buildName) noexcept {
        if (auto it = map_.find(buildName);it != map_.end()) {
            return &(vertexs_[it->second]);
        } else {
            return nullptr;
        }
    }



    const BuildingInformation* find(const std::string& buildName) const noexcept {
        if (auto it = map_.find(buildName);it != map_.end()) {
            return &(vertexs_[it->second]);
        } else {
            return nullptr;
        }
    }



    const std::vector<BuildingInformation>& getBuildings() const noexcept {
        return vertexs_;
    }


private:

    std::pair<uint32_t,std::vector<uint16_t>> dijkstra(uint16_t beg,uint16_t end) const noexcept {
        using P = std::pair<uint16_t,dist_t>;

        const int nodeSize = map_.size();
        auto func = [](P p1,P p2)->bool {return p1.second > p2.second;};

        std::priority_queue<P,std::vector<P>,decltype(func)> que(func);

        std::vector<dist_t> dists(nodeSize + 1,std::numeric_limits<dist_t>::max());
        std::vector<uint16_t> paths(nodeSize + 1,0);

        que.push(P{ beg,0 });
        dists[beg] = 0;

        while (!que.empty()) {
            const auto [node,dist] = que.top();que.pop();
            const auto& neibs = edgs_[node];

            for (int i = 1;i <= nodeSize;i++) {
                if (neibs[i] == 0)continue;
                dist_t& preDist = dists[i];
                const dist_t curDist = dists[node] + neibs[i];
                if (curDist < preDist) {
                    preDist = curDist;
                    paths[i] = node;
                    que.push(P{ i,curDist });
                }
            }
        }

        std::vector<uint16_t> result{};
        for (uint16_t node = end;node != beg;node = paths[node]) {
            result.push_back(node);
        }
        result.push_back(beg);

        return std::make_pair(dists[end],result);
    }




private:
    std::array<std::array<dist_t,MAXSIZE>,MAXSIZE> edgs_;
    std::unordered_map<std::string,uint16_t> map_;
    std::vector<BuildingInformation> vertexs_;
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
    using buildingInforTable_t = Graph;
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



    void spawnOrderBookFromTxtFile(const std::string& path) noexcept {
        std::fstream fs;
        try {
            fs.open(path);
        } catch (...) {
            printf("fail to open the file: %s",path.c_str());
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

        distributeScore();
    }



    void distributeScore() noexcept {
        for (auto &[key,value] : informations_.basicInfors_) {
            int score = 60 + std::rand() % 41;
            informations_.scoreInfors.insert(key,score);
        }

    }



    int ASL() const noexcept {
        static int len = 0;
        if (len == 0) {
            int sum = 0;
            for (auto &[key,value] : informations_.basicInfors_) {
                sum += informations_.scoreInfors.searchLen(key);
            }

            len = sum / informations_.basicInfors_.size();
        }

        return len;
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
        return informations_.buildingInfors_.find(buildName);
    }



    BuildingInformation* queryBuildingInformation(const std::string& buildName) noexcept {
        return informations_.buildingInfors_.find(buildName);
    }



    const std::pair<uint16_t,std::vector<std::string>> queryPathTwoPoint(const std::string& begin,const std::string& end) const {
        const auto res = informations_.buildingInfors_.getPathTwoPoint(begin,end);
        return res;
    }




    const std::vector<id_t>& getGroup(int groupIndex) const {
        const auto& res = informations_.orderBook_.getGroup(groupIndex);
        return res;
    }



    std::vector<id_t>& getGroup(int groupIndex) {
        auto& res = informations_.orderBook_.getGroup(groupIndex);
        return res;
    }



    void for_each(std::function<void(const id_t& id,BasicInformation& infor)>&& func) {
        for (auto&[key,value] : informations_.basicInfors_) {
            func(key,value);
        }
    }



    void for_each(std::function<void(const id_t& id,const BasicInformation& infor)>&& func) const {
        for (const auto&[key,value] : informations_.basicInfors_) {
            func(key,value);
        }
    }



    void for_each_building(std::function<void(const BuildingInformation& infor)>&& func) const noexcept {
        for (const auto& infor : informations_.buildingInfors_.getBuildings()) {
            func(infor);
        }
    }



    size_t getSize() const noexcept {
        return informations_.basicInfors_.size();
    }


private:
    InformationTable& informations_;
};



