
import React from 'react';
let uniqueSeed = 0;
function nextUniqueKey() {
    return uniqueSeed += 1;
}

const NUM_ROWS = 3, NUM_COLUMNS = 3;

class Cell extends React.Component {

    render() {
        return (
            <td onClick={() => this.props.handleClick(this.props.colIdx, this.props.rowIdx)} 
                width="50px" height="50px">
                <img src={require("./images/" + this.props.cell.color + ".png")} alt="moves"/>
            </td>
        )
    }
}

class Row extends React.Component {

    render() {
        return (
            <tr>{this.props.row.map( (cell, idx) =>
                <Cell key={nextUniqueKey()} cell={cell}
                      handleClick={this.props.handleClick} colIdx={idx} rowIdx={this.props.indx} />) }</tr>
        )
    }
}

class Game extends React.Component {

    constructor(props) {
        super(props);

        let board = Array(NUM_ROWS).fill(Array(NUM_COLUMNS).fill({color: "w", isOccupied: false}));
        board = board.map((row, rowIdx) => row.map( (col, colIdx) => {
            return {...board[rowIdx][colIdx], row: rowIdx, column: colIdx }
        }));
        
        this.state = {
            board
        };
        this.move_counter = 0;
        this.handleClick = this.handleClick.bind(this);
        this.reset = this.reset.bind(this);
    }

    deepCopyFunction (inObject){
        if (typeof inObject !== "object" || inObject === null) {
            return inObject;
        }

        let value, key;
        let outObject = []
    
        for (key in inObject) {
            value = inObject[key];
            outObject[key] = this.deepCopyFunction(value);
        }
    
        return outObject;
    }

    // Returns an array filled with different boards with every possible move 'playersturn' can move.
    Successors(board, playersturn)
    {
        let successors = [];
        for (let i = 0; i < board.length; i++)
        {
            let temp = this.deepCopyFunction(board);
            if (temp[i].color === "w")  
            {
                temp[i].color = playersturn;
                successors.push(temp);
            }
        }
        return successors;
    }
    
    // Returns a value whether 'board' has a winner
    compare_winning(board, winningPlayer, depth, checkTie)
    {
        const winning_combos = [
        [0,1,2],
        [3,4,5],
        [6,7,8],
        [0,3,6],
        [1,4,7],
        [2,5,8],
        [0,4,8],
        [2,4,6]
        ];
        
        for (let i = 0; i < winning_combos.length; i++)
        {
            if (board[winning_combos[i][0]].color === "x" && board[winning_combos[i][1]].color === "x" && board[winning_combos[i][2]].color === "x")
            {
                winningPlayer.win = -100 + depth;
                return true;
            }
            if (board[winning_combos[i][0]].color === "ai" && board[winning_combos[i][1]].color === "ai" && board[winning_combos[i][2]].color === "ai")
            {
                winningPlayer.win = 100 - depth; // '- depth' to choose a move that will lead to a sooner win.
                return true;
            }
        }

        if(checkTie){
            for (let i = 0; i < board.length; i++)
                if (board[i].color === "w")
                    return false;
            winningPlayer.win = 0;
            return true;
        }
        return false;
    }

    Alpha_Beta(board, alpha, beta, depth, player, bestSuccessor)
    {
        // When reaches a leaf node.
        let winningPlayer = {};
        if (this.compare_winning(board, winningPlayer, depth, true)){
            //console.log(winningPlayer.win, depth);
            return winningPlayer.win;
        }

        let successors = this.Successors(board, player);

        if (player === "ai"){
            let result_value = Number.NEGATIVE_INFINITY;
            
            for (let i = 0; i < successors.length; i++){
                let value = this.Alpha_Beta(successors[i], alpha, result_value, depth + 1, "x", bestSuccessor);
                
                if (value > result_value){
                    result_value = value;
                    bestSuccessor.move = successors[i];
                }
                //alpha = Math.max(alpha, value);
                //if (beta <= alpha)
                  //  break;
            }
            return result_value;
        }
        else{
            let result_value = Number.POSITIVE_INFINITY;

            for (let i = 0; i < successors.length; i++){
                let value = this.Alpha_Beta(successors[i], result_value, beta, depth + 1, "ai", bestSuccessor);
                
                if (value < result_value){
                    result_value = value;
                    bestSuccessor.move = successors[i];
                }
                //beta = Math.min(beta, value);
                //if (beta <= alpha)
                  //  break;
            }
            return result_value;
        }
    }

