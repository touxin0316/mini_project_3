#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Point
{
    int x, y;
};

int player;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::array<std::array<int, SIZE>, SIZE> boardstate;
std::vector<Point> next_valid_spots;

//位置的權重
/*
const int weight[8][8] =
{
    65, -3, 6, 4, 4, 6, -3, 65,
    -3, -29, 3, 1, 1, 3, -29, -3,
    6, 3, 5, 3, 3, 5, 3, 6,
    4, 1, 3, 1, 1, 3, 1, 4,
    4, 1, 3, 1, 1, 6, 1, 4,
    6, 3, 5, 3, 5, 5, 3, 6,
    -3, -29, 3, 1, 1, 3, -29, -3,
    65, -3, 6, 4, 4, 6, -3, 65,
};
*/

const int dx[] = {1,-1,0,0,1,-1,1,-1};
const int dy[] = {0,0,1,-1,1,-1,-1,1};

void read_board(std::ifstream& fin)
{
    fin >> player;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            fin >> board[i][j];
        }
    }
}

void read_valid_spots(std::ifstream& fin)
{
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++)
    {
        fin >> x >> y;
        next_valid_spots.push_back({x, y});
    }
}

void resetboard()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            boardstate[i][j] = board[i][j];
        }
    }
}

int flipnumber(int x, int y, int player)
{
    int flip[8] = {};
    int flipmax = 0;
    for(int i=0;i<8;i++)
    {
        int newx = x + dx[i];
        int newy = y + dy[i];
        while(boardstate[newx][newy] == 3 - player)
        {
            if(boardstate[newx + dx[i]][newy + dy[i]] == player)
            {
                flip[i] += 1;
                boardstate[newx][newy] = player;
                break;
            }
            else if(boardstate[newx + dx[i]][newy + dy[i]] == 3 - player)
            {
                flip[i] += 1;
                boardstate[newx][newy] = player;
                newx += dx[i];
                newy += dy[i];
            }
            else
            {
                flip[i] = 0;
                break;
            }
        }
    }
    for(int i=0;i<8;i++)
    {
        flipmax += flip[i];
    }
    //cout << flipmax << endl;
    return flipmax;
}


int state_value(int x, int y, int player)
{
    int score = 0;
    int distance = 0;
    if((x == 7 && y == 7) || (x == 0 && y == 0) || (x == 7 && y == 0) || (x == 0 || y == 7))
    {
        score = 65;
    }
    else
    {
        distance = abs(x - 3) + abs(y - 3);
        if(distance >= 4)
            score = -29;
        else
            score = distance;
    }
    score += flipnumber(x,y,player);
    return score;
}

bool isvalid(int x, int y, int player)
{
    bool lastvalid = false;
    int flip[8] = {};
    int flipmax = 0;
    if(board[x][y] == 0)
    {
        for(int i=0;i<8;i++)
        {
            int newx = x + dx[i];
            int newy = y + dy[i];
            while(board[newx][newy] == 3 - player)
            {
                if(board[newx + dx[i]][newy + dy[i]] == player)
                {
                    flip[i] += 1;
                    break;
                }
                else if(board[newx + dx[i]][newy + dy[i]] ==  3 - player)
                {
                    newx += dx[i];
                    newy += dy[i];
                    flip[i] += 1;
                }
                else
                {
                    flip[i] = 0;
                    break;
                }
            }
        }
        for(int i=0;i<8;i++)
        {
            flipmax += flip[i];
        }
    }

    if(flipmax > 0)
        lastvalid = true;
    //if(lastvalid)
        //cout << x << " " << y << endl;
    return lastvalid;
}
/*
Point minmaxtree()
{
    int t = next_valid_spots.size();
    int score[t];
    int max = -99;
    int max_index = 0;
    for(int i=0;i<t;i++)
    {
        int x = next_valid_spots[i].x;
        int y = next_valid_spots[i].y;
        score[i] = state_value(x,y,player);
        board[x][y] = player;
        int newmax = -99;
        for(int j=0;j<8;j++)
        {
            for(int k=0;k<8;k++)
            {
                int newscore = 0;
                if(isvalid(j, k))
                    newscore = state_value(j, k, 3 - player);
                else
                    continue;
                if(newscore > newmax)
                    newmax = newscore;
            }
        }
        if(score[i] - newmax > max)
        {
            max = score[i] - newmax;
            max_index = i;
        }
        board[x][y] = 0;
    }
    //cout << max << endl;
    return next_valid_spots[max_index];
}
*/
int minimax(int depth ,int x, int y, bool ismax, int player)
{
    //boardstate[x][y] = player;
    if(depth == 0)
    {
        return state_value(x, y, player);
    }

    if(ismax)
    {
        int maxval = -99;
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                if(isvalid(i,j, player))
                {
                    boardstate[i][j] = player;
                    int eval = minimax(depth - 1, i, j, false, 3 - player);
                    maxval = std::max(maxval,eval);
                    boardstate[i][j] = 0;
                    //cout << maxval << endl;
                }
            }
        }
        boardstate[x][y] = 0;
        return maxval;
    }
    else
    {
        int minval = 99;
        for(int i=0;i<8;i++)
        {
            for(int j=0;j<8;j++)
            {
                if(isvalid(i,j,3 - player))
                {
                    boardstate[i][j] = 3 - player;
                    int eval = minimax(depth - 1, i, j, true, player);
                    minval = std::min(minval,eval);
                    boardstate[i][j] = 0;
                }
            }
        }
        boardstate[x][y] = 0;
        return minval;
    }
}

void write_valid_spot(std::ofstream& fout)
{
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    resetboard();
    int t = next_valid_spots.size();
    int score[t];
    int max = -99;
    int max_index = 0;
    for(int i=0;i<t;i++)
    {
        int x = next_valid_spots[i].x;
        int y = next_valid_spots[i].y;
        boardstate[x][y] = player;
        score[i] = minimax(2,x,y,true,player);
        //cout << score[i] << endl;
        if(score[i] > max)
        {
            max = score[i];
            max_index = i;
        }
        //printboard();
        resetboard();
    }
    Point p = next_valid_spots[max_index];
    fout << p.x << " " << p.y << std::endl;
    fout.flush();
}

int main(int, char** argv)
{
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
