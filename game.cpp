
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>


using namespace std;

/** WARNING, THIS IS A GAME ENGINE. DO NOT TOUCH, IF YOU DON'T KNOW HOW IT WORKS*/
/**#############################################################################*/
enum {
    UP, RIGHT, DOWN, LEFT
};

class CPlayer {
public:
    int y;
    int x;
};

class CEnemy : public CPlayer {
public:
    CEnemy(int a, int b) {
        x = a;
        y = b;
        int i = 5;
        while (i) {
            moves.push_back({0, 0});
            --i;
        }
    }

    vector<pair<int, int>> moves;

};

class CMap {

public:
    CMap(const string &x) {
        int i = 0;
        while (true) {
            int size = x.size();
            string tmp;
            for (; x[i] != '\n'; ++i) {
                tmp.push_back(x[i]);
            }
            map.push_back(tmp);
            i++;
            if (i == size)
                break;
        }

        int Y = map.size();
        int X = map[0].size();
        xSize = X;
        ySize = Y;

        //Here you can add more enemies. setEnemy(x,y) - will place enemy on {x,y} coordinates at the map
        //enemy.push_back(CEnemy(x, y)) - will create an enemy and add it to the enemies list. Do not write it, if you dont want enemy to move

        setEnemy(X - 2, Y - 2);
        enemy.push_back(CEnemy(X - 2, Y - 2));
        setEnemy(X - 2, 1);
        enemy.push_back(CEnemy(X - 2, 1));
        setEnemy(1, Y - 2);
        enemy.push_back(CEnemy(1, Y - 2));

        setPlayer();
    }


    CMap(int x, int y) {

        xSize = x;
        ySize = y;
        string border;
        string playZone;

        for (int i = 0; i < x; ++i) {
            if (i == 0 || i == x - 1)
                playZone.push_back('#');
            else
                playZone.push_back(' ');
        }
        for (int i = 0; i < x; ++i) {
            border.push_back('#');
        }


        for (int i = 0; i < y; ++i) {
            if (i == 0 || i == y - 1) {
                map.push_back(border);
            } else {
                map.push_back(playZone);
            }
        }
        setPlayer();
        setEnemy(x - 2, y - 2);
        enemy.push_back(CEnemy(x - 2, y - 2));
        setEnemy(x - 2, 1);
        enemy.push_back(CEnemy(x - 2, 1));
        setEnemy(1, y - 2);
        enemy.push_back(CEnemy(1, y - 2));

    }

    void spawnEnemy(int x, int y, int pos) {
        int size = enemy.size()-1;

        int xOld=enemy[pos].x;
        int yOld=enemy[pos].y;
        if (pos> size || pos < 0)
            return;

        if (setEnemy(x, y, pos))
            delPlayer(xOld, yOld);
    }

    bool checkEnemyMoves(int x, int y, int pos) {
        if (enemy[pos].moves[0] == enemy[pos].moves[2]
            && enemy[pos].moves[1] == enemy[pos].moves[3]
            && enemy[pos].moves[2] == enemy[pos].moves[4]
            && enemy[pos].moves[3].first == x
            && enemy[pos].moves[3].second == y) {
            return true;
        }
        return false;
    }

    bool setPlayer(int x = 1, int y = 1) {
        if (map[y][x] == '#')
            return false;

        if (map[y][x] == 'x') {
            map[y][x] = '$';
            cout << " GAME OVER" << endl;
            exit(0);
        }

        map[y][x] = '@';
        player.x = x;
        player.y = y;
        return true;
    }

    bool setEnemy(int x = 1, int y = 1, int i = 666) {
        if (map[y][x] == '#')
            return false;
        if (map[y][x] == '@') {
            map[y][x] = '$';
            delPlayer(enemy[i].x, enemy[i].y);
            print();
            cout << " GAME OVER" << endl;
            exit(0);
        }
        if (map[y][x] == '0') {
            enemy.erase(enemy.begin() + i);
            map[y][x] = '#';
            print();
            if (enemy.empty()) {
                cout << "YOU WIN" << endl;
                exit(0);
            }
            return true;
        }
        if (map[y][x] == 'x')
            return false;

        if (i != 666 && checkEnemyMoves(x, y, i)) {
            respawnEnemy(i);
            return true;
        }


        map[y][x] = 'x';
        if (i != 666) {
            enemy[i].y = y;
            enemy[i].x = x;
        }
        if (i != 666)
            writeEnemyMove(x, y, i);
        return true;
    }

    void delPlayer(int x, int y) {
        map[y][x] = ' ';
    }


    void writeEnemyMove(int x, int y, int pos) {
        for (int i = 0; i < 4; ++i) {
            enemy[pos].moves[i] = enemy[pos].moves[i + 1];
        }
        enemy[pos].moves.back().first = x;
        enemy[pos].moves.back().second = y;
    }

    void print(void) {
        system("clear");
        cout << *this << flush;
    }

    bool respawnEnemy(int pos) {
        int x = rand() % xSize;
        int y = rand() % ySize;

        if (map[y][x] == '#' || map[y][x] == '@' || map[y][x] == '0' || map[y][x] == 'x')
            return respawnEnemy(pos);

        map[y][x] = 'x';
        enemy[pos].x = x;
        enemy[pos].y = y;
        return true;

    }

