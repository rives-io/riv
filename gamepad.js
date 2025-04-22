function moveGamepadToBottom(options) {
  let gamepadElem = document.getElementById("gamepad");
  gamepadElem.style.display = "flex";

    let maxH = 0;
    document.querySelectorAll('.bottom').forEach(el => {
        const rect = el.getBoundingClientRect();
        maxH = Math.max(rect.bottom, maxH)
    });
    const gamePadPaddingBottom = options.paddingBottom ?? -20;
    const diffH = window.innerHeight - maxH - gamePadPaddingBottom;
    document.querySelectorAll('.bottom').forEach(el => {
        const rect = el.getBoundingClientRect();
        el.style.top = `${rect.top + diffH}px`;
    });
}

function getGamepadButtons() {
  const gamepadButtons = [];
  document.querySelectorAll('.button, .joystick').forEach(el => {
    const rect = el.getBoundingClientRect();
    const key = el.id.replace('button', '').replace('joystick', 'Arrow');
    gamepadButtons.push({
      rect,
      key,
    })
  });
  return gamepadButtons;
}

document.addEventListener("DOMContentLoaded", function () {
  // Parse params
  let hash = window.location.hash.substring(1);
  let params = hash.split('&').reduce(function (res, item) {
      var parts = item.split('=');
      res[parts[0]] = parts[1];
      return res;
  }, {});
  console.log("DOMContentLoaded",window.location.hash,params,!params.gamepad)
  if (!params.gamepad) return;
  moveGamepadToBottom({ paddingBottom: 40 });
  const gamepadButtons = getGamepadButtons();

  const pressed = {};

  const KEY_MAP = {
    "A": { key: 'a', code: 'KeyA', keyCode: 65, charCode: 97, which: 65, bubbles: true, elemId: "buttonA" },
    "D": { key: 'd', code: 'KeyD', keyCode: 68, charCode: 100, which: 68, bubbles: true, elemId: "buttonD" },
    "S": { key: 's', code: 'KeyS', keyCode: 83, charCode: 115, which: 83, bubbles: true, elemId: "buttonS" },
    "F": { key: 'f', code: 'KeyF', keyCode: 70, charCode: 102, which: 70, bubbles: true, elemId: "buttonF" },
    "V": { key: 'v', code: 'KeyV', keyCode: 86, charCode: 118, which: 86, bubbles: true, elemId: "buttonV" },
    "X": { key: 'x', code: 'KeyX', keyCode: 88, charCode: 120, which: 88, bubbles: true, elemId: "buttonX" },
    "Z": { key: 'z', code: 'KeyZ', keyCode: 90, charCode: 122, which: 90, bubbles: true, elemId: "buttonZ" },
    "C": { key: 'c', code: 'KeyC', keyCode: 67, charCode: 99, which: 67, bubbles: true, elemId: "buttonC" },
    "E": { key: 'e', code: 'KeyE', keyCode: 69, charCode: 101, which: 69, bubbles: true, elemId: "buttonE" },
    "W": { key: 'w', code: 'KeyW', keyCode: 87, charCode: 119, which: 87, bubbles: true, elemId: "buttonW" },
    "Q": { key: 'q', code: 'KeyQ', keyCode: 81, charCode: 113, which: 81, bubbles: true, elemId: "buttonQ" },
    "R": { key: 'r', code: 'KeyR', keyCode: 82, charCode: 114, which: 82, bubbles: true, elemId: "buttonR" },
    "ArrowUp": { key: 'ArrowUp', code: 'ArrowUp', keyCode: 38, charCode: 0, which: 38, bubbles: true, elemId: "joystickUp" },
    "ArrowDown": { key: 'ArrowDown', code: 'ArrowDown', keyCode: 40, charCode: 0, which: 40, bubbles: true, elemId: "joystickDown" },
    "ArrowLeft": { key: 'ArrowLeft', code: 'ArrowLeft', keyCode: 37, charCode: 0, which: 37, bubbles: true, elemId: "joystickLeft" },
    "ArrowRight": { key: 'ArrowRight', code: 'ArrowRight', keyCode: 39, charCode: 0, which: 39, bubbles: true, elemId: "joystickRight"  }
  };


  const handleButtonPressIn = (key) => {
    if (pressed[key]) return;
    pressed[key] = true;
    if (params.vibrate) window.navigator.vibrate(10);
    setPressed(key, true);
  };

  const handleButtonPressOut = (key) => {
    if (!pressed[key]) return;
    pressed[key] = false;
    setPressed(key, false);
  };

  const setPressed = (key, isPressed) => {
    const k = KEY_MAP[key];
    const event = new KeyboardEvent(isPressed ? 'keydown' : 'keyup', k);
    document.dispatchEvent(event);
    let els = document.getElementById(k.elemId).getElementsByClassName('inner-btn');
    if (!els || els.length == 0) return;
    let el = els[0];
    el.style.backgroundColor = isPressed ? 'rgba(255, 153, 153, 1)' : 'rgba(255, 153, 153, 0.8)';
    // el.style.backgroundColor = isPressed ? 'rgba(255, 100, 100, 0.6)' : 'rgba(255, 100, 100, 0.4)';
    // document.body.style.backgroundColor = isPressed ? `rgb(100,100,${Math.floor(Math.random() * 255)})` : 'rgb(100,100,100)';
  };

  const endTouches = (touches) => {
    const pressed = {};
    for (let i = 0; i < touches.length; i++) {
      const touch = touches[i];
      const { clientX: x, clientY: y } = touch;
      for (let btn of gamepadButtons) {
        if (x > btn.rect.left && x < btn.rect.right && y > btn.rect.top && y < btn.rect.bottom) {
          pressed[btn.key] = false;
        }
      }
    }
    for (const key in pressed) {
      if (pressed[key]) handleButtonPressIn(key);
      else handleButtonPressOut(key);
    }
  };

  const updateTouches = (touches, ending) => {
    const pressed = {
      S: false,
      A: false,
      D: false,
      F: false,
      V: false,
      X: false,
      Z: false,
      C: false,
      E: false,
      W: false,
      Q: false,
      R: false,
      ArrowRight: false,
      ArrowLeft: false,
      ArrowDown: false,
      ArrowUp: false
    };

    for (let i = 0; i < touches.length; i++) {
      const touch = touches[i];
      const { clientX: x, clientY: y } = touch;
      for (let btn of gamepadButtons) {
        if (x > btn.rect.left && x < btn.rect.right && y > btn.rect.top && y < btn.rect.bottom) {
          pressed[btn.key] = true;
        }
      }
    }

    // console.log(pressed)
    for (const key in pressed) {
      if (pressed[key]) handleButtonPressIn(key);
      else handleButtonPressOut(key);
    }
  };

  document.addEventListener('touchstart', (e) => {
    const touches = e.touches;
    // console.log(`Number of touches starting: ${touches.length}`);
    updateTouches(touches);
  }, false);

  document.addEventListener('touchmove', (e) => {
    const touches = e.touches;
    // console.log(`Number of touches moving: ${touches.length}`);
    updateTouches(touches);
  }, false);

  document.addEventListener('touchend', (e) => {
    const touches = e.changedTouches;
    // console.log(`Number of touches ending: ${touches.length}`);
    endTouches(touches, true);
  });

});