    Print_BestMove(board, nextBoard)
    {
        for (let i = 0; i < board.length; i++)
        {
            if (nextBoard[i].color !== board[i].color)
                return i;
        }
        throw new Error("Board's are Identical.");
    }

    handleClick(colIdx, rowIdx) {
        if( this.state.haveAWinner )
            return;

        //if( rowIdx <  0)
          //  return;

        let newBoard = this.state.board.slice();

        // The user's click must be on an unoccupied space.
        if( this.state.board[rowIdx][colIdx].isOccupied === false )
        {
            this.move_counter += 1;
        
            newBoard[rowIdx][colIdx] = {color: "x", isOccupied: true}

            this.setState({
                board: newBoard
            });
            let winningPlayer = {};
            this.compare_winning(this.state.board.flat(), winningPlayer, 0, false)
            if( winningPlayer.win < 0 ) {
                this.setState(
                    {
                        haveAWinner: true,
                        winnerColor: "x"
                    });
            }

            // If game board isnt filled.
            else if(this.move_counter < 5) {
                // AI's Turn
                console.log("AI Thinking...");
                let bestMove = {};
                this.Alpha_Beta(newBoard.flat(), Number.NEGATIVE_INFINITY, Number.POSITIVE_INFINITY, 0, "ai", bestMove)
                
                let move = this.Print_BestMove(newBoard.flat(), bestMove.move)
                newBoard[Math.floor(move / 3)][move % 3] = {color: "ai", isOccupied: true};    

                this.setState({
                    board: newBoard
                });
                console.log("AI Moved");
                let winningPlayer = {};
                this.compare_winning(this.state.board.flat(), winningPlayer, 0, false)
                if( winningPlayer.win > 0) {
                    this.setState(
                        {
                            haveAWinner: true,
                            winnerColor: "ai"
                        });
                }
            }
        }
    }

    reset() {
            let board = Array(NUM_ROWS).fill(Array(NUM_COLUMNS).fill({color: "w", isOccupied: false}));
            board = board.map((row, rowIdx) => row.map( (col, colIdx) => {
                return {...board[rowIdx][colIdx], row: rowIdx, column: colIdx }
            }));
            this.move_counter = 0;
            this.setState({
                board,
                haveAWinner: false
            });
    }

    topMessage() {
        if( ! this.state.haveAWinner) {
            if(this.move_counter === 5) {
                return <div style={{height: "50px", textAlign: "center"}}>
                    <p align="center"> Tied. Game Over.
                            <br/>
                            <button onClick={this.reset}>Reset?
                            </button>
                    </p>
                    </div>
            }
            return <div style={{height: "50px", textAlign: "center"}}>
                <p top-margin="100px">Tic-Tac-Toe 3x3 <br /></p>
                </div>;
        }

        const winnerColor = this.state.winnerColor.charAt(0).toUpperCase() + this.state.winnerColor.slice(1);
        return <div style={{height: "50px", align: "center"}}>
                    <p align="center">{winnerColor} Wins. Game Over.
                        <br/>
                        <button onClick={this.reset}>Reset?
                        </button>
                    </p>
               </div>
    };
    
    render() {
        return (
            <div>
                {this.topMessage()}

            <table border="1" align="center">
                <tbody>
                {       
                    this.state.board.map((row, idx) =>
                            <Row key={nextUniqueKey()}
                                 handleClick={this.handleClick} row={row} indx={idx} />)
                }
                </tbody>
            </table>

            </div>
        );
    }
}

export default Game;
