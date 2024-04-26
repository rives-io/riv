var lastEntropy = '';
var lastArgs = '';
var lastIncard = new Uint8Array([]);
var lastOutcard;
var lastTape;
var lastCartridge;
var lastFrame;
var lastTotalFrames;
var lastTargetFps;
var runtimeInitialized = false;

let statusElem = document.getElementById("status");
let argsElem = document.getElementById("args");
let entropyElem = document.getElementById("entropy");
let timeElem = document.getElementById("time");
let scoreElem = document.getElementById("score");
let progressElem = document.getElementById("progress");
let frameElem = document.getElementById("frame");
let fpsElem = document.getElementById("fps");
let cpuCostElem = document.getElementById("cpuCost");
let cpuSpeedElem = document.getElementById("cpuSpeed");
let cpuUsageElem = document.getElementById("cpuUsage");
let cpuCyclesElem = document.getElementById("cpuCycles");
let resolutionElem = document.getElementById("resolution");
let canvasElem = document.getElementById("canvas");
let canvasOverlayElem = document.getElementById("canvas-overlay");
let tapesizeElem = document.getElementById("tapesize");
let tapehashElem = document.getElementById("tapehash");
let entropyhashElem = document.getElementById("entropyhash");
let entropysizeElem = document.getElementById("entropysize");
let argshashElem = document.getElementById("argshash");
let argssizeElem = document.getElementById("argssize");
let inhashElem = document.getElementById("inhash");
let insizeElem = document.getElementById("insize");
let outhashElem = document.getElementById("outhash");
let outsizeElem = document.getElementById("outsize");
let cartsizeElem = document.getElementById("cartsize");
let carthashElem = document.getElementById("carthash");
let textDecoder = new TextDecoder();
let textEncode = new TextEncoder();
var Module = {};

// Prevent space bar from clicking buttons
window.addEventListener('keydown', function(e) {
  if(e.keyCode === 32) {
    e.preventDefault();
  }
});

// Alert when WebGL context is lost.
canvasElem.addEventListener("webglcontextlost", (e) => {
  alert('WebGL context lost. You will need to reload the page.'); e.preventDefault();
}, false);

canvasElem.ondragover = canvasElem.ondragenter = function(evt) {
  evt.preventDefault();
};

canvasElem.ondrop = function(e) {
  let file = e.dataTransfer.files[0];
  e.preventDefault();
  file.arrayBuffer().then(async function(a){
    await rivemuRecord(new Uint8Array(a));
  });
};

// Fired by Emscripten when WASM is ready.
Module.onRuntimeInitialized = function(status) {
  statusElem.textContent = "Idle";
  runtimeInitialized = true;
  window.dispatchEvent(new Event('rivemu_on_runtime_initialized'));
}

// Return SHA256 hexadecimal string from a chunk of data.
async function sha256sum(data) {
  const hashBuffer = await crypto.subtle.digest("SHA-256", data);
  const hashArray = Array.from(new Uint8Array(hashBuffer));
  const hashHex = hashArray.map((b) => b.toString(16).padStart(2, "0")).join("");
  return hashHex;
}

// Open an user file dialog for saving a file.
function downloadFileDialog(data, filename) {
  if (typeof data === 'undefined') {
    return;
  }
  let blob = new Blob([data], { type: "application/octet-stream" });
  let url = URL.createObjectURL(blob);
  let el = document.createElement('a');
  el.style.display = 'none';
  el.download = filename;
  el.href = url;
  document.body.appendChild(el);
  el.click();
  URL.revokeObjectURL(url);
  el.remove();
}

// Open an user file dialog for downloading a file.
async function uploadFileDialog(ext) {
  let src = await new Promise(function(resolve, reject) {
    let el = document.createElement("input");
    el.type = "file";
    el.accept = ext;
    el.addEventListener('change', function() {
      if (el.files.length) {
        resolve(el.files[0]);
      } else {
        reject("No file");
      }
      el.remove();
    });
    el.click();
  });
  let url = URL.createObjectURL(src);
  let response = await fetch(url);
  URL.revokeObjectURL(url);
  if (!response.ok) {
    return null;
  }
  const buffer = await response.arrayBuffer();
  const data = new Uint8Array(buffer);
  return data;
}

// Fetch a file from a URL.
async function downloadFile(url) {
  // Retrieve cartridge
  const response = await fetch(url, {method: "GET", mode: "cors", cache: "no-cache"});
  if (!response.ok) {
    return null;
  }
  const buffer = await response.arrayBuffer();
  const data = new Uint8Array(buffer);
  return data;
}

// Waits for a window event to fire.
function waitEvent(name) {
  return new Promise((resolve) => {
    const listener = (e) => {
      window.removeEventListener(name, listener);
      resolve(e);
    }
    window.addEventListener(name, listener);
  })
}

