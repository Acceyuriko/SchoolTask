/**
 * Created by iiii on 15-5-18.
 */


const boundary = {x: 8000, y: 6000};
const maxDepth = 4;
const maxChildren = 100;

var user = {
    socket: null,
    userID: null,
    name: null,
    agar: null,
    maxPoints: 0,
    center: {x: 4000, y: 3000}
};

var quadtree = new window.QuadTree(
    {x: 0, y: 0, width: boundary.x, height: boundary.y},
    false,
    maxDepth,
    maxChildren
);

var food = null;
var onlineUsers = null;
var spine = null;
var runnable = false;
var eatSelf = false;
var eatSpine = true;

var mouse = {
    x: 0,
    y: 0
};

var zoomRatio = 1;

var canvas = document.getElementById("canvas");

// initial game
function init() {
    user.socket = io.connect("ws://localhost:3000");
    //user.socket = io.connect("ws://192.168.16.108:3000");

    canvas.width = window.innerWidth * zoomRatio;
    canvas.height = window.innerHeight * zoomRatio;

    listen();
}

// set user name and start animation
function setName(name) {
    if (name === "") {
        user.name = "an unnamed cell";
    }
    else {
        user.name = name;
    }

    document.getElementById("overlays").style.display = "none";
    user.socket.emit("login", {userName: user.name});

    // record of mouse postion
    document.addEventListener("mousemove", mouseMove);
    document.addEventListener("keydown", keyDown);
}

