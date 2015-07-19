// listen to messages from server
function listen() {
    user.socket.on("init", function(obj) {
        user.agar = obj.agar;
        user.userID = obj.userID;
        food = obj.food;
        onlineUsers = obj.onlineUsers;
        spine = obj.spine;

        runnable = true;
    });

    user.socket.on("update", function(obj) {
        onlineUsers = obj.onlineUsers;
        food = obj.food;
        spine = obj.spine;
        if (onlineUsers.hasOwnProperty(user.userID)) {
            user.agar = onlineUsers[user.userID].agar;
        }
        if (runnable) {
            window.requestAnimationFrame(run);
        }
    });

    user.socket.on("death", function(obj) {
        if (obj.deathID === user.userID) {
            user.agar = null;
            user.maxPoints = 0;
            runnable = false;

            // remove event handler of mouse and keyboard
            document.removeEventListener("mousemove", mouseMove);
            document.removeEventListener("keydown", keyDown);
            // hello dialog appear
            document.getElementById("overlays").style.display = "block";
            var hello = document.getElementById("helloDialog");
            hello.style.display = "block";
            hello.style.opacity = 0;
            var intervalID = setInterval(function () {
                var current = parseInt(getComputedStyle(hello).getPropertyValue("opacity"));
                if (current < 100) {
                    hello.style.opacity = Math.min(current + 2, 100);
                }
                else {
                    clearInterval(intervalID);
                }
            }, 100);
        }
    });
}

// event handler for mouse move
function mouseMove(event) {
    mouse.x = event.pageX;
    mouse.y = event.pageY;
}

// event handler for keydown
function keyDown(event) {
    switch (event.keyCode) {
        // space
        case 0x20:
            eatSelf = false;
            setTimeout(function() {
                eatSelf = true;
            }, 2000);
            user.socket.emit("split", {
                userID: user.userID,
                mouseX: mouse.x * zoomRatio + user.center.x - canvas.width / 2,
                mouseY: mouse.y * zoomRatio + user.center.y - canvas.height / 2
            });
            break;
    }
}

// resize canvas as soon as window resizes.
window.onresize = function () {
    canvas.width = window.innerWidth * zoomRatio;
    canvas.height = window.innerHeight * zoomRatio;
};