// Wait the WASM emulator be downloaded and initialized.
async function waitRuntimeInitialize() {
  if (!runtimeInitialized) {
    statusElem.textContent = "Downloading emulator...";
    await waitEvent("rivemu_on_runtime_initialized");
  }
}

// Called before record/replaying.
async function rivemuBeforeStart(tape, cartridge, incard, entropy, args) {
  // Update statistics
  statusElem.textContent = "Loading...";
  scoreElem.textContent = "N/A";
  timeElem.textContent = "N/A";
  progressElem.textContent = "N/A";
  frameElem.textContent = "N/A";
  fpsElem.textContent = "N/A";
  cpuCostElem.textContent = "N/A";
  cpuSpeedElem.textContent = "N/A";
  cpuUsageElem.textContent = "N/A";
  cpuCyclesElem.textContent= "N/A";
  resolutionElem.textContent = "N/A";
  cartsizeElem.textContent = Math.ceil(cartridge.length / 1024) + " KB";
  carthashElem.textContent = await sha256sum(cartridge);
  inhashElem.textContent = await sha256sum(incard);
  insizeElem.textContent = incard.length + " B";
  entropyhashElem.textContent = await sha256sum(textEncode.encode(entropy));
  entropysizeElem.textContent = entropy.length + " B";
  argshashElem.textContent = await sha256sum(textEncode.encode(args));
  argssizeElem.textContent = args.length + " B";
  if (tape) {
    tapesizeElem.textContent = tape.length + " B";
    tapehashElem.textContent = await sha256sum(tape);
  } else {
    tapesizeElem.textContent = "N/A";
    tapehashElem.textContent = "N/A";
    outhashElem.textContent = "N/A";
    outsizeElem.textContent = "N/A";
  }
  canvasOverlayElem.style.display = "none";

  // Disable some buttons while recording/replaying
  document.getElementById('stop').disabled = false;
  document.getElementById('record').disabled = false;
  document.getElementById('replay').disabled = false;
  document.getElementById('download_cartridge').disabled = false;
}

async function rivemuUploadCartridge(url) {
  await rivemuStop();
  statusElem.textContent = "Downloading cartridge...";
  let file = url ? await downloadFile(url) : await uploadFileDialog(".sqfs");
  await rivemuRecord(file);
}

function rivemuDownloadCartridge() {
  downloadFileDialog(lastCartridge, "cartridge.sqfs");
}

async function rivemuUploadTape(url) {
  let file = url ? await downloadFile(url) : await uploadFileDialog(".rivtape");
  await rivemuReplay(file);
}

function rivemuDownloadTape() {
  downloadFileDialog(lastTape, "gameplay.rivtape");
}

async function rivemuUploadIncard(url) {
  let file = url ? await downloadFile(url) : await uploadFileDialog(".rivincard");
  await rivemuRecord(null, file);
}

function rivemuDownloadIncard() {
  downloadFileDialog(lastIncard, "gameplay.rivincard");
}

function rivemuDownloadOutcard() {
  downloadFileDialog(lastOutcard, "gameplay.rivoutcard");
}

// Stop recording/replaying.
async function rivemuStop() {
  await waitRuntimeInitialize();
  if (Module.ccall('rivemu_stop')) {
    statusElem.textContent = "Stopping...";
    await waitEvent('rivemu_on_shutdown');
  }
  statusElem.textContent = "Stopped";
}

// Start recording a tape.
async function rivemuRecord(cartridge, incard, entropy, args) {
  // Wait previous run to finish
  await rivemuStop();
  // Set default arguments
  cartridge = cartridge || lastCartridge;
  incard = incard || lastIncard;
  entropy = entropy || entropyElem.value;
  args = args || argsElem.value;
  lastCartridge = cartridge;
  lastIncard = incard;
  lastEntropy = entropy;
  lastArgs = args;
  await rivemuBeforeStart(null, cartridge, incard, entropy, args);
  await waitRuntimeInitialize();
  // Move cartridge into WASM memory
  const cartridgeBuf = Module._malloc(cartridge.length);
  const incardBuf = Module._malloc(incard.length);
  Module.HEAPU8.set(cartridge, cartridgeBuf);
  Module.HEAPU8.set(incard, incardBuf);
  Module.ccall('rivemu_start_record', null, ['number', 'number', 'number', 'number', 'string', 'string'], [
    cartridgeBuf, cartridge.length,
    incardBuf, incard.length,
    entropy, args
  ]);
  Module._free(cartridgeBuf);
  Module._free(incardBuf);
}

