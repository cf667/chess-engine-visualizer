// MESSAGE IDs

const ID_RENDERBOARD = 1;
const ID_FEN = 2;
const ID_MOVE = 3;
const ID_MAKEBESTMOVE = 4;
const ID_NEWNODE = 5;
const ID_NODESCORE = 6;
const ID_SETTINGS = 7;

// MESSAGE HANDLERS

//input

function inputMessageHandler(message) {
  switch (message.id)
  {
    case ID_RENDERBOARD:
      renderPosition(message.position);
      break;
    case ID_NEWNODE:
      getNewNode(message.nodeId, message.depth, message.parentId, message.previousMove);
      break;
    case ID_NODESCORE:
      getNodeScore(message.nodeId, message.score);
      break;
  }
}

//send fen (ID 2)

function sendFen(message) {
  const jsonMsg = { id: ID_FEN, fen: message };
  ws.send(JSON.stringify(jsonMsg));
}

//send move (ID 3)

let originSquare = 0;
let destinationSquare = 0;
function sendMove() {
  const jsonMsg = { id: ID_MOVE, origin: parseInt(originSquare), destination: parseInt(destinationSquare) };
  ws.send(JSON.stringify(jsonMsg));
}

//send "make best move" command (ID 4)

function sendMakeBestMove() {
  const jsonMsg = { id: ID_MAKEBESTMOVE };
  ws.send(JSON.stringify(jsonMsg));
}

//send settings

const searchMode = document.getElementById("search_mode");
const depthLabel = document.getElementById("depth_label");
const depthSlider = document.getElementById("search_depth");
const timeInput = document.getElementById("search_time");
const alphaBetaCheckbox = document.getElementById("enable_alpha_beta");
const moveSortingCheckbox = document.getElementById("enable_move_sorting");
const transpositionCheckbox = document.getElementById("enable_transposition_table");

