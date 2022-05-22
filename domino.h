#ifndef _DOMINO_H_
#define _DOMINO_H_
#include <list>
#include <string>
#include <iostream>

/*HOW ONE PIECE LOOKS:
  [a|b] or
  [false|true]
*/

extern void clearing(bool error = true);

class Game;

class Domino
{
   int a, b;//[a|b] - that is how one piece looks
public:
   int A() {return a;}
   int B() {return b;}
   int C(bool what) {return what ? a : b; }//returns a number; here piece looks like [true|false], which is convenient in the context.
   Domino(int a, int b) : a(a), b(b) { }
   void reverse() {int c = a; a = b; b = c;}
   bool operator==(Domino that);
   bool operator||(Domino that); //true if pieces can be matched (for checking, if human player could move)
   bool operator||(int c); //true if either a or b is equal to c   
};
extern std::ostream& operator<<(std::ostream& output, Domino input);


class Dominos
{
public:
   void print(std::ostream &output);
   std::list<Domino> pieces;
   bool empty() {return pieces.empty();}
   int size() {return pieces.size();}
};

class DS_stack : private Dominos
{
friend class Draw;
public:
   decltype(pieces.begin()) lot(); //random piece from stack
   DS_stack();
   Dominos::empty;
};

class DS_in_hand : public Dominos
{
public:
   decltype(pieces.begin()) search(Domino piece);
   bool check(Domino piece);
};
extern std::ostream& operator<<(std::ostream& output, DS_in_hand &input);
class DS_table : private Dominos
{
friend class Move_real;
public:
   Domino fit(); //domino with ends like ends of table(for checking, if human player could move)
   int fit(bool which_end);
   Dominos::size;
   friend std::ostream& operator<<(std::ostream& output, DS_table &input);
};

class Avatar;

/**********EVENTS******AND******ACTIONS***********************************/

class Event
{
protected:
   Avatar *trigger;
public:
   virtual void print(std::ostream &output) {}
};
extern std::ostream& operator<<(std::ostream& output, Event &input);

class Tie : public Event
{
public:
   Tie() {std::cout << *this;}
   void print(std::ostream &output) {output << "Nobody can move, the stack is empty. A tie.\n";}
};


class Join : public Event
{
public:
   Join(int n, Game *party);
   void print(std::ostream &output);
};

class Set_Stack : public Event
{
public:
   Set_Stack(Game *party);
   void print(std::ostream &output);
};

class Action
{
protected:
   Domino *piece;
   Action() {}
   Action(Domino *piece) : piece(piece) { }
};

class Draw : public Action, public Event
//friend with stack
{
public:
   Draw(Avatar *who, DS_stack *stack, DS_in_hand *hand);
   void print(std::ostream &output);
};

class Move : public Action
{
public:
   const bool where; //false [a|b][c|d]...[e|f] true
   Move(bool where, Domino *piece) : Action(piece), where(where) { }
};

class Move_planned : public Move
{
friend class Move_real;
public: const int free_end;
private:
   std::list<Domino>::iterator what;
   Move_planned(std::list<Domino>::iterator what, int free, bool where) :
      Move(where, &*what), free_end(free), what(what) { }
public:
   friend Move_planned *executable
      (std::list<Domino>::iterator what, DS_table *table, bool where);
   friend Move_planned *executable
      (Domino piece, DS_in_hand *hand, DS_table *table, bool where);
};
extern Move_planned pointer2object(Move_planned *pointer);

class Move_real : public Move, public Event
//friend with table and planned move
{
   int pieces_left;
public:
   Move_real(Move_planned plan, Avatar *who, DS_in_hand *hand, DS_table *table);
   void print(std::ostream &output);
};

/****************AVATAR******AND********PLAYERS************************/

class Player
{
public:
   virtual void accept_pass(const std::string name) { }
   virtual Move_planned plan_move(Avatar *trigger, DS_in_hand *hand, Game *party) = 0;
};

class Human : public Player
{
public:
   void accept_pass(const std::string name);
   Move_planned plan_move(Avatar *trigger, DS_in_hand *hand, Game *party);
};

class Easy_AI : public Player
{
public:
   Move_planned plan_move(Avatar *trigger, DS_in_hand *hand, Game *party);
};

class Hard_AI : public Player
{
   Avatar *next;
   void search_next(Avatar *trigger, Game *party);
public:
   Hard_AI() : next(NULL) { }
   Move_planned plan_move(Avatar *trigger, DS_in_hand *hand, Game *party);
};

class Avatar
{
   friend class Game;
   Player *mind;
   Game *party;
   DS_in_hand *hand;
public:
   bool can_move();
   const std::string name;
   Avatar(std::string name, Player *mind, Game *party) :
      mind(mind), party(party), name(name) {hand = new DS_in_hand;}
   ~Avatar() {delete mind; delete hand;}
   Event *do_something();
   Draw *fill_hand();

};

/*************************GAME*****************************************/
class Game
{
   unsigned int load_number();
   void load_players(unsigned int n);
   bool turn(int n); //false if final; n is number of players
public:
   DS_stack *stack;
   DS_table *table;
   std::list<Event *> history;
   std::list<Avatar> crowd;
   void hands_up(Avatar *except, std::ostream& output = std::cout);
   bool search_fit(Avatar* his, int number);
   Game();
   void print(std::ostream &output);
   ~Game();
};

#endif
