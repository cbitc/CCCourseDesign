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
    IViewer* viewer = new ConsoleView();
    Controler controler(queryer,viewer);
    controler.addTeamFromTxtFile("../Assets/team.txt");
    controler.run();
    return 0;
}