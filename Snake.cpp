#include <bits/stdc++.h>
#include <unordered_map>
#include <cstdlib>
using namespace std;

class Snake
{
private:
    int start;
    int end;

public:
    Snake(int start, int end)
    {
        this->start = start;
        this->end = end;
    }
    int getStart()
    {
        return start;
    }
    int getEnd()
    {
        return end;
    }
};

class Ladder
{
private:
    int start;
    int end;

public:
    Ladder(int start, int end)
    {
        this->start = start;
        this->end = end;
    }
    int getStart()
    {
        return start;
    }
    int getEnd()
    {
        return end;
    }
};

class Player
{
private:
    string name;
    int id;

public:
    Player(string name, int id)
    {
        this->name = name;
        this->id = id;
    }
    string getName()
    {
        return name;
    }
    int getId()
    {
        return id;
    }
};

class SnakeAndLadderBoard
{
private:
    int size;
    vector<Snake> snakes;
    vector<Ladder> ladders;
    unordered_map<int, int> playerPieces;

public:
    SnakeAndLadderBoard(int size)
    {
        this->size = size;
        snakes.clear();
        ladders.clear();
        playerPieces.clear();
    }
    SnakeAndLadderBoard() {}
    int getSize()
    {
        return size;
    }
    vector<Snake> getSnakes()
    {
        return snakes;
    }
    void setSnakes(vector<Snake> snakes)
    {
        this->snakes = snakes;
    }
    vector<Ladder> getLadders()
    {
        return ladders;
    }
    void setLadder(vector<Ladder> ladders)
    {
        this->ladders = ladders;
    }
    unordered_map<int, int> getPlayerPieces()
    {
        return playerPieces;
    }
    void setPlayerPieces(unordered_map<int, int> playerPieces)
    {
        this->playerPieces = playerPieces;
    }
};

class DiceService
{
public:
    static int roll()
    {
        int lower = 1, upper = 6;
        return rand() % (upper - lower + 1) + lower;
    }
};

class SnakeAndLadderService
{
private:
    SnakeAndLadderBoard snakeAndLadderBoard;
    int initialNumberOfPlayers;
    queue<Player> players;

    int numberOfDices;
    bool shouldGameContinueTillLastPlayer;
    bool shouldAllowMultipleDiceRollOnSix;

public:
    SnakeAndLadderService(int boardSize, int numberOfDices)
    {
        snakeAndLadderBoard = SnakeAndLadderBoard(boardSize);
        this->players = queue<Player>();
        this->numberOfDices = numberOfDices;
    }

    void setShouldGameContinueTillLastPlayer(bool shouldGameContinueTillLastPlayer)
    {
        this->shouldGameContinueTillLastPlayer = shouldGameContinueTillLastPlayer;
    }

    void setShouldAllowMultipleDiceRollOnSix(bool shouldAllowMultipleDiceRollOnSix)
    {
        this->shouldAllowMultipleDiceRollOnSix = shouldAllowMultipleDiceRollOnSix;
    }

    void setPlayers(vector<Player> players)
    {
        this->players = queue<Player>();
        this->initialNumberOfPlayers = players.size();
        unordered_map<int, int> playerPieces;
        for (Player player : players)
        {
            this->players.push(player);
            playerPieces[player.getId()] = 0;
        }
        snakeAndLadderBoard.setPlayerPieces(playerPieces);
    }

    void setSnakes(vector<Snake> snakes)
    {
        snakeAndLadderBoard.setSnakes(snakes);
    }

    void setLadders(vector<Ladder> ladders)
    {
        snakeAndLadderBoard.setLadder(ladders);
    }

private:
    int getNewPositionAfterGoingThroughSnakesAndLadders(int newPosition)
    {
        int previousPosition;

        do
        {
            previousPosition = newPosition;
            for (Snake snake : snakeAndLadderBoard.getSnakes())
            {
                if (snake.getStart() == previousPosition)
                {
                    newPosition = snake.getEnd();
                }
            }

            for (Ladder ladder : snakeAndLadderBoard.getLadders())
            {
                if (ladder.getStart() == previousPosition)
                {
                    newPosition = ladder.getEnd();
                }
            }
        } while (newPosition != previousPosition);
        return newPosition;
    }

    void movePlayer(Player player, int positions)
    {
        int oldPosition = snakeAndLadderBoard.getPlayerPieces()[player.getId()];
        int newPosition = oldPosition + positions;
        int boardSize = snakeAndLadderBoard.getSize();

        if (newPosition > boardSize)
        {
            newPosition = oldPosition;
        }
        else
        {
            newPosition = getNewPositionAfterGoingThroughSnakesAndLadders(newPosition);
        }
        unordered_map<int, int> playerPieces = snakeAndLadderBoard.getPlayerPieces();
        playerPieces[player.getId()] = newPosition;
        // snakeAndLadderBoard.getPlayerPieces()[player.getId()] = newPosition;
        snakeAndLadderBoard.setPlayerPieces(playerPieces);
        cout << player.getName() << " rolled a " << positions << " and moved from " << oldPosition << " to " << newPosition << endl;
    }

    int getTotalValueAfterDiceRolls()
    {
        return DiceService::roll();
    }

    bool hasPlayerWon(Player player)
    {
        int playerPosition = snakeAndLadderBoard.getPlayerPieces()[player.getId()];
        int winningPosition = snakeAndLadderBoard.getSize();
        return playerPosition == winningPosition;
    }

    bool isGameCompleted()
    {
        int currentNumberOfPlayers = players.size();
        return currentNumberOfPlayers < initialNumberOfPlayers;
    }

public:
    void startGame()
    {
        while (!isGameCompleted())
        {
            int totalDiceValue = getTotalValueAfterDiceRolls();
            Player currentPlayer = players.front();
            players.pop();
            movePlayer(currentPlayer, totalDiceValue);
            if (hasPlayerWon(currentPlayer))
            {
                cout << currentPlayer.getName() << " wins the game!!" << endl;
                snakeAndLadderBoard.getPlayerPieces().erase(currentPlayer.getId());
            }
            else
            {
                players.push(currentPlayer);
            }
        }
    }
};

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    int noOfSnakes;
    cin >> noOfSnakes;
    vector<Snake> snakes;
    for (int i = 0; i < noOfSnakes; i++)
    {
        int start, end;
        cin >> start >> end;
        snakes.push_back(Snake(start, end));
    }
    int noOfLadders;
    cin >> noOfLadders;
    vector<Ladder> ladders;
    for (int i = 0; i < noOfLadders; i++)
    {
        int start, end;
        cin >> start >> end;
        ladders.push_back(Ladder(start, end));
    }
    int noOfPlayers;
    cin >> noOfPlayers;
    vector<Player> players;
    for (int i = 0; i < noOfPlayers; i++)
    {
        string name;
        int id;
        cin >> name >> id;
        players.push_back(Player(name, id));
    }
    int boardSize = 100;
    int noOfDices = 1;
    SnakeAndLadderService snakeAndLadderService(boardSize, noOfDices);
    snakeAndLadderService.setPlayers(players);
    snakeAndLadderService.setSnakes(snakes);
    snakeAndLadderService.setLadders(ladders);
    snakeAndLadderService.startGame();
}
