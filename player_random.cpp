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
std::vector<Point> next_valid_spots;

//位置的權重
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

/////現在棋盤落子位置的權重

int state_value(int index)
{
    int x = next_valid_spots[index].x;
    int y = next_valid_spots[index].y;
    int score = wieght[x][y];
    return score;
}

Point maxtree(int Index)
{
    int t = next_valid_spots.size();
    int score[t];
    //int array[t+1];
    //array[0] = 0;
    int max = -99;
    int max_index = 0;
    for(int i=0;i<t;i++)
    {
        score[i] = state_value[i - 1];

        //int j = 0;
        /*while(array[j] != 0)//traverse 下去
        {
            if(score[i] > array[j]) // 如果是>現在這個節點，就往右邊放
            {
                j = j * 2 + 1;//右邊
            }
            else if(score[i] < array[j]) // 如果是<現在這個節點就往左邊放
            {
                j *= 2; // 左邊
            }
            else if(score[i] == array[j]) // 如果值一樣的話就不插入
            {
                return;
            }
        }
        array[j] = score[i];*/
        if(score[i] > max)
        {
            max = score[i];
            max_index = i;
        }
    }

    return next_valid_spots[max_index];
}

void write_valid_spot(std::ofstream& fout)
{
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    //int index = (rand() % n_valid_spots);
    //Point p = next_valid_spots[index];
    // Remember to flush the output to ensure the last action is written to file.
    Point p = state_value();
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
