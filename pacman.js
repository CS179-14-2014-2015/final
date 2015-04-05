var pacman = (function() {
 
    var frame = 0;
    var screen;
    var nivel1 = 0;
    var nivel2 = 0;
    function actualizarnivel() 
    {
        nivel1 = pacman.tools.elroyLevel();
        nivel2 = nivel1 / 2;
    }
    function tick() {
        pacman.frame++;
        pacman.frame %= 30;
        var movement = pacman.keyhandler.getMovement();
        if (movement !== null) 
        {
            pacman.player.newMovement = movement;
        }
        moveObjects();
        animateObjects();
        pacman.player.checkCollisions();
        

        if (pacman.player.eat()) {
            if (pacman.pelletCount === nivel1) {
                pacman.ghosts[0].elroyLevel = 1;
            } else if (pacman.pelletCount === nivel2) {
                pacman.ghosts[0].elroyLevel = 2;
            } else if (pacman.pelletCount === 0) {
                // next level!
                nextLevel();
            }
        }
    }
    function moveObjects() {
        
        pacman.player.move();
        
        $.each(pacman.ghosts, function(index, ghost) {
            ghost.move();
        });
    }

    function animateObjects() {
        
        pacman.player.animate();
        
        $.each(pacman.ghosts, function(index, ghost) {
            ghost.animate();
        });
    }

    function setObjectsMode(mode) {
        // pac-man
        pacman.player.setMode(mode);
        // ghosts
        $.each(pacman.ghosts, function(index, ghost) {
            ghost.setMode(mode);
        });
    }

    function resetObjects() {
        pacman.player.reset();
        $.each(pacman.ghosts, function(index, ghost) {
            ghost.reset();
        });
    }

    // mode start and interval functions

    function startMode(mode) {
        pacman.mode = mode;
        setObjectsMode(mode);
    }

    function startChase() {

        startMode("chase");
        pacman.modeTimeout = setTimeout(startScatter, 15000);
    }

    function startFright() {
        pacman.stats.frightStart();
        pacman.blinkTimer = 3;
        startMode("fright");
        clearTimeout(pacman.modeTimeout);
        clearTimeout(pacman.phaseEndInterval);
        pacman.modeTimeout = setTimeout(frightPhaseEnd, 7000);
    }

    function frightPhaseEnd() {
        pacman.phaseEndTimer = 3;
        pacman.phaseEndInterval = setInterval(blink, 1000);
    }

    function blink() {
        if (pacman.phaseEndTimer === 0) {
            clearInterval(pacman.phaseEndInterval);
            startChase();
        } else {
            pacman.svg.blinkGhosts();
            pacman.phaseEndTimer--;
        }
    }

    function startScatter() {
        startMode("scatter");
        pacman.modeTimeout = setTimeout(startChase, 5000);
    }
    function nextLevel() {
        stop();
        pacman.stats.addLevel();
        pacman.actualizarnivel();
        pacman.fieldBuilder.renewPellets();
        resetObjects();
        animateObjects();
        startWithCountDown(2);
    }

    function startWithCountDown(time) {
        pacman.countdownTimer = time;
        pacman.countDownInterval = setInterval(start, 1000);
    }
    function start() {
        if (pacman.countdownTimer <= 0 || pacman.countdownTimer === undefined) {
            clearInterval(pacman.countDownInterval);
            pacman.gameInterval = setInterval(tick, 1000 / 60);
            startChase();
        } else {
            pacman.svg.animateCountdownNumber(pacman.playerStart, pacman.countdownTimer);
            pacman.countdownTimer--;
        }
    }
    function stop() {
        clearTimers();
    }
    function end() {
        setupEnd();
    }
    function death(callback) {
        stop();
        $.each(pacman.ghosts, function(index, ghost) {
            ghost.body.hide();
            ghost.leftEye.hide();
            ghost.rightEye.hide();
            ghost.leftPupil.hide();
            ghost.rightPupil.hide();
        });
        pacman.svg.animatePlayerDeath(callback);
    }

   
    function continueAfterDeath() {
        resetObjects();
        $.each(pacman.ghosts, function(index, ghost) {
            ghost.body.show();
            ghost.leftEye.show();
            ghost.rightEye.show();
            ghost.leftPupil.show();
            ghost.rightPupil.show();
        });
        animateObjects();
        startWithCountDown(1);
    }
    function setupStart() {
        
        pacman.fieldBuilder.buildStartScreen();
        screen = "start";
    }
    function setupPlayField() {
        pacman.fieldBuilder.buildPlayField();
        pacman.ghosts = [];
        pacman.ghosts.push(new pacman.Enemy("blinky", pacman.enemyStarts[0]));
        pacman.ghosts.push(new pacman.Enemy("inky", pacman.enemyStarts[1]));
        pacman.ghosts.push(new pacman.Enemy("pinky", pacman.enemyStarts[2]));
        pacman.ghosts.push(new pacman.Enemy("clyde", pacman.enemyStarts[3]));
        pacman.player = new pacman.Player();
        pacman.actualizarnivel();
        animateObjects();
        pacman.stats.resetStats();
        screen = "play";
    }
    function setupEnd() {
        pacman.fieldBuilder.buildEndScreen();
        pacman.stats.sendPoints();
        screen = "end";
    }

    function enterEvent() {
        clearTimers();
        if (screen === "play") {
            stop();
            setupStart();
        } else if (screen === "start") {
            setupPlayField();
            startWithCountDown(2);
        } else if (screen === "end") {
            setupStart();
        }
    }

    function clearTimers() {
        clearInterval(pacman.gameInterval);
        clearInterval(pacman.phaseEndTimer);
        clearInterval(pacman.countDownInterval);
        clearTimeout(pacman.modeTimeout);
    }

    return {
        setupStart: setupStart,
        setupPlayField: setupPlayField,
        startWithCountDown: startWithCountDown,
        start: start,
        death: death,
        continueAfterDeath: continueAfterDeath,
        end: end,
        frame: frame,
        startFright: startFright,
        resetObjects: resetObjects,
        actualizarnivel: actualizarnivel,
        enterEvent: enterEvent
    };
})();

$(document).ready(function() {
    // create paper
    pacman.paper = Raphael(pacman.config.containerId, 0, 0);

    // bind keyhandler
    $(document).keydown(function(eventInfo) {
        pacman.keyhandler.keydown(eventInfo.keyCode);
    });
    $(document).keyup(function(eventInfo) {
        pacman.keyhandler.keyup(eventInfo.keyCode);
    });

    // start with start screen
    pacman.setupStart();
});