    void enemyMove(void) {
        int i = enemy.size() - 1;
        int pX = player.x;
        int pY = player.y;
        while (i >= 0) {
            int eX = enemy[i].x;
            int eY = enemy[i].y;

            int rX = pX - eX;
            int rY = pY - eY;

            if (rX == 0) {
                if (rY < 0) {
                    if (setEnemy(eX, eY - 1, i))
                        delPlayer(eX, eY);
                    else {
                        if (!setEnemy(eX - 1, eY, i)) {
                            if (setEnemy(eX + 1, eY, i))
                                delPlayer(eX, eY);
                        } else
                            delPlayer(eX, eY);
                    }
                } else {
                    if (setEnemy(eX, eY + 1, i))
                        delPlayer(eX, eY);
                    else {
                        if (!setEnemy(eX - 1, eY, i)) {
                            if (setEnemy(eX + 1, eY, i))
                                delPlayer(eX, eY);
                        } else
                            delPlayer(eX, eY);
                    }
                }
                --i;
                continue;
            }

            if (rY == 0) {
                if (rX < 0) {
                    if (setEnemy(eX - 1, eY, i))
                        delPlayer(eX, eY);
                    else {
                        if (!setEnemy(eX, eY - 1, i)) {
                            if (setEnemy(eX, eY + 1, i))
                                delPlayer(eX, eY);
                        } else
                            delPlayer(eX, eY);
                    }
                } else {
                    if (setEnemy(eX + 1, eY, i))
                        delPlayer(eX, eY);
                    else {
                        if (!setEnemy(eX, eY - 1, i)) {
                            if (setEnemy(eX, eY + 1, i))
                                delPlayer(eX, eY);
                        } else
                            delPlayer(eX, eY);
                    }
                }
                --i;
                continue;
            }


            if (rX >= rY) {
                if (rY < 0) {
                    if (setEnemy(eX, eY - 1, i))
                        delPlayer(eX, eY);
                    else {
                        if (!setEnemy(eX - 1, eY, i)) {
                            if (setEnemy(eX + 1, eY, i))
                                delPlayer(eX, eY);
                        } else
                            delPlayer(eX, eY);
                    }
                } else {
                    if (setEnemy(eX, eY + 1, i))
                        delPlayer(eX, eY);
                    else {
                        if (!setEnemy(eX - 1, eY, i)) {
                            if (setEnemy(eX + 1, eY, i))
                                delPlayer(eX, eY);
                        } else
                            delPlayer(eX, eY);
                    }
                }
            } else {
                if (rX < 0) {
                    if (setEnemy(eX - 1, eY, i))
                        delPlayer(eX, eY);
                    else {
                        if (!setEnemy(eX, eY - 1, i)) {
                            if (setEnemy(eX, eY + 1, i))
                                delPlayer(eX, eY);
                        } else
                            delPlayer(eX, eY);
                    }
                } else {
                    if (setEnemy(eX + 1, eY, i))
                        delPlayer(eX, eY);
                    else {
                        if (!setEnemy(eX, eY - 1, i)) {
                            if (setEnemy(eX, eY + 1, i))
                                delPlayer(eX, eY);
                        } else
                            delPlayer(eX, eY);
                    }
                }
            }
            --i;
        }
    }


    void playerMove(int direct) {

        switch (direct) {
            case UP:
                if (setPlayer(player.x, player.y - 1)) {
                    delPlayer(player.x, player.y + 1);
                }
                break;
            case RIGHT:
                if (setPlayer(player.x + 1, player.y)) {
                    delPlayer(player.x - 1, player.y);
                }
                break;
            case DOWN:
                if (setPlayer(player.x, player.y + 1)) {
                    delPlayer(player.x, player.y - 1);
                }
                break;
            case LEFT:
                if (setPlayer(player.x - 1, player.y)) {
                    delPlayer(player.x + 1, player.y);
                }
                break;
        }
    }


    friend ostream &operator<<(ostream &x, const CMap &b) {

        int size = b.map.size();
        for (int i = 0; i < size; ++i) {
            x << b.map[i] << "\n";
        }
        return x;
    }

private:
    vector<string> map;
    CPlayer player;
    vector<CEnemy> enemy;
    int ySize;
    int xSize;
};

/**####################################################################################*/

int main(void) {

    CMap x("####################################\n"  // U can create ur own map
           "#                                  #\n"
           "#                   ##             #\n"
           "#                   #0             #\n"
           "#                   ##             #\n"
           "#          ##                      #\n"
           "#          0#                      #\n"
           "#          ##         ###          #\n"
           "#                     #0#          #\n"
           "#                                  #\n"
           "####################################\n");
    // x.spawnEnemy(int x, int y, int enemy_number) (where x is an instance of CMap class) - spawns an enemy number "enemy_number" in {x,y} if it's legal. !!! USING THIS METHOD YOU HAVE TO BE SURE, ENEMY WITH "enemy_number" EXISTS !!!. 
    //Leave it commented if you want to spawn enemy at default position.

    /**If you'll find bugs - report it please*/


    x.print();
    while (true) {
        char a;
        a = getchar();


        switch (a) {
            case -1:
                x.playerMove(DOWN);
                x.enemyMove();
                x.print();
                break;
            case 'w':
                x.playerMove(UP);
                x.enemyMove();
                x.print();
                break;
            case 'd':
                x.playerMove(RIGHT);
                x.enemyMove();
                x.print();
                break;
            case 's':

                x.playerMove(DOWN);
                x.enemyMove();
                x.print();
                break;
            case 'a':
                x.playerMove(LEFT);
                x.enemyMove();
                x.print();
                break;
            case 'q':
                cout << "EXIT" << endl;
                return 0;
        }

    }
}

