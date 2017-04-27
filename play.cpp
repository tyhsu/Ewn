#include <iostream>
#include <algorithm>
#include "play.h"
using namespace std;

Movement Play::player_move(const int& dice)
{
    int movable_chs_cnt = this->game.count_movable_chs(dice);
    int chs_index, direct;
    Movement mvmt;

    do {
        cout << endl << "Next chessman able to move: ";
        for (int i=0; i<movable_chs_cnt; i++)
            cout << i << ")" << this->game.get_movable_chs(i).symbol << " ";
        cout << endl;
        cout << "Choose: ";
        cin >> chs_index;

        if (!this->game.get_is_switch())
            cout << "Direction: 0)Right 1)Down 2)Right-down" << endl;
        else
            cout << "Direction: 0)Left 1)Up 2)Left-up" << endl;
        cout << "Choose: ";
        cin >> direct;

        // get index
        chs_index = (this->game.get_is_switch() == false) ? this->game.get_movable_chs(chs_index).symbol - '1' : this->game.get_movable_chs(chs_index).symbol - 'A';
        mvmt.first = chs_index;
        mvmt.second = direct;
        //If the movement is illegal, run the loop
    } while (!this->game.check_in_board(mvmt));

    return mvmt;
}

void Play::two_players_mode()
{
    this->game.print_board();
    while (1) {
        //update: 0(game continues), 1(A wins), 2(B wins)
        int dice = this->game.roll_dice();
        cout << "dice: " << dice+1 << endl;
        int game_status = this->game.update_game_status(player_move(dice));
        this->game.print_board();
        if (game_status != 0) {
            cout << "====================================" << endl;
            if (game_status == 1) cout << "A is the winner!!!" << endl;
            else cout << "B is the winner!!!" << endl;
            cout << "====================================" << endl;
            return;
        }
        this->game.switch_player();
    }
}

void Play::player_AI_mode()
{
    EwnAI ewnAI;
    this->game.print_board();
    while (1) {
        int dice = this->game.roll_dice();
        cout << "dice: " << dice+1 << endl;
        int game_status;
        if (this->game.get_is_switch() == true) {
            game_status = this->game.update_game_status(player_move(dice));
        }
        else
            game_status = this->game.update_game_status(ewnAI.AI_move(this->game, dice));

        this->game.print_board();
        //update: 0(game continues), 1(A wins), 2(B wins)
        if (game_status != 0) {
            cout << "====================================" << endl;
            if (game_status == 1) cout << "AI is the winner!!!" << endl;
            else cout << "B is the winner!!!" << endl;
            cout << "====================================" << endl;
            return;
        }
        this->game.switch_player();
    }
}

void Play::two_AIs_mode()
{
    cout << "==========The first AI (A)==========" << endl;
    EwnAI ewnAI1;
    cout << "==========The second AI (B)==========" << endl;
    EwnAI ewnAI2;
    this->game.print_board();
    while (1) {
        int dice = this->game.roll_dice();
        cout << "dice: " << dice+1 << endl;

        int game_status;
        if (!this->game.get_is_switch())	// the first AI (A)
            game_status = this->game.update_game_status(ewnAI1.AI_move(this->game, dice));
        else					// the second AI (B)
            game_status = this->game.update_game_status(ewnAI2.AI_move(this->game, dice));
        this->game.print_board();

        //update: 0(game continues), 1(A wins), 2(B wins)
        if (game_status!=0) {
            cout << "====================================" << endl;
            if (game_status==1) cout << "A is the winner!!!" << endl;
            else cout << "B is the winner!!!" << endl;
            cout << "====================================" << endl;
            return;
        }
        this->game.switch_player();
    }
}

