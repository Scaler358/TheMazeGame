#include <QCoreApplication>
#include <array>
#include <iostream>
#include <random>
#include <ctime>
#include <conio.h>

//создаем условия
const short int MaxRowSize=20;
const short int MaxColumnSize=20;

const char MapPlayerSign='@';
const char MapWallSign='#';
const char MapExitSign='E';
const char MapEmptySign=' ';

using Map = std::array<std::array<char,MaxRowSize>,MaxColumnSize>;

//создаем структуру для сохранения координат
struct Position
{
    int RowPosition;
    int ClmnPosition;
};

int myrand(int a=0,int b=2)
{
    return (rand()%(b-a))+a;
}

//заполняем карту по контуру
void fillMap(Map&map)
{
    for(int row=0;row<MaxRowSize;row++)
    {
        for(int column=0;column<MaxColumnSize;column++)
        {
            if(column==0||row==0||column==MaxColumnSize-1||row==MaxRowSize-1)
            {
                map[row][column]=MapWallSign;
            }
            else
            {
                map[row][column]=MapEmptySign;
            }
        }
    }
}

//проверяем можно ли поставить стену
bool validatePuting(const Map map,Position position)
{
    short int result=0;
    for(int row=position.RowPosition-1;row<position.RowPosition+2;row++)
    {
        for(int clm=position.ClmnPosition-1;clm<position.ClmnPosition+2;clm++)
        {
            if(!(clm==0||row==0||clm==MaxColumnSize-1||row==MaxRowSize-1))
            {
                if(map[row][clm]!=MapEmptySign)
                    result++;
            }
        }
    }
    if(result>1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//создаем лабиринт
void buildMaze(Map&map,short int GameHardLevel)
{
    for(int row=0;row<MaxRowSize;row++)
    {
        for(int column=0;column<MaxColumnSize;column++)
        {
            if(myrand(0,7-GameHardLevel)==0&&validatePuting(map,Position{row,column}))
            {
                map[row][column]=MapWallSign;
            }
        }
    }
}

//показываем карту
void showMap(const Map& map)
{
    std::cout<<std::endl;
    for(int row=0;row<MaxRowSize;row++)
    {
        for(int column=0;column<MaxColumnSize;column++)
        {
            std::cout<<map[row][column];
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

//помещаем на карту игрока
Position addplayer(Map& map)
{
    int RowPosition;
    int ColumnPosition;
    do
    {
        RowPosition=myrand(1,MaxRowSize-2);
        ColumnPosition=myrand(1,MaxColumnSize-2);
    }
    while(map[RowPosition][ColumnPosition]!=MapEmptySign);
    map[RowPosition][ColumnPosition]=MapPlayerSign;
    return Position{RowPosition,ColumnPosition};
}

//помещаем на карту выход
void addExit(Map&map)
{
    int RowPosition;
    int ColumnPosition;
    do
    {
        RowPosition=myrand(1,MaxRowSize-2);
        ColumnPosition=myrand(1,MaxColumnSize-2);
    }
    while(map[RowPosition][ColumnPosition]!=MapEmptySign);
    map[RowPosition][ColumnPosition]=MapExitSign;
}

//показываем меню
void showmenu()
{
    std::cout<<"Maze game\n"<<std::endl;
    std::cout<<"Controls : use arrows to move and escape to quit\n"<<std::endl;
}

//двигаем игрока
Position moveplayer(Map&map,const Position PlayerPosition,const char where)
{
    Position newPlayerPosition;
    switch (where)
    {
    case 'r':
        newPlayerPosition=Position{PlayerPosition.RowPosition,PlayerPosition.ClmnPosition+1};
        break;
    case 'l':
        newPlayerPosition=Position{PlayerPosition.RowPosition,PlayerPosition.ClmnPosition-1};
        break;
    case 'u':
        newPlayerPosition=Position{PlayerPosition.RowPosition-1,PlayerPosition.ClmnPosition};
        break;
    case 'd':
        newPlayerPosition=Position{PlayerPosition.RowPosition+1,PlayerPosition.ClmnPosition};
        break;
    }
    if(map[newPlayerPosition.RowPosition][newPlayerPosition.ClmnPosition]==MapEmptySign)
    {
        map[PlayerPosition.RowPosition][PlayerPosition.ClmnPosition]=MapEmptySign;
        map[newPlayerPosition.RowPosition][newPlayerPosition.ClmnPosition]=MapPlayerSign;
        return newPlayerPosition;
    }
    else if(map[newPlayerPosition.RowPosition][newPlayerPosition.ClmnPosition]==MapWallSign)
    {
        return Position{-1,-1};
    }
    else if(map[newPlayerPosition.RowPosition][newPlayerPosition.ClmnPosition]==MapExitSign)
    {
        return Position{MaxRowSize,MaxColumnSize};
    }
}

//проверяем продолжается ли игра
bool analizePosition(Position position)
{
    if((position.RowPosition==-1&&position.ClmnPosition==-1)||(position.RowPosition==MaxColumnSize&&position.ClmnPosition==MaxRowSize))
    {
        if(position.RowPosition==-1&&position.ClmnPosition==-1)
            std::cout<<"Sorry, you lost\n"<<std::endl;
        else
            std::cout<<"Congratulations, you won!\n"<<std::endl;
        return false;
    }
    else
    {
        return true;
    }
}

//сама игра
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    srand(time(0));
    Map map;
    bool restartGame;
    bool gameContinue;
    int whereToMove;
    short int GameHardLevel;
    showmenu();
    std::cout<<"Start game?(1/0)\n"<<std::endl;
    std::cin>>gameContinue;
    do //глобальный цикл повторения игр
    {
    system("cls");
    std::cout<<"\nChoose level (1-5)\n"<<std::endl;
    std::cin>>GameHardLevel;
    do //цикл перехода по уровням
    {
    system("cls");
    gameContinue=true;
    fillMap(map);
    buildMaze(map,GameHardLevel);
    Position PlayerPosition=addplayer(map);
    addExit(map);
    while(gameContinue) //цикл отрисовки поля и игры
    {
        system("cls");
        std::cout<<"Level : "<<GameHardLevel<<std::endl;
        showMap(map);
        whereToMove=getch();
        switch (whereToMove)
        {
            case 77:
            PlayerPosition=moveplayer(map,PlayerPosition,'r');
            break;
            case 75:
            PlayerPosition=moveplayer(map,PlayerPosition,'l');
            break;
            case 72:
            PlayerPosition=moveplayer(map,PlayerPosition,'u');
            break;
            case 80:
            PlayerPosition=moveplayer(map,PlayerPosition,'d');
            break;
            case 27:
            GameHardLevel=6;
            gameContinue=false;
            break;
        }
        if(gameContinue)
        {
            if(!analizePosition(PlayerPosition))
            {
                gameContinue=false;
                GameHardLevel=6;
            }
            else
                gameContinue=true;
        }
    }
    if(GameHardLevel==5)
    {
        GameHardLevel++;
        std::cout<<"Wow, you completed the game!\n"<<std::endl;
    }
    else
        GameHardLevel++;
    }
    while(GameHardLevel<=5);
    std::cout<<"Restart game?(1/0)\n"<<std::endl;
    std::cin>>restartGame;
    }
    while(restartGame);

    std::cout<<"end"<<std::endl;
    return a.exec();
}
