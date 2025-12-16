
#include "ArcadiaEngine.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <iomanip>



// --- 2. Leaderboard (Skip List) ---

class ConcreteLeaderboard : public Leaderboard {
private:
    struct Node {
        int id, score;
        vector<Node*> forward; // skip list levels
    };
    int MAX_LEVEL = 5;
    Node* head;
    int level;
    Node* update[6];
    void Search(int playerID, int score) {
        Node* current = head;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != nullptr &&
                ((current->forward[i]->score > score)||(current->forward[i]->score==score && current->forward[i]->id < playerID)))
            {
                current = current->forward[i];
            }
            update[i] = current;
        }


    }
    int getScore(int playerID) {
        Node* current = head->forward[0]; // level 0
        while(current != nullptr) {
            if(current->id == playerID) {
                return current->score;
            }
            current = current->forward[0];
        }
        return -1; // if player not exist
    }

public:
    ConcreteLeaderboard() {
        head = new Node();
        level = 0;
        head->forward.resize(MAX_LEVEL+1,nullptr);

    }


    void addScore(int playerID, int score) override {
        // TODO: Implement skip list insertion
        //Search for the correct position
        Search(playerID, score);

        //Randomize the level for the new node

        int newlevel=0;
        while (rand()%2 && newlevel < MAX_LEVEL) {
            newlevel++;
        }
        if (newlevel > level) {
            for (int i = level+1; i <= newlevel; i++) {
                update[i] = head;
            }
            level = newlevel;
        }
        //creation of new node and add it in the right position
        Node* newNode = new Node();
        newNode->id = playerID;
        newNode->score = score;
        newNode->forward.resize(newlevel + 1, nullptr);
        for (int i = 0; i <= newlevel; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }



    }

    void removePlayer(int playerID) override {
        int score = getScore(playerID);
        if(score == -1) {
            cout << "Player " << playerID << " does not exist" << endl;
            return;
        }

        Search(playerID, score);
        Node* DeletedNode = update[0]->forward[0];

        for(int i = 0; i <= level; i++) {
            if(update[i]->forward[i] == DeletedNode)
                update[i]->forward[i] = DeletedNode->forward[i];
        }
        // IF THE NODE AT THE LAST LEVEL
        while(level > 0 && head->forward[level] == nullptr) {
            level--;
        }

        delete DeletedNode; // deallocate
    }

    vector<int> getTopN(int n) override {
        // TODO: Return top N player IDs in descending score order
        vector<int> result;
        int count=0;
        Node* current = head->forward[0];
        while(current != nullptr && count<n) {
            result.push_back(current->id);
            current = current->forward[0];
            count++;
        }
        return result;
    };

    // =========================================================
    // PART D: SERVER KERNEL (Greedy)
    // =========================================================


};
int ServerKernel::minIntervals(vector<char>& tasks, int n) {
    unordered_map<char, int> freq;

    // count frequency
    if(!tasks.empty()) {
        for(char t : tasks) {
            freq[t]++;
        }

        int maxFreq = 0;
        for(auto &p : freq) {
            maxFreq = max(maxFreq, p.second);
        }

        int countMax = 0;
        for(auto &p : freq) {
            if(p.second == maxFreq)
                countMax++;
        }

        int intervals = (maxFreq - 1) * (n + 1) + countMax;

        return max(intervals, (int)tasks.size());
    }else {
        return -1;
    }
}
