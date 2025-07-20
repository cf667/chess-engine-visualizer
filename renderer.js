// MESSAGE HANDLERS

//input

function inputMessageHandler(message) {
  switch (message.id)
  {
    case 1:
      renderPosition(message.position);
      break;
    case 5:
      getNewNode(message.nodeId, message.depth, message.parentId, message.previousMove);
      break;
    case 6:
      getNodeScore(message.nodeId, message.score);
      break;
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

//send "make best move" command (ID 4)

function sendMakeBestMove() {
  const jsonMsg = { id: 4 };
  ws.send(JSON.stringify(jsonMsg));
}

//get node data (ID 5 / 6)

// let nodes = [
//   { id: 1, depth: 0, parentId: 0, x: 0, y: 0, previousMove: 0, score: "no score" },
//   { id: 2, depth: 1, parentId: 1, x: 0, y: 0, previousMove: "d2d3", score: "no score" },
//   { id: 3, depth: 1, parentId: 1, x: 0, y: 0, previousMove: "d2d4", score: "no score" },
//   { id: 4, depth: 2, parentId: 3, x: 0, y: 0, previousMove: "d4d5", score: "-4" },
//   { id: 5, depth: 2, parentId: 3, x: 0, y: 0, previousMove: "d4e4", score: "5" }
// ];
let nodes = [];
function getNewNode(nodeId, depth, parentId, previousMove) {
  nodes.push({id: nodeId, depth: depth, parentId: parentId, previousMove: previousMove, score: "no score"});
  throttledDraw();
}
function getNodeScore(nodeId, score) {
  nodes.find(n => n.id === nodeId).score = score;
  throttledDraw();
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

//visualization - search tree

const treeCanvas = document.getElementById('tree-canvas');
const ctx = treeCanvas.getContext('2d');

let offsetX = 0, offsetY = 0, zoom = 1;
let isDragging = false;
let dragStart = { x: 0, y: 0 };

treeCanvas.addEventListener('mousedown', e => {
  isDragging = true;
  dragStart = { x: e.clientX, y: e.clientY };
});

treeCanvas.addEventListener('mousemove', e => {
  if (isDragging) {
    offsetX += (e.clientX - dragStart.x);
    offsetY += (e.clientY - dragStart.y);
    dragStart = { x: e.clientX, y: e.clientY };
    draw();
  }
});

treeCanvas.addEventListener('mouseup', () => {
    isDragging = false;
});

treeCanvas.addEventListener('wheel', e => {
  const delta = e.deltaY < 0 ? 1.1 : 0.9;
  zoom *= delta;
  draw();
});

function draw() {
  treeCanvas.width = treeCanvas.clientWidth;
  treeCanvas.height = treeCanvas.clientHeight;

  ctx.clearRect(0, 0, treeCanvas.width, treeCanvas.height);
  ctx.save();
  ctx.translate(offsetX, offsetY);
  ctx.scale(zoom, zoom);

  //get amount of notes per depth
  let nodesOnDepth = [];
  for (const node of nodes) {
    const currDepth = node.depth;
    if (currDepth >= nodesOnDepth.length) {
      nodesOnDepth.push(0);
    }
    nodesOnDepth[currDepth]++;
  }

  let depthIterator = [];
  for (const depth of nodesOnDepth) {
    depthIterator.push(1);
  }

  for (const node of nodes)
  {
    node.y = node.depth * 100;
    node.x = treeCanvas.width / (nodesOnDepth[node.depth] + 1) * depthIterator[node.depth];
    depthIterator[node.depth]++;

    if (node.depth === 0)
    {
      continue;
    }

    const parent = nodes.find(n => n.id === node.parentId);

    ctx.beginPath();
    ctx.moveTo(node.x, node.y);
    ctx.lineTo(parent.x, parent.y);
    ctx.strokeStyle = 'white';
    ctx.stroke();

    //move label
    const midX = (node.x + parent.x) / 2;
    const midY = (node.y + parent.y) / 2;
    ctx.fillStyle = 'gray';
    ctx.font = '12px sans-serif';
    ctx.fillText(node.previousMove, midX + 5, midY - 5);
  }

  //draw edges
  for (const node of nodes) {
    ctx.beginPath();
    ctx.arc(node.x, node.y, 20, 0, Math.PI * 2);
    ctx.fillStyle = 'lightblue';
    ctx.fill();
    ctx.strokeStyle = 'white';
    ctx.stroke();

    if (node.score !== "no score")
    {
      ctx.fillStyle = 'black';
      ctx.font = '14px sans-serif';
      ctx.fillText(node.score, node.x - 10, node.y + 5);
    }
  }

  ctx.restore();
}

draw();

let drawCooldown = false;
const cooldownTime = 50; // in Millisekunden (z. B. 50ms = max 20 FPS)

function throttledDraw() {
  if (drawCooldown) return; // Wenn Cooldown aktiv, überspringen

  drawCooldown = true;
  draw(); // Deine eigentliche Zeichenfunktion

  setTimeout(() => {
    drawCooldown = false; // Nach Ablauf: wieder erlaubt
  }, cooldownTime);
}

// INPUT HANDLING

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

//make best move

const makeBestMoveButton = document.getElementById("make_best_move");
console.log("init");
makeBestMoveButton.addEventListener("click", (event) => {
  console.log("click");
  sendMakeBestMove();
});