void Play::contest_AI_mode()
{
    int is_right_upper_side;
    cout << "Whose AI is at the left upper section?" << endl;
    cout << "=>Who is A (1~6)? 0)Ours 1)Opponent's" << endl;
    cout << "Choose: ";
    cin >> is_right_upper_side;

    int is_init_board, is_switch;
    cout << "Do you need to set the board? 0)No 1)Yes" << endl;
    cout << "Choose: ";
    cin >> is_init_board;
    if (is_init_board == 1) {
        char board[5][5];
        do {
            cout << endl << "Please give values for the 5x5 array" << endl;
            char c;
            for (int i=0; i<5; i++) {
                for (int j=0; j<5; j++) {
                    cin >> c;
                    if (c == '0')
                        board[i][j] = 0;
                    else
                        board[i][j] = c;
                }
            }
            cout << "Are you ready to set the board? 0)No 1)Yes" << endl;
            cout << "Choose: ";
            cin >> is_init_board;
        } while (!is_init_board);

        cout << endl << "Whose turn is it now? 0)A 1)B" << endl;
        cout << "Choose: ";
        cin >> is_switch;
        this->game.set_board(board, is_switch);
    }
    cout << endl;
    this->game.print_board();

    EwnAI ewnAI;
    while (1) {
        int game_status;
        //The opponent's turn (A)
        if (is_right_upper_side && !this->game.get_is_switch()) {
            char chs_symbol;
            int direct;
            cout << endl << "Choose the chessman(1~6): ";
            cin >> chs_symbol;
            if (!(chs_symbol>='1' && chs_symbol<='6'))
                continue;

            cout << "Direction: 0)Right 1)Down 2)Right-down" << endl;
            cout << "Choose: ";
            cin >> direct;
            if (!(direct>=0 && direct<=2))
                continue;

            int is_recover;
            cout << "Do you want to recover? 0)No 1)Yes" << endl;
            cout << "Choose: ";
            cin >> is_recover;
            if (is_recover != 0)
                continue;

            game_status = this->game.update_game_status(make_pair(chs_symbol-'1', direct));
        }

        //The opponent's turn (B)
        else if (!is_right_upper_side && this->game.get_is_switch()) {
            char chs_symbol;
            int direct;
            cout << endl << "Choose the chessman(A~F): ";
            cin >> chs_symbol;
            if (!(chs_symbol>='A' && chs_symbol<='F'))
                continue;

            cout << "Direction: 0)Left 1)Up 2)Left-up" << endl;
            cout << "Choose: ";
            cin >> direct;
            if (!(direct>=0 && direct<=2))
                continue;

            int is_recover;
            cout << "Do you want to recover? 0)No 1)Yes" << endl;
            cout << "Choose: ";
            cin >> is_recover;
            if (is_recover != 0)
                continue;

            game_status = this->game.update_game_status(make_pair(chs_symbol-'A', direct));
        }

        //Our AI's turn
        else {
            char dice;
            cout << endl << "Set the dice(1~6): ";
            cin >> dice;
            if (!(dice>='1' && dice<='6'))
                continue;

            game_status = this->game.update_game_status(ewnAI.AI_move(this->game, dice-'1'));
        }

        this->game.print_board();
        //update: 0(game continues), 1(A wins), 2(B wins)
        if (game_status!=0) {
            cout << "====================================" << endl;
            if ((is_right_upper_side && game_status==1) || (!is_right_upper_side && game_status==2))
                cout << "The opponent is the winner!!!" << endl;
            else
                cout << "Our AI is the winner!!!" << endl;
            cout << "====================================" << endl;
            return;
        }
        this->game.switch_player();
    }
}

