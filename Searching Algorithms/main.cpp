#include <iostream>
#include <algorithm>
#include <string>
#include <stack>
#include <vector>
using namespace std;

// This vector will be used to
// save visited states when needed.
vector<string> visited;

// Global goal state
string goal = "187206345";

// Node to represent a single state
struct node{
    node* parent;
    string state;
    int depth, action, cost; 
	
	// Action would be: 1 Up, 2 Right, 3 Down or 4 Left
    
	node()
    {
        depth = 0;
        action = 0;
        parent = 0;
        cost = 0;
    }
};

// This heuristic function counts number
// of blocks that are not in place.

int heuristic(string goal, string state){
    int h = 0;
    for(int i=0; i<state.size(); i++)
        if(state[i]!=goal[i]) h++;
    return h;
}

// Compare function for UCS sort
bool ucs_cmp(node* i, node* j){
    return i->cost < j->cost;
}

// Compare function for Greedy search sort
bool heuristic_cmp(node* i, node* j){
    return heuristic(goal, i->state) < heuristic(goal, j->state);
}

// Compare function for A* search sort
bool cost_heuristic_cmp(node* i, node* j){
    return i->cost + heuristic(goal, i->state) < j->cost + heuristic(goal, j->state);
}

// Function to check for visited state in the vector.
bool is_visited(string state){
    vector<string>::iterator it;
    it = find(visited.begin(), visited.end(), state);
    if(it==visited.end()) return false;
    return true;
}

// Function to create new state node.
node* create_node(string State, node* Parent, int Action, int Depth, int Cost = 0){
    if(State == "x") return 0;
    node* Nod = new node();
    Nod->state = State;
    Nod->parent = Parent;
    Nod->depth = Depth;
    Nod->action = Action;
    Nod->cost = Cost;
    return Nod;
}

// Function to create a new state based on action.
string Move(int dir, string state){
    int i = 0;
    while(state[i]!='0') i++;
    switch(dir){
        case 1:
            if(i==0 || i==3 || i==6) return "x";
            swap(state[i],state[i-1]);
            break;
        case 2:
            if(i==6 || i==7 || i==8) return "x";
            swap(state[i],state[i+3]);
            break;

        case 3:
            if(i==2 || i==5 || i==8) return "x";
            swap(state[i],state[i+1]);
            break;

        case 4:
            if(i==0 || i==1 || i==2) return "x";
            swap(state[i],state[i-3]);
            break;

        default:
            return "x";
    }

    return state;
}

// Function to create child nodes holding possible moves.
vector<node*> expand(node* Nod, bool cumulative_cost = false){
    vector<node *> ex;
    string s = Nod->state;
    int d = (Nod->depth)+1, c = 0;
    if(cumulative_cost) c = Nod->cost + 1;
    ex.push_back(create_node(Move(1,s),Nod,1,d, c));
    ex.push_back(create_node(Move(2,s),Nod,2,d, c));
    ex.push_back(create_node(Move(3,s),Nod,3,d, c));
    ex.push_back(create_node(Move(4,s),Nod,4,d, c));
    return ex;
}

// Breadth First Search
vector<int> bfs(node* start, string goal){
    int idx = 0;
    vector<int> moves;
    vector<node*> nodes;
    nodes.push_back(start);
    while(true){
        node* current = nodes[idx++];
        if(current->state == goal){
            while(current->parent!=0){
                moves.push_back(current->action);
                current = current->parent;
            }
            return moves;
        }

        vector<node* > temp = expand(current);

        for(size_t i = 0; i < temp.size(); i++)
            if(temp[i]!=0 && temp[i]->action)
                nodes.push_back(temp[i]);
            else continue;
    }
}

