#include<iostream>
#include<ctime>
#include"../include/Data.hpp"
#include"../include/Presenter.hpp"


using std::cin;
using std::cout;

int main() {
    std::srand(std::time(nullptr));
    TeamsInformationTable table;
    Queryer queryer(table);
    queryer.addTeamFromTxtFile("../Assets/team.txt");
    queryer.spawnOrderBookFromTxtFile("../Assets/决赛分组-江科大.txt");
    queryer.distributeScore();

    Presenter presenter(queryer);
    presenter.run();
    return 0;
}