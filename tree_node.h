#include <vector>
#include <game.h>

using namespace std;
class tree_node {
public:
    float score;
    int num_visit;
    int game_status
    Game game;
    tree_node * parent;
    vector < *tree_node > children_list;
    vector <Movement> legal_move_list;
    tree_node (int _game_status, Game _game , tree_node * _parent) {
        score = 3;
        num_visit = 0;
        game_status = _game_status;
        game = _game;
        parent = _parent;
        for ( int i = 0; i < 18; i ++ )
            if ( game.check_in_board( Movement movement( i % 6, i % 3) ) )
                legal_move_list.push_back( movement );
    }
    void set_score( float _score ) { this->score = _score; }
    bool is_terminate() { return game != 0; }
    void update(float _score) {
        this->num_visit ++;
        this->score += _score;
    }
    TreeNodeT* get_parent() { 
    	return parent; 
    }
    tree_node * add_child_with_action(int move_index) {
        Game child_game = this->game;
        tree_node * child = new tree_node( child_game.update_game_status( this->legal_move_list[move_index] ),
                        child_game,
                        this );
        children_list.push_back( child );
        return child;
    }
}
