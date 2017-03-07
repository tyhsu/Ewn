#include <vector>
#include "game.h"

using namespace std;
class Tree_node {
public:
    float score;
    int num_visit;
    int game_status;
    Game game;
    Tree_node* parent;
    bool is_legal_list[18];
    Movement legal_move_list[18];
	Tree_node* children_list[18];
	//Default Constructor
	Tree_node();
	//Constructor
    Tree_node(int _game_status = 0, Game _game = Game() , Tree_node * _parent = NULL);
	//Copy Constructor
	Tree_node(Tree_node* _Tree_node);
	//Copy operator
	void operator=(Tree_node* _Tree_node);
	//Set Score
    void set_score(float _score);
	//If the node is a end-gamed node
    bool is_terminate();
	//Update score and num_visit
    void update(float _score);
	//Get parent
    Tree_node* get_parent();
};
