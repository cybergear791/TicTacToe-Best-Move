# Desc
The minmax algorithm was used to find the optimal move for the AI. Letting the AI `O` be the Maximizer Player, and the user `X` be the Minimizer, board states that are a win for the AI returned the value `100 - depth`, and returned `-100 + depth` for winning board states for the User. Facorting the depth of the board state into the minmax value is so that the AI chooses a move that will lead to a sooner win.

Alpha-Beta Pruning was incorporated to reduce the number of subtrees that are evaluated by the minmax function, by not exploring subtrees whose board moves will not be chosen as the optimal move, for their minmax value could not be better than a previous explored subtrees.

# Install
- Install node.js from https://nodejs.org
- In the root folder of this project enter ```npm install``` to install the dependencies.
- Run ```npm start```
- Open http://localhost:3000 on your web browser to play.

# Game
You are the `X` and the AI is the `O`.

The AI always plays the optimal move, so it is impossible for `X` to win. <br/>
Only Ties and Wins for the AI occur.

![](https://github.com/cybergear791/TicTacToe-Best-Move/blob/master/src/images/Game_Screenshot.png)
