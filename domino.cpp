#include "domino.h"
#include <stdlib.h>
#include <limits>
#include <iomanip>
#include <unistd.h>

void clearing(bool error)
{
   std::cin.clear();
   char sign;
   std::cin.get(sign);
   if (sign != '\n')
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   if (error) std::cout << "incorrect input, please try again: ";
}
/**************************DOMINO************************************/
bool Domino::operator||(int c)
   {return (a==c)||(b==c)||(a==7)||(b==7)||(c==7);}
bool Domino::operator||(Domino that) {return (that||a)||(that||b);}
bool Domino::operator==(Domino that)
   {return ((a!=b)&&(that||a)&&(that||b))||((a==b)&&(that.A()==that.B()));}
std::ostream& operator<<(std::ostream& output, Domino input)
{
   output << "[" << input.A() << "|" << input.B() << "]";
   return output;
}
/*************************DOMINOS************************************/
void Dominos::print(std::ostream &output)
{
   if (!empty()) for(auto it = pieces.begin(); it != pieces.end(); ++it)
      output << *it;
   output << "\n";
};
/************************DS_STACK************************************/
decltype(DS_stack::pieces.begin()) DS_stack::lot()
{
   if (pieces.empty()) return pieces.end();
   int n = rand() % pieces.size() + 1;
   auto it = pieces.end();
   for(int i = 0; i < n; ++i) --it;
   return it;
}
DS_stack::DS_stack()
{
   for(int a = 0; a < 7; a++)
      for(int b = a; b < 7; b++)
         pieces.push_back(Domino(a, b));
   srandom(time(NULL));
}
/************************DS_IN_HAND*********************************/
decltype(DS_in_hand::pieces.begin()) DS_in_hand::search(Domino piece)
{
   decltype(DS_in_hand::pieces.begin()) it;
   for (it = pieces.begin(); (it != pieces.end())&&!(*it == piece); ++it);
   return it;
}

bool DS_in_hand::check(Domino piece)
{
   for (auto it = pieces.begin(); it != pieces.end(); ++it)
      if (*it == piece) return true;
   return false;
}

std::ostream& operator<<(std::ostream& output, DS_in_hand &input)
{
   output << "Pieces in posesion:\n";
   input.print(output);
   return output;
}

/************************DS_TABLE***********************************/
Domino DS_table::fit()
{
   if (empty()) return Domino(7, 7);
   return Domino(pieces.begin()->A(), pieces.rbegin()->B());
}
int DS_table::fit(bool which_end)
{
   if (pieces.empty()) return 7;
   return which_end ? pieces.rbegin()->B() : pieces.begin()->A();
}
std::ostream& operator<<(std::ostream& output, DS_table &input)
{
   output << "Pieces on table:\n";
   input.print(output);
   return output;
}
/**************************EVENT***********************************/
std::ostream& operator<<(std::ostream& output, Event &input)
{
   input.print(output);
   return output;
};

/***************************JOIN***********************************/
//Currently, only humans and stupid ai considered
Join::Join(int n, Game *party)
{
   std::cout << "Player" << std::setw(3) << n << ": ";
   std::string name, species;
   Player *brain;
   for(bool i = true; i==true;)
   {
      std::cin >> species >> name;
      i=false;
      if(species == "human") brain = new Human();
      else if(species == "easy") brain = new Easy_AI();
      else if(species == "hard") brain = new Hard_AI();
      else i = true;
      clearing(i);
   }
   trigger = new Avatar(name, brain, party);
   (party->crowd).push_back(*trigger);
   std::cout << *this;
}
void Join::print(std::ostream &output)
{
  output << trigger->name << " joins the game.\n";
}

/************************SET_STACK*****************************/
Set_Stack::Set_Stack(Game *party)
{
   party->stack = new DS_stack();
   trigger = &*((party->crowd).begin());
   std::cout << *this;
}
void Set_Stack::print(std::ostream &output)
{
  output << trigger->name << " prepares the stack.\n";
}

/***************************DRAW*******************************/
Draw::Draw(Avatar *who, DS_stack *stack, DS_in_hand *hand)
{
   trigger = who;
   if (stack->empty()) piece = NULL;
   else
   {
      auto bone = stack->lot();
      piece = &*bone;
      hand->pieces.splice
         (hand->pieces.begin(), stack->pieces, bone);
   }
   std::cout << *this;
}
void Draw::print(std::ostream &output)
{
  output << trigger->name;
  if (piece == NULL) output << " draws no dominos; the stack is empty.\n";
  else output << " draws from stack: " << *piece << ".\n";
}

/***********************MOVE_PLANNED**************************/
Move_planned *executable
   (std::list<Domino>::iterator what, DS_table *table, bool where)
{
   int c = table->fit(where);
   if (!(*what || c)) return NULL;
   int free = (what->A()==table->fit(where)) ? what-> B(): what->A();
   return new Move_planned(what, free, where);
}

Move_planned *executable
   (Domino piece, DS_in_hand *hand, DS_table *table, bool where)
{
   if (!hand->check(piece)) return NULL;
   auto it = hand->search(piece);
   return executable(it, table, where);
}

/************************MOVE_REAL***************************/
Move_real::Move_real(Move_planned plan, Avatar *who, DS_in_hand *hand, DS_table *table) :
   Move(plan.where, plan.piece), pieces_left((hand->size())-1)
{
   trigger = who;
   auto put = where ? table->pieces.end() : table->pieces.begin();
   if (plan.free_end==piece->C(where)) piece->reverse();
   table->pieces.splice(put, hand->pieces, plan.what);
   std::cout << *this;
}
void Move_real::print(std::ostream &output)
{
  char c = where ? 'b' : 'a';
  output << trigger->name << " puts " << *piece << " at the end "
         << c << " of line. " << pieces_left << " left";
  switch(pieces_left)
  {
     case 0: output << ", so that's a win!\n";
             break;
     case 1: output << ". Domino"; //break and final dot below
     default: output << ".\n";
  }
}

