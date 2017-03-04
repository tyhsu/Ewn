#include <vector>
#include <game.h>

using namespace std;
class Tree_node {
public:
    float score;
    int num_visit;
    int game_status;
    Game game;
    Tree_node *parent;
    vector<*Tree_node> children_list;
    vector<Movement> legal_move_list;
	//Constructor
    Tree_node(int _game_status = 0, Game _game = NULL , Tree_node * _parent = NULL);
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
    Tree_NodeT* get_parent();
	//Add child
    Tree_node * add_child_with_action(int move_index);
}