// Start replaying a tape.
async function rivemuReplay(tape, cartridge, incard, entropy, args) {
  // Wait previous run to finish
  await rivemuStop();
  // Set default arguments
  tape = tape || lastTape;
  cartridge = cartridge || lastCartridge;
  incard = incard || lastIncard;
  entropy = entropy || entropyElem.value;
  args = args || argsElem.value;
  lastTape = tape;
  lastCartridge = cartridge;
  lastIncard = incard;
  lastEntropy = entropy;
  lastArgs = args;
  await rivemuBeforeStart(tape, cartridge, incard, entropy, args);
  await waitRuntimeInitialize();
  // Move cartridge into WASM memory
  const cartridgeBuf = Module._malloc(cartridge.length);
  const incardBuf = Module._malloc(incard.length);
  const tapeBuf = Module._malloc(tape.length);
  Module.HEAPU8.set(cartridge, cartridgeBuf);
  Module.HEAPU8.set(incard, incardBuf);
  Module.HEAPU8.set(tape, tapeBuf);
  Module.ccall('rivemu_start_replay', null, ['number', 'number', 'number', 'number', 'string', 'string', 'number', 'number'], [
    cartridgeBuf, cartridge.length,
    incardBuf, incard.length,
    entropy, args,
    tapeBuf, tape.length
  ]);
  Module._free(cartridgeBuf);
  Module._free(incardBuf);
  Module._free(tapeBuf);
}

// Enter fullscreen mode.
function rivemuFullscreen() {
  canvasElem.requestFullscreen();
}

// Called by RIVEMU before the first frame.
function rivemu_on_begin(width, height, target_fps, total_frames) {
  lastFrame = 0;
  lastTotalFrames = total_frames;
  lastTargetFps = target_fps;
  resolutionElem.textContent =
    width+"x"+height+"@"+target_fps + " -> " + canvasElem.width+"x"+canvasElem.height+"@60";
  frameElem.textContent = "0";
  if (total_frames > 0) {
    progressElem.textContent = "0%";
    statusElem.textContent = "Replaying...";
  } else {
    statusElem.textContent = "Recording...";
  }
}

// Called by RIVEMU after the last frame.
async function rivemu_on_finish(tape, outcard, outhash) {
  // We need to make a deep copy of a Uint8Array here, otherwise
  lastTape = new Uint8Array(tape);
  lastOutcard = new Uint8Array(outcard);
  // Show outcard
  let outcard_str = textDecoder.decode(outcard);
  if (outcard_str.substring(0, 4) == 'JSON') {
    let scores = JSON.parse(outcard_str.substring(4));
    console.log(scores);
  }
  // Update buttons
  document.getElementById('stop').disabled = true;
  document.getElementById('replay').disabled = false;
  document.getElementById('download_outcard').disabled = false;
  document.getElementById('download_incard').disabled = false;
  document.getElementById('download_tape').disabled = false;
  // Update labels
  statusElem.textContent = "Stopped";
  outhashElem.textContent = outhash;
  outsizeElem.textContent = outcard.length + " B";
  tapesizeElem.textContent = tape.length + " B";
  tapehashElem.textContent = await sha256sum(tape);
}

// Called by RIVEMU on every frame.
function rivemu_on_frame(outcard, frame, cycles, fps, cpu_cost, cpu_speed, cpu_usage, cpu_quota) {
  const mins = Math.floor(frame / (60 * lastTargetFps));
  const secs = (frame / lastTargetFps) - (60 * mins);
  timeElem.textContent = mins + " mins " + secs.toFixed(3) + " secs";
  fpsElem.textContent = fps.toFixed(2) + " fps";
  cpuCostElem.textContent = cpu_cost.toFixed(2) + " MIPS";
  cpuSpeedElem.textContent = cpu_speed.toFixed(2) + " MIPS";
  cpuUsageElem.textContent = cpu_usage.toFixed(2) + " %";
  cpuCyclesElem.textContent = (cycles / 1000000).toFixed(3) + " M (" + cpu_quota.toFixed(2) + "% quota)";
  frameElem.textContent = frame.toFixed(0);
  if (lastTotalFrames > 0) {
    progressElem.textContent = (frame*100/lastTotalFrames).toFixed(2) + "%";
  }
  let outcard_str = textDecoder.decode(outcard);
  if (outcard_str.substring(0, 4) == 'JSON') {
    let scores = JSON.parse(outcard_str.substring(4));
    if (typeof scores.score !== 'undefined') {
      scoreElem.textContent = scores.score;
    }
  }
}

let hash = window.location.hash.substr(1);
let params = hash.split('&').reduce(function (res, item) {
    var parts = item.split('=');
    res[parts[0]] = parts[1];
    return res;
}, {});

// Play external cartridge
if (params.cartridge) {
  rivemuUploadCartridge(params.cartridge);
}