Move_planned pointer2object(Move_planned *pointer)
{
   Move_planned ret = *pointer;
   delete pointer;
   return ret;
}

/**************************HUMAN******************************/
void Human::accept_pass(const std::string name)
{
   std::cout << "->" << name << ", you have no piece to put on table. Please accept it.\n";
   clearing(false);
}

Move_planned Human::plan_move(Avatar *trigger, DS_in_hand *hand, Game *party)
{
   std::cout << "*******" << *(party->table) << trigger->name << ", your turn. " << *hand;
   party->hands_up(trigger);
   clearing(false);
   std::cout << "->Please enter a position and a piece (see README.txt for details): ";  
   for(;;)
   {
      char a, b, c;
      std::cin >> c >> a >> b;
      int A = a - '0', B = b - '0'; //now we have digits
      if((c == 'a' || c == 'b')
        && (A > -1) && (A < 8) && (B > -1) && (B < 8))
      {
         bool position = (c == 'a') ? false : true;
         Move_planned *ret = executable
            (Domino(A, B), hand, party->table, position);
         if (ret!=NULL) return pointer2object(ret);
      }
      clearing();
   }
}

/**********************EASY***AI****************************/
Move_planned Easy_AI::plan_move(Avatar *trigger, DS_in_hand *hand, Game *party)
{
   Move_planned *ret = NULL;
   for(auto it = hand->pieces.begin(); it != hand->pieces.end(); ++it)
      for(int i = 0; i < 3; ++i)
      {
         Move_planned *plan = executable(it, party->table, i);
         if (plan != NULL)
         {
            ret = plan;
            break;
         }
      }
   return pointer2object(ret);
}

/***********************HARD_AI*****************************/
void Hard_AI::search_next(Avatar *trigger, Game *party)
{
   auto it = party->crowd.begin();
   for(; &*it!=trigger; ++it);
   ++it;
   if(it == party->crowd.end()) it = party->crowd.begin();
   next = &*it;
}

Move_planned Hard_AI::plan_move(Avatar *trigger, DS_in_hand *hand, Game *party)
{
   if(next == NULL) search_next(trigger, party);
   Move_planned *cunning_plan = NULL;
   for(auto it = hand->pieces.begin(); it != hand->pieces.end(); ++it)
   for(int i = 0; i < 3; ++i)
      {
         Move_planned *plan = executable(it, party->table, i);
         if (plan != NULL)
         {
            if(!party->search_fit(next, plan->free_end))
            {
               cunning_plan = plan;
               break;
            }
            if(cunning_plan == NULL) cunning_plan = plan;
            else delete plan;
         }
      }
   return pointer2object(cunning_plan);
}

/**********************AVATAR*******************************/
bool Avatar::can_move()
{
   for(auto it = hand->pieces.begin(); it != hand->pieces.end(); ++it)
      if (*it || party->table->fit()) return true;
   return false;
}

Event *Avatar::do_something()
{
   if (can_move())
   {
      Move_planned plan = mind->plan_move(this, hand, party);
      return new Move_real(plan, this, hand, party->table);
   }
   mind->accept_pass(name);
   return new Draw(this, party->stack, hand);
}

Draw *Avatar::fill_hand()
{
   return new Draw(this, party->stack, hand);
}

/****************************GAME*****************************/
unsigned int Game::load_number()
{
   std::cout << "Please give number of players: ";
   unsigned int n;
   while((!(std::cin >> n))||(n>5)) clearing();
   return n;
}

void Game::load_players(unsigned int n)
{
   for(unsigned int i = 0; i<n; ++i)
   {
      history.push_back(new Join(i+1, this));
      if (stack == NULL) history.push_back(new Set_Stack(this));
      history.push_back(crowd.rbegin()->fill_hand());
   }
   for(int i = 0; i < 4; ++i)
      for(auto it = crowd.begin(); it != crowd.end(); ++it)
         history.push_back(it->fill_hand());
}

bool Game::turn(int n)
{
   static auto it = crowd.begin();
   if(it==crowd.end()) it = crowd.begin();
   int s = table->size();
   static int nothing_happened = 0;
   history.push_back(it->do_something());
   if (it->hand->empty()) return false;
   if(stack->empty() && s==table->size()) ++nothing_happened;
   else nothing_happened = 0;
   if (nothing_happened==n)
   {
      history.push_back(new Tie());
      return false;
   }
   ++it;
   return true;
}

Game::Game() : stack(NULL)
{
   int n = load_number();
   load_players(n);
   table = new DS_table;
   while(turn(n));
   
}

void Game::print(std::ostream &output)
{
   for(auto it = history.begin(); it != history.end(); ++it)
      output << **it;
   output << *table;
   hands_up(NULL, output);
}

void Game::hands_up(Avatar *except, std::ostream& output)
{
   for(auto it = crowd.begin(); it != crowd.end(); ++it)
      if ((&*it)!=except) output << it->name << " - " << *(it->hand);
}

bool Game::search_fit(Avatar* him, int number)
{
   for(auto it = him->hand->pieces.begin();
            it != him->hand->pieces.end(); ++it)
      if ((*it) || number) return true;
   return false;
}

Game::~Game()
{
   delete stack;
   delete table;
}