//Compare two AIs with several games
void Play::compare_AI_mode()
{
    char mode_name[10][25] = { "evaluate_feature",
                            "simulate_rand_type1",
                            "simulate_rand_type2",
                            "simulate_rand_type3",
                            "simulate_rand_type4",
                            "MCTS_simulate_rand_type1", \
                            "MCTS_simulate_rand_type2", \
                            "MCTS_simulate_rand_type3", \
                            "MCTS_simulate_rand_type4", \
                            "Random"
                           };

    int mode_A, mode_B;
    int game_cnt;
    double avg_move_timer_A1 = 0, avg_move_timer_A2 = 0, avg_move_timer_B1 = 0, avg_move_timer_B2 = 0;
    int win_cnt_A1 = 0, win_cnt_A2 = 0, win_cnt_B1 = 0, win_cnt_B2 = 0;
    double max_time_cost_A = 0, max_time_cost_B = 0;
    double total_time_cost_A = 0, total_time_cost_B = 0;
    cout << "Please choose two AIs:" << endl;
    cout << "0)Minimax evaluating with feature" << endl;
    cout << "1)Minimax evaluating with simulation 1" << endl;
    cout << "2)Minimax evaluating with simulation 2" << endl;
    cout << "3)Minimax evaluating with simulation 3" << endl;
    cout << "4)Minimax evaluating with simulation 4" << endl;
    cout << "5)MCTS evaluating with simulation 1" << endl;
    cout << "6)MCTS evaluating with simulation 2" << endl;
    cout << "7)MCTS evaluating with simulation 3" << endl;
    cout << "8)MCTS evaluating with simulation 4" << endl;
    cout << "9)Random                              " << endl;
    cout << "Choose: ";
    cout << "modeA choose: ";
    cin >> mode_A;
    cout << "modeB choose: ";
    cin >> mode_B;
    cout << "Please enter the count of the games: ";
    cin >> game_cnt;

    // modeA plays first, then modeB
    cout << "========== modeA then modeB ==========" << endl;
    for(int i = 0; i < game_cnt ; i++){
        Game init_game = this->game;
        EwnAI ewnAI1(mode_A);
        EwnAI ewnAI2(mode_B);
        while (1) {
            int debug = 0;
            int dice = init_game.roll_dice();

            int game_status;
            if (!init_game.get_is_switch()) {	// the first AI (A)
                cerr << "-";
                cerr.flush();
                clock_t init_time = clock();
                Movement mvmt = ewnAI1.AI_move(init_game, dice);
                game_status = init_game.update_game_status(mvmt);
                double time_cost = (double)(clock() - init_time) / CLOCKS_PER_SEC;
                max_time_cost_A = max(max_time_cost_A, time_cost);
                total_time_cost_A += time_cost;
                avg_move_timer_A1 += 1;
            }
            else {								// the second AI (B)
                cerr << "-";
                cerr.flush();
                clock_t init_time = clock();
                Movement mvmt = ewnAI2.AI_move(init_game, dice);
                game_status = init_game.update_game_status(mvmt);
                double time_cost = (double)(clock() - init_time) / CLOCKS_PER_SEC;
                max_time_cost_B = max(max_time_cost_B, time_cost);
                total_time_cost_B += time_cost;
                avg_move_timer_B1 += 1;
            }
            //update: 0(game continues), 1(A wins), 2(B wins)
            if (game_status!=0) {
                if (game_status==1) {
                    win_cnt_A1 ++ ;
                    cout << "A wins.  " << win_cnt_A1 + win_cnt_B1 << " games." << endl;
                }

                else {
                    win_cnt_B1 ++;
                    cout << "B wins.  " << win_cnt_A1 + win_cnt_B1 << " games." << endl;
                }
                break;
            }
            init_game.switch_player();
        }
    }
    printf("%-25s%-10s%-10s%-10s\n", "mode", "wins", "max_time_cost", "total_time_cost");
    printf("%-25s%-10d%-10lf%-10lf\n", mode_name[mode_A], win_cnt_A1, max_time_cost_A, total_time_cost_A);
    printf("%-25s%-10d%-10lf%-10lf\n", mode_name[mode_B], win_cnt_B1, max_time_cost_B, total_time_cost_B);

    // modeB plays first, then modeA
    cout << "========== modeB then modeA ==========" << endl;
    for (int i = 0; i < game_cnt ; i++){
        Game init_game = this->game;
        EwnAI ewnAI1(mode_B);
        EwnAI ewnAI2(mode_A);
        while (1) {
            int debug = 0;
            int dice = init_game.roll_dice();

            int game_status;
            if (!init_game.get_is_switch()) {	// the first AI (B)
                cerr << "-";
                cerr.flush();
                clock_t init_time = clock();
                Movement mvmt = ewnAI1.AI_move(init_game, dice);
                game_status = init_game.update_game_status(mvmt);
                double time_cost = (double)(clock() - init_time) / CLOCKS_PER_SEC;
                max_time_cost_B = max(max_time_cost_B, time_cost);
                avg_move_timer_B2 += 1;
            }
            else {							// the second AI (A)
                cerr << "-";
                cerr.flush();
                clock_t init_time = clock();
                Movement mvmt = ewnAI2.AI_move(init_game, dice);
                game_status = init_game.update_game_status(mvmt);
                double time_cost = (double)(clock() - init_time) / CLOCKS_PER_SEC;
                max_time_cost_A = max(max_time_cost_A, time_cost);
                avg_move_timer_A2 += 1;
            }
            //update: 0(game continues), 1(B wins), 2(A wins)
            if (game_status!=0) {
                if (game_status==1) {
                    win_cnt_B2 ++ ;
                    cout << "B wins.  " << win_cnt_A2 + win_cnt_B2 << " games." << endl;
                }

                else {
                    win_cnt_A2 ++;
                    cout << "A win.  " << win_cnt_A2 + win_cnt_B2 << " games." <<endl;
                }
                break;
            }
            init_game.switch_player();
        }
    }
    cout << endl << "==================================================" << endl;
    cout << "========== modeA then modeB ==========" << endl;
    printf("%-30s%-10s%-15s%-20s%-20s%s\n", "mode", "wins", "max_time_cost", "total_time_cost", "move_count", "time_cost_per_move(avg)");
    printf("%-30s%-10d%-15lf%-20lf%-20lf%lf\n", mode_name[mode_A], win_cnt_A1, max_time_cost_A, total_time_cost_A, avg_move_timer_A1, total_time_cost_A/avg_move_timer_A1);
    printf("%-30s%-10d%-15lf%-20lf%-20lf%lf\n", mode_name[mode_B], win_cnt_B1, max_time_cost_B, total_time_cost_B, avg_move_timer_B1, total_time_cost_B/avg_move_timer_B1);

    cout << "========== modeB then modeA ==========" << endl;
    printf("%-30s%-10s%-15s%-20s%-20s%s\n", "mode", "wins", "max_time_cost", "total_time_cost", "move_count", "time_cost_per_move(avg)");
    printf("%-30s%-10d%-15lf%-20lf%-20lf%lf\n", mode_name[mode_A], win_cnt_A2, max_time_cost_A, total_time_cost_A, avg_move_timer_A2, total_time_cost_A/avg_move_timer_A2);
    printf("%-30s%-10d%-15lf%-20lf%-20lf%lf\n", mode_name[mode_B], win_cnt_B2, max_time_cost_B, total_time_cost_B, avg_move_timer_B2, total_time_cost_B/avg_move_timer_B2);
}
