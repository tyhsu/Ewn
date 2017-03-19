#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include<string>
#include<sstream>
#include<windows.h>
using namespace std;

class Move
{
public:
    unsigned char Ch=0;
    unsigned char from=0;
    unsigned char to=0;
};

class EBoard
{
public:
    unsigned char BN=0;
    unsigned char RN=0;
    unsigned char Ch[16]= {};
    unsigned char*Blue=Ch;
    unsigned char*Red=Ch+6;

    unsigned char ChessBoard[32]= {};
};


int main()
{
    srand(time(0));
    int sleeptime=0;

    FILE*sf=fopen("sleeptime.txt","r");
    if(sf)
    {
        fscanf(sf,"%d",&sleeptime);
        fclose(sf);
    }
    string line;
    stringstream ss;
    char Color;
    int Dice;

    while(true)
    {
        getline(cin,line);
        ss.clear();
        ss.str("");
        ss<<line;
        ss>>line;

        if(line=="exit")return 0;
        else if(line=="ini")
        {

            ss>>Color;
            if(Color=='B')
                cout<<"23 19 15 25 24 20\n";
            else
                cout<<"6 7 11 1 2 3\n";
        }
        else if(line=="get")
        {
            if(sleeptime)Sleep(sleeptime);
            ss>>Color;
            ss>>Dice;

            Dice-=1;
            int pos;
            int num;
            EBoard board;
            for(int i=0; i<12; ++i)
            {
                ss>>pos;
                board.Ch[i]=pos;
                if(pos)board.ChessBoard[pos-1]=i+1;
            }

            if(Color=='B')
            {
                if(board.Blue[Dice]!=0)num=board.Blue[Dice];
                else
                {
                    for(int i=Dice+1; i<6; ++i) if(board.Blue[i]!=0)
                        {
                            num=board.Blue[i];
                            break;
                        }
                    for(int i=Dice-1; i>=0; --i) if(board.Blue[i]!=0)
                        {
                            num=board.Blue[i];
                            break;
                        }
                }
                cout<<num<<" ";
                int x=(num-1)%5;
                int y=(num-1)/5;
                if (x != 0)
                {
                    if (y != 0)cout<<num - 6<<endl;
                    else cout<<num - 1<<endl;
                }
                else cout<<num - 5<<endl;
            }
            else
            {
                if(board.Red[Dice]!=0)num=board.Red[Dice];
                else
                {
                    for(int i=Dice+1; i<6; ++i) if(board.Red[i]!=0)
                        {
                            num=board.Red[i];
                            break;
                        }
                    for(int i=Dice-1; i>=0; --i) if(board.Red[i]!=0)
                        {
                            num=board.Red[i];
                            break;
                        }
                }
                cout<<num<<" ";
                int x=(num-1)%5;
                int y=(num-1)/5;
                if (x != 4)
                {
                    if (y != 4)cout<<num + 6<<endl;
                    else cout<<num + 1<<endl;
                }
                else cout<<num + 5<<endl;
            }
        }
    }
}
