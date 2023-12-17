#include<iostream>
#include<ctime>
#include"../include/Data.hpp"
#include"../include/Controler.hpp"
#include"../include/View.hpp"

using std::cin;
using std::cout;

int main() {
    std::srand(std::time(nullptr));
    TeamsInformationTable table;
    Queryer queryer(table);
    queryer.addTeamFromTxtFile("../Assets/team.txt");
    queryer.spawnOrderBookFromTxtFile("../Assets/决赛分组-江科大.txt");

    Controler controler(queryer,new ConsoleView());
    controler.run();
    return 0;
}