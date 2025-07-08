// MESSAGE HANDLER

function messageHandler(message) {
  if (message.id === 0) {
    console.log(message.log);
  }
  else if (message.id === 1) {
    renderPosition(message.position);
  }
}

// INIT SOCKET

let ws;
function initSocket() {
  ws = new WebSocket("ws://localhost:8123");

  ws.onopen = () => {
    console.log("ws open");
  }

  ws.onmessage = (event) => {
    console.log(event.data);
    messageHandler(JSON.parse(event.data));
  }

  ws.onerror = (err) => {
    console.log(err);
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

const imageNames = ["", "bQ", "bP", "bR", "bN", "bB", "bK", "", "", "wQ", "wP", "wR", "wN", "wB", "wK"];

const darkSquareStyles = "square bg-green-700";
const lightSquareStyles = "square bg-green-200";

function renderBoard() {
  for (let i = 0; i < 8; i++) {
    for (let j = 0; j < 8; j++) {
      const div = document.createElement("div");

      if ((i + j) % 2) { div.className = darkSquareStyles; }
      else { div.className = lightSquareStyles; }

      board.appendChild(div);
    }
  }
}

renderBoard();

function renderPosition(position) {
  let squareNum = 0;
  for (let i = 0; i < position.length; i++) {
    const piece = position[i];
    square = board.children[squareNum];

    //if square is out of bound
    if (piece === 0x20) { continue; } 

    square.innerHTML = "";

    //if square is not empty
    if (piece !== 0x10) {
      const img = document.createElement("img");
      img.src = `img/${imageNames[piece]}.svg`;
      img.alt = piece;
      square.appendChild(img);
    }

    squareNum++;
  }
}

//fen input

const textbox = document.getElementById("fen_input");
textbox.addEventListener("keydown", (event) => {
  if(event.key === "Enter") {
    const message = { id: 2, fen: textbox.value };
    ws.send(JSON.stringify(message));
  }
})