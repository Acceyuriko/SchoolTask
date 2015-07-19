
/**
 * Module dependencies.
 */

var express = require("express");
var app = express();
var server = require("http").createServer(app);
var io = require("socket.io")(server);

//var user = {
//    type: "user",
//    userID: null,
//    agar: null
//};

const boundary = {x: 8000, y: 6000};
const speed = 33;

var onlineUsers = {};

var food = genFood();
var spine = genSpine();

var updateAll = setInterval(function() {
    io.emit("update", {
        onlineUsers: onlineUsers,
        food: food,
        spine: spine
    });
}, 16);

io.on("connection", function(socket){
    console.log("a user connected");
    var userID = genUid();
    socket.id = userID;


    // when new user login
    socket.on("login", function(obj){
        var agar = genAgar(userID);
        onlineUsers[userID] = {name: obj.userName, agar: agar};
        console.log(userID+ ":" + obj.userName + " login");
        socket.emit("init", {
            agar: agar,
            food: food,
            onlineUsers: onlineUsers,
            spine: spine,
            userID: userID});
    });

    // move message
    socket.on("move", function(obj){
        if (!onlineUsers.hasOwnProperty(obj.id) || onlineUsers[obj.id].agar === null) {
            return;
        }
        for (var i = 0; i < onlineUsers[obj.id].agar.length; i++) {
            //if (onlineUsers[obj.id].agar[i].status !== "normal") {
            //    continue;
            //}
            var x = onlineUsers[obj.id].agar[i].x, y = onlineUsers[obj.id].agar[i].y;
            var distance =
                Math.sqrt(
                    (x - obj.mouse.x) * (x - obj.mouse.x) +
                    (y - obj.mouse.y) * (y - obj.mouse.y)
                );

            // speed is relative to radius
            x += (speed * (obj.mouse.x - x) / distance) / Math.sqrt(onlineUsers[obj.id].agar[i].radius);
            y += (speed * (obj.mouse.y - y) / distance) / Math.sqrt(onlineUsers[obj.id].agar[i].radius);
            if (x > 0 && x < boundary.x) {
                onlineUsers[obj.id].agar[i].x = x;
            }
            if (y > 0 && y < boundary.y) {
                onlineUsers[obj.id].agar[i].y = y;
            }
        }

    });

    socket.on("eat food", function(obj){
        // update food
        var part = {
            x: 0,
            y: 0,
            type: "food",
            id: null,
            radius: 12,
            area: Math.PI * this.radius * this.radius,
            color: null
        };
        part.id = obj.foodID;
        part.x = Math.floor(Math.random() * boundary.x);
        part.y = Math.floor(Math.random() * boundary.y);
        part.color =
            "rgb("
            + Math.floor(Math.random()*255)
            + ","
            + Math.floor(Math.random()*255)
            + ","
            + Math.floor(Math.random()*255);

        food[obj.foodID] = part;

        // update agar
        if (onlineUsers.hasOwnProperty(obj.userID)) {
            if (onlineUsers[obj.userID].agar === undefined) {
                return;
            }
            onlineUsers[obj.userID].agar[obj.i].area = obj.area;
            onlineUsers[obj.userID].agar[obj.i].radius = obj.radius;
        }
    });

    socket.on("eat user", function(obj) {
        if (onlineUsers.hasOwnProperty(obj.eateeID)) {
            var agar = onlineUsers[obj.eateeID].agar;
            if (agar !== null && agar[obj.eateeNum] !== undefined) {
                agar.splice(obj.eateeNum, 1);
                if (onlineUsers[obj.eateeID].agar.length == 0) {
                    socket.emit("death", {
                        deathID: obj.eateeID
                    });
                    onlineUsers[obj.eateeID].agar = null;
                }
            }
        }
        if (onlineUsers.hasOwnProperty(obj.eaterID)) {
            var agar = onlineUsers[obj.eaterID].agar;
            if (agar !== null && agar[obj.eaterNum] !== undefined) {
                agar[obj.eaterNum].area = obj.eaterArea;
                agar[obj.eaterNum].radius = obj.eaterRadius;
            }
        }
    });

    socket.on("split", function(obj) {
        if (onlineUsers.hasOwnProperty(obj.userID)) {
            var agar = onlineUsers[obj.userID].agar;
            var len = agar.length;
            var deltaX = {};
            var deltaY = {};
            for (var i = 0; i < len; i++) {
                if (agar[i].radius > 40) {
                    agar[i].area /= 2;
                    agar[i].radius = Math.sqrt(agar[i].area / Math.PI);
                    var part = {
                        x: agar[i].x,
                        y: agar[i].y,
                        type: "user",
                        id: agar[i].id,
                        num: 0,
                        radius: agar[i].radius,
                        area: agar[i].area,
                        color: agar[i].color,
                        status: "fly"
                    };

                    agar.push(part);
                    var num = agar.length - 1;
                    agar[num].num = num;

                    var dist = Math.sqrt(
                        Math.pow(obj.mouseX - agar[i].x, 2) +
                        Math.pow(obj.mouseY - agar[i].y, 2));
                    deltaX[num] = 1.3 * agar[i].radius * (obj.mouseX - agar[i].x) / dist;
                    deltaY[num] = 1.3 * agar[i].radius * (obj.mouseY - agar[i].y) / dist;

                    // setInterval(func, delay[, param1, param2, ...]) is available
                    var splitID = setInterval(function (num) {
                        if (agar[num] === undefined) {
                            clearInterval(splitID);
                            return;
                        }
                        var nextX = agar[num].x + deltaX[num];
                        var nextY = agar[num].y + deltaY[num];
                        if (nextX > 0 && nextX < boundary.x) {
                            agar[num].x = nextX;
                        }
                        if (nextY > 0 && nextY < boundary.y) {
                            agar[num].y = nextY;
                        }
                        deltaX[num] = deltaX[num] / 1.1;
                        deltaY[num] = deltaY[num] / 1.1;

                        if (Math.abs(deltaX[num]) < 5 && Math.abs(deltaY[num]) < 5) {
                            agar[num].status = "normal";
                            clearInterval(splitID);
                        }
                    }, 50, num);
                }
            }
        }
    });

    socket.on("eat spine", function(obj) {
        if (onlineUsers.hasOwnProperty(obj.eaterID)) {
            var agar = onlineUsers[obj.eaterID].agar;
            agar[obj.eaterNum].area = obj.area;
            agar[obj.eaterNum].radius = obj.radius;

            var deltaX = {};
            var deltaY = {};
            for (var i = 0; i < obj.count - 1; i++) {
                var part = {
                    x: agar[obj.eaterNum].x,
                    y: agar[obj.eaterNum].y,
                    type: "user",
                    id: agar[obj.eaterNum].id,
                    num: agar.length,
                    radius: obj.radius,
                    area: obj.area,
                    color: agar[obj.eaterNum].color,
                    status: "fly"
                };

                agar.push(part);

                deltaX[part.num] = 33 * Math.cos(i * 2 * Math.PI / obj.count);
                deltaY[part.num] = 33 * Math.sin(i * 2 * Math.PI / obj.count);


                var splitID = setInterval(function (num) {
                    if (agar[num] === undefined) {
                        clearInterval(splitID);
                        return;
                    }
                    var nextX = agar[num].x + deltaX[num];
                    var nextY = agar[num].y + deltaY[num];
                    if (nextX > 0 && nextX < boundary.x) {
                        agar[num].x = nextX;
                    }
                    if (nextY > 0 && nextY < boundary.y) {
                        agar[num].y = nextY;
                    }
                    deltaX[num] = deltaX[num] / 1.1;
                    deltaY[num] = deltaY[num] / 1.1;

                    if (Math.abs(deltaX[num]) < 5 && Math.abs(deltaY[num]) < 5) {
                        agar[num].status = "normal";
                        clearInterval(splitID);
                    }
                }, 50, part.num);

                part = {
                    x: 0,
                    y: 0,
                    type: "spine",
                    id: obj.spineID,
                    radius: 50,
                    color: "rgb(10, 255, 0)"
                };

                part.x = Math.floor(Math.random() * boundary.x);
                part.y = Math.floor(Math.random() * boundary.y);

                spine[obj.spineID] = part;
            }
        }
    });

    // logout
    socket.on("disconnect", function() {
        if (onlineUsers.hasOwnProperty(socket.id)) {
            delete onlineUsers[socket.id];
        }
        //io.emit("update_users", {onlineUsers: onlineUsers});
        console.log("exit");
    });
});

