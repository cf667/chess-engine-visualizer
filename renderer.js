// MESSAGE HANDLERS

//input

function inputMessageHandler(message) {
  if (message.id === 1) {
    renderPosition(message.position);
  }
}

//send fen (ID 2)

function sendFen(message) {
  const jsonMsg = { id: 2, fen: message };
  ws.send(JSON.stringify(jsonMsg));
}

//send move (ID 3)

let originSquare = 0;
let destinationSquare = 0;
function sendMove() {
  const jsonMsg = { id: 3, origin: parseInt(originSquare), destination: parseInt(destinationSquare) };
  ws.send(JSON.stringify(jsonMsg));
}

// INIT SOCKET

let ws;
function initSocket() {
  ws = new WebSocket("ws://localhost:8123");

  ws.onopen = () => {
    console.log("ws open");
  }

  ws.onmessage = (event) => {
    console.log("message received:");
    console.log(event.data);
    inputMessageHandler(JSON.parse(event.data));
  }

  ws.onerror = (err) => {
    console.log(err);
    //retry
    setTimeout(initSocket, 1000);
  }
}

initSocket();

// RENDERING

//board

const board = document.getElementById("chessboard");

const startPosition =
[ 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
  0x20, 0x3,  0x4,  0x5,  0x1,  0x6,  0x5,  0x4,  0x3,  0x20, 
  0x20, 0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x20, 
  0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 
  0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 
  0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 
  0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 
  0x20, 0xA,  0xA,  0xA,  0xA,  0xA,  0xA,  0xA,  0xA,  0x20, 
  0x20, 0xB,  0xC,  0xD,  0x9,  0xE,  0xD,  0xC,  0xB,  0x20, 
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20  ]; //starting position

//file names of each piece image
const imageNames = ["", "bQ", "bP", "bR", "bN", "bB", "bK", "", "", "wQ", "wP", "wR", "wN", "wB", "wK"];

const darkSquareStyles = "square bg-green-700";
const lightSquareStyles = "square bg-green-200";

function renderBoard() {
  let currentSquareIndex = 0; // = list index in 10x12 representation
  for (let i = 0; i < 8; i++) {
    for (let j = 0; j < 8; j++) {
      const div = document.createElement("div");

      if ((i + j) % 2) { div.className = darkSquareStyles; }
      else { div.className = lightSquareStyles; }
      
      while (startPosition[currentSquareIndex] == 0x20) { //skip over every out of bound square
        currentSquareIndex++;
      }
      div.dataset.squareIndex = currentSquareIndex;
      currentSquareIndex++;

      board.appendChild(div);
    }
  }
}

renderBoard();

let draggedPiece = 0;

function renderPosition(position) {
  let squareNum = 0; // = index of current square on board

  //clear board
  document.querySelectorAll(".piece").forEach((element) => element.remove());

  for (let i = 0; i < position.length; i++) {
    const piece = position[i];

    //if square is out of bound
    if (piece === 0x20) { continue; } 

    //if square is not empty
    if (piece !== 0x10) {
      const img = document.createElement("img");
      img.src = `img/${imageNames[piece]}.svg`;
      img.alt = piece;
      img.style.userSelect = "none";
      img.className = "piece"; // no styles, just for finding every piece
      img.draggable = false; // own dragging implemented
      
      //dragging
      img.addEventListener("mousedown", (event) => {
        draggedPiece = event.target;
        originSquare = event.target.parentElement.dataset.squareIndex;

        //make piece position absolute but keep size
        const originalImageRect = event.target.getBoundingClientRect();
        event.target.style.position = "absolute";
        event.target.style.width = originalImageRect.width + "px";
        event.target.style.height = originalImageRect.height + "px";
        
        //disable events for every piece (see global mouseup event listener)
        document.querySelectorAll(".piece").forEach((element) => {
          element.style.pointerEvents = "none";
        });
      });
      board.children[squareNum].appendChild(img);
    }

    squareNum++; //only if square is not out of bound
  }
}

renderPosition(startPosition);

//mouse input

document.addEventListener("mousemove", (event) => {
  if (draggedPiece) {
    //dragged piece follows cursor
    const imgRect = draggedPiece.getBoundingClientRect();
    draggedPiece.style.left = event.pageX - imgRect.width / 2 + "px";
    draggedPiece.style.top = event.pageY - imgRect.height / 2 + "px";
  }
});

document.addEventListener("mouseup", (event) => {
  if (draggedPiece) {
    // even if the draggedPiece is dropped on another piece, 
    // event.target will still be the square below the piece 
    // because all events are disabled for every piece when a piece is dragged
    if (event.target.dataset.squareIndex) {
      destinationSquare = event.target.dataset.squareIndex;
      sendMove();
    }

    //reactivate all events for pieces
    document.querySelectorAll(".piece").forEach((element) => {
      element.style.pointerEvents = "auto";
    });
    draggedPiece = 0;
    originSquare = 0;
  }
});

//fen input

const fenTextbox = document.getElementById("fen_input");
fenTextbox.addEventListener("keydown", (event) => {
  if(event.key === "Enter") {
    sendFen(fenTextbox.value);
  }
});