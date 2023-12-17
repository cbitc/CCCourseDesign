#pragma once

#include"Common.hpp"
#include<unordered_map>
#include<string>
#include<functional>


/*��1��	������Ϣ������team.txt�ж�ȡ��������Ļ�����Ϣ����ƺ��ʵ����ݽṹ�洢����ʵ�ֶԲ�����������ӡ��޸ĺ������Ϊ�����������һ������Ϊ60~100֮��ĳ����ɼ�������ʵ�ֲ�������ĳɼ���ѯ��ʵ�ֻ��ڶ����������Ĳ��ң�����ƺ��ʵ����������������ʾ��������ӱ�ţ���ѯ����ĳ����ɼ��������ҳɹ����������������Ӧ�Ļ�����Ϣ��������Ʒ���ơ�����ѧУ��������𡢲����ߺͳ����ɼ���Ϣ�������⣬���ȫ�������ӵ�ƽ�����ҳ���ASL��*/


class TeamsInformationTable final
{
    friend class Queryer;


    using key_t = id_t;
    using value_t = BasicInformation;
private:
    class BSTree final
    {
        struct Node
        {
            key_t key;
            int score;
            Node* left,*right;
        };

    public:
        BSTree() :root_(nullptr) {}



        ~BSTree() {
            _destroy(root_);
        }



        void insert(const key_t& key,int score) {
            root_ = _insert(root_,key,score);
        }



        void find(const key_t& key) {

        }

        void print() const {
            _print(root_);
        }
    private:
        Node* _insert(Node* root,const key_t& key,uint8_t score) {
            if (root == nullptr)return new Node{ key,score,nullptr,nullptr };
            if (score > root->score) {
                root->right = _insert(root->right,key,score);
            } else {
                root->left = _insert(root->left,key,score);
            }
            return root;
        }

        void _print(Node* root) const {
            if (root == nullptr)return;
            _print(root->left);
            printf("(%lld,%d) ",root->key,root->score);
            _print(root->right);
        }

        void _destroy(Node* root) {
            if (root == nullptr)return;
            _destroy(root->left),_destroy(root->right);
            delete root;
        }
        Node* root_;
    };

private:
    using basicInforTable_t = std::unordered_map<key_t,value_t>;
    using scoreInforTable_t = BSTree;
    basicInforTable_t basicInfors_;
    scoreInforTable_t scoreInfors_;
};




class Queryer final
{

public:

    Queryer(TeamsInformationTable& table) :informations_(table) {}

    void addTeam(const id_t& id,const BasicInformation& infor) noexcept {
        informations_.basicInfors_.insert(std::make_pair(id,infor));
        return;
    }

    BasicInformation queryInformation(const id_t& id) const {
        if (auto it = informations_.basicInfors_.find(id);
            it != informations_.basicInfors_.end()) {
            const TeamsInformationTable::value_t& value = it->second;
            return value;
        }
        assertm("the id not found",false);
        return {};
    }


    void modifyInformation(const id_t& id,const BasicInformation& infor) {
        if (auto it = informations_.basicInfors_.find(id);
            it != informations_.basicInfors_.end()) {
            it->second = infor;
        } else {
            assertm("the id not found",false);
        }
        return;
    }


    void distributeScore(const id_t& id,int score) {
        informations_.scoreInfors_.insert(id,score);
    }

    int queryerScore(const id_t& id) const {

    }

    void for_each(std::function<void(const id_t& id)>&& func) {
        for (auto it = informations_.basicInfors_.begin();
            it != informations_.basicInfors_.end();++it) {
            func(it->first);
        }
    }

    size_t getSize() const {
        return informations_.basicInfors_.size();
    }
private:
    TeamsInformationTable& informations_;
};