function sendSettings() {
  const jsonMsg = {
    id: ID_SETTINGS, 
    searchByDepth: !searchMode.checked, 
    searchDepth: parseInt(depthSlider.value), 
    searchTime: parseInt(timeInput.value), 
    useAlphaBetaPruning: alphaBetaCheckbox.checked, 
    useMoveSorting: moveSortingCheckbox.checked, 
    useTranspositionTable: transpositionCheckbox.checked };

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
  nodes.push({ id: nodeId, depth: depth, parentId: parentId, previousMove: previousMove, score: "no score", childCount: 0 });
  if (parentId) {
    nodes.find(n => n.id === parentId).childCount++;
  }
}
function getNodeScore(nodeId, score) {
  nodes.find(n => n.id === nodeId).score = score;
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

//switch between sections

const btnGame = document.getElementById("btn_game_view");
const btnSettings = document.getElementById("btn_settings_view");
const gameView = document.getElementById("game_view");
const settingsView = document.getElementById("settings_view");

btnGame.addEventListener("click", () => {
  gameView.classList.remove("hidden");
  settingsView.classList.add("hidden");
});

btnSettings.addEventListener("click", () => {
  settingsView.classList.remove("hidden");
  gameView.classList.add("hidden");
});

//depth search slider

const depthValue = document.getElementById("depth_value");

depthSlider.addEventListener("input", () => {
  depthValue.textContent = depthSlider.value;
});

//switch between depth and time search

const timeLabel = document.getElementById("time_label");

searchMode.addEventListener("change", () => {
  if (searchMode.checked) {
    depthLabel.classList.add("hidden");
    timeLabel.classList.remove("hidden");
  } else {
    depthLabel.classList.remove("hidden");
    timeLabel.classList.add("hidden");
  }
});

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
const context = treeCanvas.getContext('2d');

//dragging and zooming
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

let depthInfo = [];
let bottomNodeIterator = 1;

function renderNode(node) {
  if (depthInfo[node.depth].renderMode === "count") {
    //count
    node.x = null;
    return;
  }

  node.y = node.depth * 100 / zoom;

  if (!node.childCount) {
    node.x = treeCanvas.width / (depthInfo[node.depth].totalNodes + 1) * bottomNodeIterator;
    bottomNodeIterator++;

    //draw line
    context.beginPath();
    context.moveTo(node.x, node.y);
    context.lineTo(parent.x, parent.y);
    context.strokeStyle = 'white';
    context.stroke();

    //draw node
    if (node.renderMode === "eval") {
      context.beginPath();
      context.arc(node.x, node.y, 20 / zoom, 0, Math.PI * 2);
      context.fillStyle = 'lightblue';
      context.fill();

      if (node.score !== "no score")
      {
        context.fillStyle = 'black';
        context.font = `${14 / zoom}px sans-serif`;
        context.fillText(node.score, node.x - 7 / zoom, node.y + 7 / zoom);
      }
    }
    else if (node.renderMode === "dot") {
      context.beginPath();
      context.arc(node.x, node.y, 5 / zoom, 0, Math.PI * 2);
      context.fillStyle = 'lightblue';
      context.fill();
    }
    return;
  }

  const childNodes = nodes.filter(n => n.parentId === node.id);
  for (const childNode of childNodes) {
    renderNode(childNode);
  }
  if (childNodes[0].x === null) {
    node.x = treeCanvas.width / (depthInfo[node.depth].totalNodes + 1) * bottomNodeIterator;
    bottomNodeIterator++;
  }
  else {
    console.log(node);
    node.x = ((childNodes[childNodes.length - 1].x - childNodes[0].x) / 2) + childNodes[0].x;
    for (const childNode of childNodes) {
      context.beginPath();
      context.moveTo(node.x, node.y);
      context.lineTo(childNode.x, childNode.y);
      context.strokeStyle = 'white';
      context.lineWidth = 1 / zoom;
      context.stroke();
    }
  }

  //draw node
  if (depthInfo[node.depth].renderMode === "eval") {
    context.beginPath();
    context.arc(node.x, node.y, 20 / zoom, 0, Math.PI * 2);
    context.fillStyle = 'lightblue';
    context.fill();

    if (node.score !== "no score")
    {
      context.fillStyle = 'black';
      context.font = `${14 / zoom}px sans-serif`;
      context.fillText(node.score, node.x - 7 / zoom, node.y + 7 / zoom);
    }
  }
  else if (depthInfo[node.depth].renderMode === "dot") {
    context.beginPath();
    context.arc(node.x, node.y, 5 / zoom, 0, Math.PI * 2);
    context.fillStyle = 'lightblue';
    context.fill();
  }
}

//rendering
function draw() {
  treeCanvas.width = treeCanvas.clientWidth;
  treeCanvas.height = treeCanvas.clientHeight;

  context.clearRect(0, 0, treeCanvas.width, treeCanvas.height);
  context.save();
  context.translate(offsetX, offsetY);
  context.scale(zoom, zoom);

  //get amount of notes per depth
  depthInfo = [];
  const newDepth = { totalNodes: 0, renderMode: "count", iterator: 1 };
  for (const node of nodes) {
    if (node.depth >= depthInfo.length) {
      depthInfo.push({...newDepth});
    }
    depthInfo[node.depth].totalNodes++;
  }

  //get render mode for every depth
  for (depth of depthInfo) {
    const distanceBetweenNodes = (treeCanvas.width / depth.totalNodes) * zoom;
    if (distanceBetweenNodes > 50) {
      depth.renderMode = "eval";
    }
    else if (distanceBetweenNodes > 5) {
      depth.renderMode = "dot";
    }
    //else "count"
  }

  if (nodes[0]) {
    renderNode(nodes[0]);
  }
  bottomNodeIterator = 1;

  // for (const node of nodes)
  // {
  //   node.y = node.depth * 100;
  //   node.x = treeCanvas.width / (depthInfo[node.depth].totalNodes + 1) * depthInfo[node.depth].iterator;
  //   depthInfo[node.depth].iterator++;

  //   if (node.depth === 0) {
  //     continue;
  //   }

  //   const parent = nodes.find(n => n.id === node.parentId);

  //   context.beginPath();
  //   context.moveTo(node.x, node.y);
  //   context.lineTo(parent.x, parent.y);
  //   context.strokeStyle = 'white';
  //   context.stroke();

  //   //move label
  //   const midX = (node.x + parent.x) / 2;
  //   const midY = (node.y + parent.y) / 2;
  //   context.fillStyle = 'gray';
  //   context.font = '12px sans-serif';
  //   context.fillText(node.previousMove, midX + 5, midY - 5);
  // }

  // for (const node of nodes) {
  //   context.beginPath();
  //   context.arc(node.x, node.y, 20, 0, Math.PI * 2);
  //   context.fillStyle = 'lightblue';
  //   context.fill();
  //   context.strokeStyle = 'white';
  //   context.stroke();

  //   if (node.score !== "no score")
  //   {
  //     context.fillStyle = 'black';
  //     context.font = '14px sans-serif';
  //     context.fillText(node.score, node.x - 10, node.y + 5);
  //   }
  // }

  context.restore();
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

//apply settings

applyChangesButton = document.getElementById("apply_changes");
applyChangesButton.addEventListener("click", sendSettings);

//make best move

const makeBestMoveButton = document.getElementById("make_best_move");
console.log("init");
makeBestMoveButton.addEventListener("click", (event) => {
  console.log("click");
  sendMakeBestMove();
});