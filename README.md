# [D|o][M|i][N|o][!|!]
*by Stanisław Sołtan*

A game of dominoes. 1 to 5 players (human or AI). Old course project in object-oriented programming, used now to learn GitHub.

## Compilation (on Linux using terminal):

The following files are neccessary for the compilation proces:
- main.cpp
- domino.h
- domino.cpp
- Makefile

To create an executable file under the name "domino", enter a "make" command.
Enter "make clean" to delete all produced files.

## Rules:

See the text of the task for details, on: https://www.fuw.edu.pl/~werner/pz/files/kol2.pdf [text in Polish]. An
extra rule was introduced, that the number of players cannot exceed 5.

## Usage:

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