app.use("/", express.static(__dirname));

app.get("/", function(req, res){
    res.sendFile(__dirname+"/views/index.html");
});
server.listen(3000, function(){
    console.log("listening on 3000:")
});

function genUid() {
    return new Date().getTime() + "" + Math.floor(Math.random() * 899 + 100);
}

// Generate a Agar when user login
function genAgar(userID){
    var agar = [];
    var part = {
        x: 0,
        y: 0,
        type: "user",
        id: null,
        num: 0,
        radius: 20,
        area: 0,
        color: null,
        status: "normal"
    };
    part.id = userID;
    part.area = Math.PI * part.radius * part.radius;
    part.x = Math.floor(Math.random() * boundary.x);
    part.y = Math.floor(Math.random() * boundary.y);
    part.color =
        "rgb("
        + Math.floor(Math.random()*255)
        + ","
        + Math.floor(Math.random()*255)
        + ","
        + Math.floor(Math.random()*255);

    agar.push(part);

    return agar;
}

function genFood(){
    var food = [];
    for (var i = 0; i < 300; i++) {
        var part = {
            x: 0,
            y: 0,
            type: "food",
            id: null,
            radius: 12,
            area: 0,
            color: null
        };
        part.area = Math.PI * part.radius * part.radius;
        part.id = i;
        part.x = Math.floor(Math.random() * boundary.x);
        part.y = Math.floor(Math.random() * boundary.y);
        part.color =
            "rgb("
            + Math.floor(Math.random()*255)
            + ","
            + Math.floor(Math.random()*255)
            + ","
            + Math.floor(Math.random()*255);

        food.push(part);
    }
    return food;
}

function genSpine(){
    var spine = [];
    for (var i = 0; i < 20; i++) {
        var part = {
            x: 0,
            y: 0,
            type: "spine",
            id: null,
            radius: 50,
            color: "rgb(10, 255, 0)"
        };
        part.id = i;
        part.x = Math.floor(Math.random() * boundary.x);
        part.y = Math.floor(Math.random() * boundary.y);

        spine.push(part);
    }

    return spine;
}
