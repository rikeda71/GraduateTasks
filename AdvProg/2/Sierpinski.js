var ctx;
var x = 256, y = 256;
var dx = 8, dy = 0;
var h = 0;

function forwardCPS(k) {
  ctx.strokeStyle = "hsla(" + h +", 100%, 50%, 0.8)";
  h++;
  ctx.beginPath();
  ctx.moveTo(x, y);
  ctx.lineTo(x += dx, y += dy);
  ctx.closePath();
  ctx.stroke();
  return k;
}

function turnLeft() {
  var tmp = dx; dx = dy; dy = -tmp;
}

function turnRight() {
  var tmp = dx; dx = -dy; dy = tmp;
}

function sierpinskiCPS(n, k) {
  return zigCPS(n, function(ignore) {
    return zigCPS(n, k);
  });
}

function zigCPS(n, k) {
  if (n <= 1) {
    turnLeft();
    return forwardCPS(function(ignore) {
      turnLeft();
      return forwardCPS(k);
    });
  } else {
    return zigCPS(n / 2, function(ignore) {
      return zagCPS(n / 2, function(ignore) {
        return zigCPS(n / 2, function(ignore) {
          return zagCPS(n / 2, k);
        });
      });
    });
  }
}

function zagCPS(n, k) {
  if (n <= 1) {
    turnRight();
    return forwardCPS(function(ignore) {
      turnRight();
      return forwardCPS(function(ignore){
        turnLeft();
        return forwardCPS(k);
      });
    });
  } else {
    return zagCPS(n / 2, function(ignore) {
      return zagCPS(n / 2, function(ignore) {
        return zigCPS(n / 2, function(ignore) {
          return zigCPS(n / 2, k);
        });
      });
    });
  }
}

function doEnd(n) {
  return doEnd;
}

var restart = function(ignore) {
  return sierpinskiCPS(16, doEnd);
};

function exec() {
  var canvas = document.getElementById('canvas');
  ctx = canvas.getContext("2d");
  console.log(typeof restart);
  restart = restart(0);
}
