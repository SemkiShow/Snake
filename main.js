/*// Init
var canvas = document.getElementById('canvas');
canvas.width = screen.width;
canvas.height = screen.height;
var ctx = canvas.getContext('2d');
const boardSize = 50;
const pixelSize = canvas.width / boardSize;
let board = [];
for (let i = 0; i < pixelSize; i++)
{
  board[i] = [];  
}
for (let i = 0; i < boardSize; i++) 
{
  for (let j = 0; j < boardSize; j++) 
  {
    board[i][j] = " "; 
  }
}
board[0][0] = "S";
let direction = "RIGHT";

function DrawBoard()
{
  // Clear board
  ctx.fillStyle = "Black";
  ctx.fillRect(0, 0, canvas.width, canvas.height);

  // Draw snake&apples
  for (let i = 0; i < array.length; i++) 
  {
    for (let j = 0; j < array.length; j++) 
    {
      if (board[i][j] == "S") 
      {
        ctx.fillStyle = "Green";
        ctx.fillRect(i * pixelSize, j * pixelSize, i * pixelSize + pixelSize, j * pixelSize + pixelSize);
      }
      
      if (board[i][j] == "A") 
      {
        ctx.fillStyle = "Red";
        ctx.fillRect(i * pixelSize, j * pixelSize, i * pixelSize + pixelSize, j * pixelSize + pixelSize);
      }
    }  
  }
}

function ResetGame()
{
  for (let i = 0; i < boardSize; i++) 
  {
    for (let j = 0; j < boardSize; j++) 
   {
     board[i][j] = " "; 
   }
  }
  board[0][0] = "S";

  DrawBoard();
}

function Main()
{
  ResetGame();
}

Main();
*/
