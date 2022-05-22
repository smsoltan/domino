# domino
A game of dominoes. Old course project in object-oriented programming, used now to learn GitHub. Original readme below:

[D|o][M|i][N|o][!|!] by Stanisław Sołtan

General description:

A game of dominos. 1 to 5 players (human or AI). Detailed description of
implemented classes at the end of this file.

Compilation:

The following files are neccessary for the compilation proces:
main.cpp
domino.h
domino.cpp
Makefile

To create an executable file under the name "domino", enter a "make" command.
Enter "make clean" to delete all produced files.

Rules:

See the text of the task for details, on: http://www.fuw.edu.pl/~werner/pz/. An
extra rule was introduced, that the number of players cannot be bigger than 5.

Usage:

When you start the game, it would first ask You for the number of players; then
You would be asked to introduce all players. Such introduction consists of two
words:

- first indicates the type of player: could be 'human' for human players, or
  'easy' or 'hard' for two levels of artificial inteligence. Note that there
could be only computer players.

- second is the name. This is the only information about the player, that will
  be written in the history of the game.

While the players are introduced, the game will be informing, what is
happening: that the stack is set, and players draws pieces. After the last
player joined the game, the first player's turn begin, then the second and so
on.

On human's turn, a general description of the situation will be printed and a
human in question is required to enter his move - if he had any. Assuming
he/she can move, he/she should first enter a letter indicating position on
table (a for left end, b for right), and then two digits indicating the piece
he has that would match table in this position. Note that the program could
compute Your input only after he had asked for it; if there is no explicit ask
and the game paused, pressing enter should proceed the game.

If the human cannot move, his avatar would automatically draw a piece, and a
player would be asked to accept this fact in silence. The game would also
proceed through turns of computer players without waiting for any human's
response.

The tie happens, if all the players would not move one after another while the
stack is empty.

If the game ends with a win or a tie (there is no other possibility, to be
honest), the player or anybody in charge may decide to save the history of the
game for the future, in the form of a text file; in that case he can enter a
name of save file when asked.  If the player would not enter a name and press
enter instead, the game would not be saved.

That's all, wish You good game!

Behind the scenes: the project of classes

After short consideration, two main groups of objects were distinguished:
material objects and events.

-> Material objects are changeable during the course of time; they are in interaction with other objects mostly through events. Material objects include:

Domino:
a single piece of domino.
- a and b, two constant numbers.
- a printing method
- useful operators, like:
--== that is normal equality, immune to reversing.
--|| that checks, whether two dominos matches, or a domino matches an integer
(one end)

Dominos:
by nature, dominos are groupped. All such groups should contain:
- a list of dominos
- a printing method (well, not all)
- some functions regarding basic states of the group, ie: if_empty, size..

Hand : public Dominos
where the word 'public' indicates only, that hands of players should be
visible. Of course, for only one player a hand should be fully aviable.
- searching methods, since all pieces in hand participate in a game.

Table : public Dominos
where the pieces are put in a line
- methods of managing two ends of a line

Stack : private Dominos

Where private indicates, that dominos should definitely not be seen.
- constructor of stack should create all dominos in the game, that later will
  wander from it to players, from players to table.
- a lotting method.

Let's leave materials for a while and jump to events. But before that, let's
underline two different types of pointers that may appear.

* classical pointer, or a pointer for short, should point on an object (which
* would be ussually a piece of domino) whenever it is.

* a pointer called iterator, that would be used for finding what piece where
* the piece is at the moment in a given group.

- Event is a one in a lifetime object; it creates itself (takes place) wholy in the constructor, and then exists only archivised. As opposed to material objects, all events may have an easily constructed parent class.

Event:
An abstract class
- pointer to an avatar (see below) that triggered it; I assume that all events
  are triggered by avatars.
- a printing method, so that archives could be written and seen. All freshly
  created events should print themselves on a screen.

To name some basic events:

Join: Event
A player joins the game.
- a constructor, that will receive an introduction of a player and add him to a
  list of players. An argument: pointer to the main object - the Game (see
below).

Set_Stack: Event
Well, this event triggers the self-constructing of the stack. The
first player is remembered as the one who didi this only because of courtesy
towards him.
- A constructor that does that.

Tie: Event
An exceptional event that marks a tie. No player triggeres it (null
pointer), but the main object - Game.

Before introducing two fundamental events for a game, a new abstract class
should be introduced:

Action:
Everything that happened or MAY HAPPEN with a piece of domino. So:
- a pointer to a piece of domino.

Draw: Action, Event
//may need special privileges from stack to be performed
An event of taking a random piece from stack and moving it to hand.
- a constructor needs pointers to a hand and to a stack. Remembered piece would
  be taken piece

Move: Action
still abstract class.
- a variable indicating, where the piece is wanted to be put on table.

Move_planned: Move
not an abstract class, yet it describes only what happens in minds of avatars.
ONLY EXECUTABLE MOVES SHOULD BE POSSIBLE TO BE CONSTRUCTED!
- this leads to such a method of constructing, that does not allocate memory
  when the move is executable. To enter details: an extern function would check
if everything's correct, and then call the constructor; otherwise returns null
pointer. Constructor may be in private scope. Needs pointers to hand and table,
and a piece that is wanted to be moved and position indicator (of putting).
- an iterator pointing to the same piece as the pointer (since the piece is
  still in hand )may be convenient for realisation of a move.

Move_real: Move, Event
//may need special priviledges from planned move and table to be performed
Well, let's just take a planned move and make it real. Moves pieces from hand
to table.
- a constructor: needs pointers to avatar, table and hand, and a planned move.

To be honest, the whole construction of planned moves and real moves was
created while thinking of more advanced ai that is neccessary. Oh well.

That concludes events. 

Avatar:
Makes all the physical actions the player should do. Priviledges it grants
should be reduced to minimum.
- a hand; one is enough.
- pointer to the main object, the game.
- a MIND, that is, a pointer to player (see below)
- method of checking the possibility of moving without riggering mind - in case
  the mind is a fake and cheats.
- method of doing something; if can move, ask the mind for a planned move and
  make a real; if not, return draw.

Player:
Abstract class.
- method of planning move.
- method of accepting pass - only humans needs it, AI doesn't bother.

From the Player we have:
Human
Easy Ai
Hard Ai

Any extra variables these classes possess may be useful for creating
strategies.for example, it is convenient for hard ai to have the pointer to a
next player, whom it hates.

Game:
//may need priviledges from avatar
The Main Object.
- list of players
- history - the list of archivised events.
- pointers to table and stack
- methods that enables seeing by other players all the pieces without the
  danger of cheating.
- a meta-event / method turn, that controls what is happening now and manages
  the order.
- constructor. Let the whole game takes place here; the game is a some kind of
  event after all.
- a printing method, for printing history.
