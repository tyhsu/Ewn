#ifndef MCTS3_H
#define MCTS3_H

#include <stdlib.h>
#include "mcts.h"
using namespace std;

class MCTS3: public MCTS
{
public:
    MCTS3() {
        this->selection_ptr = new UCT(eps);
    }

    // ~MCTS3() {
    //     delete this->selection_ptr;
    // }

    Movement AI_move(Game& cur_game, int dice) {
        this->max_iterations = 100;
        this->ai_side = cur_game.get_is_switch();
        this->ai_symbol = this->ai_side? 'A': '1';

        int best_val = -1e9, child_val;
        int win = this->ai_side? 2: 1;
        int next_move_cnt = cur_game.count_movable_chs(dice);
        Movement answer;
        for (int i=0; i<next_move_cnt; i++) {
            int chs_index = cur_game.get_movable_chs(i).symbol - this->ai_symbol;
            for (int direct = 0; direct < 3; direct++) {
                Movement mvmt(chs_index, direct);
                if (cur_game.check_in_board(mvmt)) {
                    Game child_game = cur_game;
                    int game_status = child_game.update_game_status(mvmt);
                    child_game.switch_player();

                    // check if the game ends
                    if (game_status == win) {
                        answer = mvmt;
                        break;
                    }
                    else if (game_status != 0)	// lose the game
                        continue;
                    else {
                        child_val = this->run(child_game);
                    }

                    // update the best value and the best movement
                    if (child_val >= best_val) {
                        best_val = child_val;
                        answer = mvmt;
                    }
                }
            }
        }
        return answer;
    }

    // mcts main
    int run(const Game& cur_game) {
        // initialize root Tree_node with current state
        Tree_node* root_node_ptr = new Tree_node(cur_game);

        int iteration = 0;  // nodes
        while(iteration++ < this->max_iterations) {
            // 1. select
            // Start from the root, digging down until finding an unvistied node
            Tree_node* node_ptr = root_node_ptr;
            int best_child_index;
            // The root has been visited
            if (root_node_ptr->is_visit()) {
                while(!node_ptr->is_terminate()) {
                    best_child_index = this->selection_ptr->select_children_list_index(node_ptr);
                    node_ptr = node_ptr->get_child_ptr(best_child_index);
                    // The next child pointer hasn't been visited
                    if(!node_ptr->is_visit()) {
                        break;
                    }
                }
            }

            int win_reward;
            if (node_ptr->is_terminate()) {
                win_reward = node_ptr->get_game_status() == (this->ai_side? 2: 1);
            }
            else {
                // 2. expand by adding a single child (if not terminal or not fully expanded)
                node_ptr->new_child_nodes();

                // 3. simulate
                win_reward = this->simulation(node_ptr->game);
            }

            // 4. back propagation
            while(true) {
                node_ptr->update(win_reward);
                if(node_ptr->get_parent_ptr() == NULL) break;
                node_ptr = node_ptr->get_parent_ptr();
            }
        }

        int result = root_node_ptr->get_win_count();
        this->recursive_delete_tree_node(root_node_ptr);
        return result;
    }

    int simulation(const Game& cur_game) {
        Game simu_game = cur_game;
        char cur_symbol;
        int game_status = 0, ai_win = this->ai_side ? 2 : 1;
        if (simu_game.get_is_switch() == this->ai_side)
            cur_symbol = this->ai_symbol;
        else
            cur_symbol = this->ai_side? '1' : 'A';

        while (game_status == 0) {
            // random a chess
            int chs_index = rand() % 6;
            while (!simu_game.get_cur_chs_list(chs_index).exist) {
                chs_index = rand() % 6;
            }
            // random a direction according the chess
            int direct = rand() % 3;
            Movement mvmt(chs_index, direct);
            while (!simu_game.check_in_board(mvmt)) {
                direct = rand() % 3;
                mvmt.second = direct;
            }
            // check game status => if the game keep going, switch the player.
            game_status = simu_game.update_game_status(mvmt);
            if(game_status == 0) simu_game.switch_player();
            else break;
        }
        return (game_status == ai_win? 1: 0);
    }

    void recursive_delete_tree_node(Tree_node* node_ptr) {
        for (int i = 0; i < 18; i++) {
            if (node_ptr->get_child_ptr(i)) {
                recursive_delete_tree_node(node_ptr->get_child_ptr(i));
            }
        }
        delete node_ptr;
    }
};

#endif