// draw game scene
function run() {
    if (user.agar === null || onlineUsers === null || food === null || spine === null) {
        if (runnable) {
            window.requestAnimationFrame(run);
        }
        return;
    }

    canvas.width = window.innerWidth * zoomRatio;
    canvas.height = window.innerHeight * zoomRatio;
    var context = canvas.getContext("2d");
    //context.clearRect(0, 0, canvas.width, canvas.height);

    var centerOfWindow = {x: canvas.width/2, y: canvas.height/2};


    // compute center of user, which is the mean of coordinates of agars owned by user
    // compute zoomRatio
    var sumX = 0, sumY = 0;
    var markI = 0, markJ = 0;
    var i = 0;
    for (i = 0; i < user.agar.length; i++) {
        sumX += user.agar[i].x;
        sumY += user.agar[i].y;
    }
    user.center.x = sumX / user.agar.length;
    user.center.y = sumY / user.agar.length;

    // only one agar
    zoomRatio = Math.sqrt(20 * user.agar[0].radius * user.agar.length / window.innerWidth);
    // zoomRatio = 2 * (maxDist + user.agar[markI].radius + user.agar[markJ].radius) / window.innerWidth;

    drawGrid();
    drawFood();
    drawEnemy();
    drawAgar();
    drawSpine();
    drawPoints();

    checkCollision();

    // draw grid relatively
    function drawGrid() {
        context.save();
        context.translate(-user.center.x, -user.center.y);
        context.beginPath();
        for (var x = canvas.width / 2; x <= boundary.x + canvas.width / 2; x += 40) {
            context.moveTo(x, canvas.height / 2);
            context.lineTo(x, boundary.y + canvas.height / 2);
        }
        for (var y = canvas.height / 2; y <= boundary.y + canvas.height / 2; y += 40) {
            context.moveTo(canvas.width / 2, y);
            context.lineTo(boundary.x + canvas.width / 2, y);
        }
        context.strokeStyle = "#303030";
        context.lineWidth = 0.4;
        context.stroke();
        context.closePath();
        context.restore();
    }

    // draw agars relatively
    function drawAgar() {
        if (user.agar === null) {
            return;
        }
        for (var i = 0; i < user.agar.length; i++) {
            var fontPosition = {
                x: user.agar[i].x - user.center.x + centerOfWindow.x,
                y: user.agar[i].y - user.center.y + centerOfWindow.y
            };
            context.beginPath();
            context.arc(
                fontPosition.x,
                fontPosition.y,
                user.agar[i].radius,
                0,
                2*Math.PI
            );
            context.fillStyle = user.agar[0].color;
            context.fill();
            context.closePath();

            // draw name text
            context.fillStyle = "rgb(220, 220, 220)";
            var fontsize = Math.sqrt(user.agar[i].radius) * 4;
            context.textAlign = "center";
            context.textBaseline = 'middle';
            context.font = fontsize + "px sans-serif";
            context.lineWidth = 0.4;
            context.strokeText(user.name, fontPosition.x, fontPosition.y);
            context.fillText(user.name, fontPosition.x, fontPosition.y);
        }
    }

    // draw points
    function drawPoints() {
        var points = 0;
        if (user.agar === null) {
            points = 0;
        }
        else {
            for (var i = 0; i < user.agar.length; i++) {
                points += user.agar[i].area;
            }
            points = Math.sqrt(points / Math.PI);
            if (user.maxPoints < points) {
                user.maxPoints = points;
            }
            else {
                points = user.maxPoints;
            }
        }
        points = Math.floor(points);
        context.fillStyle = "rgb(200, 200, 200)";
        context.fillRect(
            10 * zoomRatio ,
            canvas.height - 50 * zoomRatio ,
            (95 + points.toString().length * 20) * zoomRatio,
            40 * zoomRatio
        );
        context.fillStyle = "rgb(255, 255, 255)";
        context.font = 30 * zoomRatio + "px sans-serif";
        context.textAlign = "left";
        context.textBaseline = "Top";
        context.fillText("Score:" + points, 15 * zoomRatio, canvas.height - 30 * zoomRatio);
    }

    // draw enemy
    function drawEnemy() {
        if (onlineUsers === null) {
            return;
        }
        context.save();
        context.translate(-user.center.x + centerOfWindow.x, -user.center.y + centerOfWindow.y);

        for (var key in onlineUsers) {
            if (!onlineUsers.hasOwnProperty(key) || onlineUsers[key].agar === null) {
                continue;
            }
            if (key === user.userID) {
                continue;
            }
            for (var i = 0; i < onlineUsers[key].agar.length; i++) {
                var position = {
                    x: onlineUsers[key].agar[i].x,
                    y: onlineUsers[key].agar[i].y
                };
                context.beginPath();
                context.arc(
                    position.x,
                    position.y,
                    onlineUsers[key].agar[i].radius,
                    0,
                    2 * Math.PI
                );
                context.fillStyle = onlineUsers[key].agar[0].color;
                context.fill();
                context.closePath();

                // draw name text
                context.fillStyle = "rgb(220, 220, 220)";
                var fontsize = Math.sqrt(onlineUsers[key].agar[i].radius) * 4;
                context.textAlign = "center";
                context.textBaseline = 'middle';
                context.font = fontsize + "px sans-serif";
                context.lineWidth = 0.4;
                context.strokeText(onlineUsers[key].name, position.x, position.y);
                context.fillText(onlineUsers[key].name, position.x, position.y);

            }
        }

        context.restore();
    }

    function drawSpine() {
        if (spine === null) {
            return;
        }
        context.save();
        context.translate(-user.center.x + centerOfWindow.x, -user.center.y + centerOfWindow.y);

        for (var i = 0; i < spine.length; i++) {
            if (spine[i] === undefined) {
                continue;
            }
            context.beginPath();
            context.moveTo(spine[i].x + spine[i].radius, spine[i].y);
            for (var j = 0; j <= 360; j += 6) {
                var r = 0;
                var radian = j / (2 * Math.PI);
                if (j % 12 == 0) {
                    r = spine[i].radius;
                }
                else {
                    r = spine[i].radius + 12;
                }
                context.lineTo(spine[i].x + r * Math.cos(radian), spine[i].y + r * Math.sin(radian));
            }
            context.fillStyle = spine[i].color;
            context.fill();
            context.closePath();
        }

        context.restore();
    }

    // draw food
    function drawFood() {
        context.save();
        context.translate(-user.center.x, -user.center.y);
        for (var i = 0; i < food.length; i++) {
            context.beginPath();
            context.arc(
                food[i].x + canvas.width / 2,
                food[i].y + canvas.height / 2,
                food[i].radius,
                0,
                2*Math.PI
            );
            context.fillStyle = food[i].color;
            context.fill();
            context.closePath();
        }
        context.restore();
    }

    function checkCollision() {
        quadtree.clear();
        quadtree.insert(food);
        quadtree.insert(spine);
        for (var key in onlineUsers) {
            if (onlineUsers[key].agar !== null) {
                quadtree.insert(onlineUsers[key].agar);
            }
        }

        var agar = user.agar.slice();
        for (var i = 0; i < user.agar.length; i++) {
            var objectToCheck = quadtree.retrieve(user.agar[i]);
            for (var j = 0; j < objectToCheck.length; j++) {
                if (objectToCheck[j] === undefined) {
                    continue;
                }
                var dist = Math.sqrt(
                    Math.pow(user.agar[i].x - objectToCheck[j].x, 2) +
                    Math.pow(user.agar[i].y - objectToCheck[j].y, 2)
                );
                if (dist < Math.max(objectToCheck[j].radius, user.agar[i].radius)) {
                    if (objectToCheck[j] === user.agar[i]) {
                        // todo
                    }
                    else {
                        if (objectToCheck[j].type === "food") {
                            user.agar[i].area += objectToCheck[j].area;
                            user.agar[i].radius = Math.sqrt(user.agar[i].area / Math.PI);
                            food.splice(objectToCheck[j].id, 1);
                            user.socket.emit(
                                "eat food", {
                                    foodID: objectToCheck[j].id,
                                    userID: user.userID,
                                    i: i,
                                    area: user.agar[i].area,
                                    radius: user.agar[i].radius
                                }
                            );
                        }
                        else if (objectToCheck[j].type === "user") {
                            if (user.userID !== objectToCheck[j].id &&
                                user.agar[i].radius * 1.15 < objectToCheck[j].radius) {
                                objectToCheck[j].area += user.agar[i].area;
                                objectToCheck[j].radius = Math.sqrt(objectToCheck[j].area / Math.PI);
                                if (user.agar.length === 1) {
                                    onlineUsers[user.userID].agar = null;
                                }
                                user.socket.emit(
                                    "eat user", {
                                        eateeID: user.userID,
                                        eateeNum: i,
                                        eaterArea: objectToCheck[j].area,
                                        eaterRadius: objectToCheck[j].radius,
                                        eaterID: objectToCheck[j].id,
                                        eaterNum: objectToCheck[j].num
                                    }
                                );
                            }
                            else if (eatSelf &&
                                user.userID === objectToCheck[j].id &&
                                i > objectToCheck[j].num) {
                                if (agar[i] !== null) {
                                    var eaterNum = objectToCheck[j].num;
                                    user.agar[eaterNum].area += user.agar[i].area;
                                    user.agar[eaterNum].radius = Math.sqrt(user.agar[eaterNum].area / Math.PI);
                                    delete agar[i];
                                    user.socket.emit(
                                        "eat user", {
                                            eateeID: user.userID,
                                            eateeNum: i,
                                            eaterArea: user.agar[eaterNum].area,
                                            eaterRadius: user.agar[eaterNum].radius,
                                            eaterID: user.userID,
                                            eaterNum: eaterNum
                                        }
                                    );
                                }
                            }
                        }
                        else if (eatSpine &&
                            objectToCheck[j].type === "spine" &&
                            user.agar[i].radius * 1.4 > objectToCheck[j].radius) {
                            eatSpine = false;
                            setTimeout(function () {
                                eatSpine = true;
                            }, 1000);

                            var count = 0;
                            if (user.agar[i].area > 10 * Math.PI * 20 * 20) {
                                count = 10;
                            }
                            else {
                                count = Math.floor(user.agar[i].area / (Math.PI * 20 * 20));
                            }

                            user.agar[i].area = user.agar[i].area / count;
                            user.agar[i].radius = Math.sqrt(user.agar[i].area / Math.PI);

                            eatSelf = false;
                            setTimeout(function() {
                                eatSelf = true;
                            }, 4000);

                            user.socket.emit(
                                "eat spine", {
                                    eaterID: user.userID,
                                    eaterNum: i,
                                    area: user.agar[i].area,
                                    radius: user.agar[i].radius,
                                    spineID: objectToCheck[j].id,
                                    count: count
                                }
                            );
                            delete spine[objectToCheck[j].id];
                        }
                    }
                }
            }
            user.agar = agar.filter(function(item) {
                return item !== undefined;
            });
        }

        // update the mouse position
        var mousePosition = {
            x: mouse.x * zoomRatio - centerOfWindow.x + user.center.x,
            y: mouse.y * zoomRatio - centerOfWindow.y + user.center.y
        };


        user.socket.emit("move", {mouse: mousePosition, id: user.userID});
    }
}

init();


