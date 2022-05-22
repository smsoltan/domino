Behind the scenes: the project of classes

Two main groups of objects were distinguished:
material objects and events.

## Material objects
These are changeable during the course of time; they are in interaction with other objects mostly through events. Material objects include:

### Domino:
a single piece of domino.
- a and b, two constant numbers.
- a printing method
- useful operators, like:
-- == that is normal equality, immune to reversing.
-- || that checks, whether two dominos matches, or a domino matches an integer (one end)

### Dominos:
by nature, dominos are groupped. All such groups should contain:
- a list of dominos
- a printing method (well, not all)
- some functions regarding basic states of the group, ie: if_empty, size..

### Hand : public Dominos
where the word 'public' indicates only, that hands of players should be
visible. Of course, for only one player a hand should be fully aviable.
- searching methods, since all pieces in hand participate in a game.

### Table : public Dominos
where the pieces are put in a line
- methods of managing two ends of a line

### Stack : private Dominos
where private indicates that dominos should definitely not be seen.
- constructor of stack should create all dominos in the game, that later will wander from it to players, from players to table.
- a random drawing method.

## Events
Let's leave materials for a while and jump to events. But before that, let's
underline two different types of pointers that may appear.

- classical pointer, or a pointer for short, should point on an object (which would be ussually a piece of domino) whenever it is.
- a pointer called iterator, that would be used for finding what piece where the piece is at the moment in a given group.

### Event:
An abstract class. Event is a one in a lifetime object; it creates itself (takes place) wholy in the constructor, and then exists only archivised. As opposed to material objects, all events may have an easily constructed parent class.
- pointer to an avatar (see below) that triggered it; I assume that all events are triggered by avatars.
- a printing method, so that archives could be written and seen. All freshly created events should print themselves on a screen.

To name some basic events:

### Join: Event
A player joins the game.
- a constructor, that will receive an introduction of a player and add him to a list of players. An argument: pointer to the main object - the Game (see below).

### Set_Stack: Event
Well, this event triggers the self-constructing of the stack. The first player is remembered as the one who did it only because of courtesy towards him.
- A constructor that does that.

### Tie: Event
An exceptional event that marks a tie. No player triggeres it (null pointer), but the main object - Game.

Before introducing two fundamental events for a game, a new abstract class should be introduced:

### Action:
Everything that happened or MAY HAPPEN with a piece of domino. So:
- a pointer to a piece of domino.

### Draw: Action, Event
may need special privileges from stack to be performed
An event of taking a random piece from stack and moving it to hand.
- a constructor needs pointers to a hand and to a stack. Remembered piece would be taken piece

### Move: Action
still abstract class.
- a variable indicating, where the piece is wanted to be put on table.

### Move_planned: Move
not an abstract class, yet it describes only what happens in minds of avatars.
ONLY EXECUTABLE MOVES SHOULD BE POSSIBLE TO BE CONSTRUCTED!
- this leads to such a method of constructing, that does not allocate memory when the move is executable. To enter details: an extern function would check if everything's correct, and then call the constructor; otherwise returns null pointer. Constructor may be in private scope. Needs pointers to hand and table, and a piece that is wanted to be moved and position indicator (of putting the said piece).
- an iterator pointing to the same piece as the pointer (since the piece is still in hand) may be convenient for realisation of a move.

### Move_real: Move, Event
//may need special priviledges from planned move and table to be performed
Well, let's just take a planned move and make it real. Moves pieces from hand to table.
- a constructor: needs pointers to avatar, table and hand, and a planned move.

To be honest, the whole construction of planned moves and real moves was created while thinking of more advanced ai that was required. Oh well.

That concludes events. 

## Materials cont.

### Avatar:
Makes all the physical actions the player should do. Priviledges it grants
should be reduced to minimum.
- a hand; one is enough.
- pointer to the main object, the game.
- a MIND, that is, a pointer to player (see below)
- method of checking the possibility of moving without triggering mind - in case the mind is a fake and cheats.
- method of doing something; if can move, ask the mind for a planned move and make a real; if not, return draw.

### Player:
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

### Game:
//may need priviledges from avatar
The Main Object.
- list of players
- history - the list of archivised events.
- pointers to table and stack
- methods that enables seeing by other players all the pieces without the danger of cheating.
- a meta-event / method turn, that controls what is happening now and manages the order.
- constructor. Let the whole game takes place here; the game is a some kind of event after all.
- a printing method, for printing history.

