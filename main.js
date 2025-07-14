// IMPORTS

const { app, BrowserWindow } = require("electron");
require("electron-reload")(__dirname);
const path = require("path");
const { spawn } = require("child_process");

// START ENGINE

const enginePath = path.join(__dirname, "engine", "x64", "Debug", "engine.exe");
//const engineProc = spawn(enginePath);

// GET CONSOLE OUTPUTS OF ENGINE

// engineProc.stdout.on("data", (data) => {
//   process.stdout.write(data);
// });

// engineProc.stderr.on("data", (data) => {
//   console.log("ENGINE ERROR: " + data);
// });

// engineProc.on("close", (code) => {
//   console.log("ENGINE CLOSE: " + code);
// });

// engineProc.on("error", (error) => {
//   console.log("ENGINE ERROR: " + error);
// });

// engineProc.on("exit", (code) => {
//   console.log("ENGINE EXIT: " + code);
// });

// CREATE WINDOW

function createWindow () {
  const win = new BrowserWindow({
    show: false
  });

  win.maximize();
  win.show();

  win.loadFile("main.html");
}

app.whenReady().then(() => {
  createWindow();

  app.on("activate", () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow();
    }
  });
})

// CLOSE WINDOW AND ENGINE

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    //engineProc.kill();
    app.quit()
  }
});