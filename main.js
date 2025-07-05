// IMPORTS

const { app, BrowserWindow } = require('electron');
require('electron-reload')(__dirname);
const path = require('path');
const { spawn } = require('child_process');

// START ENGINE

/* const enginePath = path.join(__dirname, 'engine', 'x64', 'Debug', 'engine.exe');
console.log(enginePath);
const engineProc = spawn(enginePath); */

// CREATE WINDOW

function createWindow () {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
  });

  win.loadFile('main.html');
}

app.whenReady().then(() => {
  createWindow();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow();
    }
  });
})

app.on('window-all-closed', () => {
  //engineProc.kill();
  app.quit();
});

// INIT SOCKET

/* ws = new WebSocket('ws://localhost:8123');

ws.onopen = () => {
  console.log('ws open');
  ws.send('echo');
}

ws.onmessage = (event) => {
  console.log(event.data);
}

ws.onerror = (err) => {
  console.log(err);
} */