// Depth First Search
vector<int> dfs(node* start, string goal, int Depth = 10){
    vector<int> moves;
    stack<node* >nodes;
    nodes.push(start);
    while(true){
        if(nodes.size()<1) {cout<<"Solution not found!\n"; return moves;}
        node* current = nodes.top(); nodes.pop();
        if(current->state == goal){
            while(current->parent!=0){
                moves.push_back(current->action);
                current = current->parent;
            }
            return moves;
        }
        vector<node* > temp = expand(current);
        for(size_t i = 0; i < temp.size(); i++){
            if(Depth){
                if(temp[i]!=0 && temp[i]->action && temp[i]->depth <= Depth)
                    nodes.push(temp[i]);
            }
            else{
                if(temp[i]!=0 && temp[i]->action && !is_visited(temp[i]->state)){
                    nodes.push(temp[i]);
                    visited.push_back(temp[i]->state);
                }
            }
        }
    }
}

// Uniform Cost Search
vector<int> ucs(node* start, string goal){
    int idx = 0;
    vector<int> moves;
    vector<node*> nodes;
    nodes.push_back(start);
    while(true){
        node* current = nodes[idx++];
        if(current->state == goal){
            while(current->parent!=0){
                moves.push_back(current->action);
                current = current->parent;
            }
            return moves;
        }

        vector<node* > temp = expand(current, true);
        for(size_t i = 0; i < temp.size(); i++)
            if(temp[i]!=0 && temp[i]->action)
                nodes.push_back(temp[i]);

        sort(nodes.begin()+idx, nodes.end(), ucs_cmp);
    }

}

// Greedy Search
vector<int> greedy(node* start, string goal){
    int idx = 0;
    vector<int> moves;
    vector<node*> nodes;
    nodes.push_back(start);
    while(true){
        node* current = nodes[idx++];
        if(current->state == goal){
            while(current->parent!=0){
                moves.push_back(current->action);
                current = current->parent;
            }
            return moves;
        }

        vector<node* > temp = expand(current);
        for(size_t i = 0; i < temp.size(); i++)
            if(temp[i]!=0 && temp[i]->action && !is_visited(temp[i]->state)){
                nodes.push_back(temp[i]);
                visited.push_back(temp[i]->state);
            }
        sort(nodes.begin()+idx, nodes.end(), heuristic_cmp);
    }
}

// A* Search
vector<int> astar(node* start, string goal){
    int idx = 0;
    vector<int> moves;
    vector<node*> nodes;
    nodes.push_back(start);
    while(true){
        node* current = nodes[idx++];
        if(current->state == goal){
            while(current->parent!=0){
                moves.push_back(current->action);
                current = current->parent;
            }
            return moves;
        }

        vector<node* > temp = expand(current, true);
        for(size_t i = 0; i < temp.size(); i++)
            if(temp[i]!=0 && temp[i]->action)
                nodes.push_back(temp[i]);

        sort(nodes.begin()+idx, nodes.end(), cost_heuristic_cmp);
    }
}

int main(){
    node* start;
    start = new node();
    start->state = "876102345";
    //start->state = "876140352";

    visited.push_back(start->state);

    //vector<int> Moves = bfs(start, goal); cout<<"Breadth First Search\n";
    //vector<int> Moves = dfs(start, goal, 0); cout<<"Depth First Search (Visited)\n";
    //vector<int> Moves = dfs(start, goal, 10); cout<<"Depth First Search (Limit)\n";
    vector<int> Moves = ucs(start, goal); cout<<"Uniform Cost Search\n";
    //vector<int> Moves = greedy(start, goal); cout<<"Greedy Search\n";
    //vector<int> Moves = astar(start, goal); cout<<"A* Search\n";
    int n = Moves.size();

    cout<<"Total Moves = "<<n<<endl;
    for(int i=n-1; i>=0; i--){
        switch(Moves[i]){
            case 1: cout<<n-i<<") up\n"; break;
            case 2: cout<<n-i<<") right\n"; break;
            case 3: cout<<n-i<<") down\n"; break;
            case 4: cout<<n-i<<") left\n"; break;
            default: break;
        }
    }
}
