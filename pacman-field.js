pacman.fieldBuilder = (function() {

    function clearPaper() {
       
        pacman.paper.clear();
        
        var paperWidth = pacman.config.playField.width * pacman.config.tileSize;
        var paperHeight = pacman.config.playField.height * pacman.config.tileSize;
        pacman.paper.setSize(paperWidth, paperHeight);
        var background = pacman.paper.rect(0, 0, paperWidth, paperHeight);
        background.attr("fill", pacman.config.colours.background);
    }

    function buildPlayField() {
        pacman.fieldInUse = pacman.config.playField;

        clearPaper();

        pacman.playerMovement = [];
        pacman.enemyMovement = [];
        
        pacman.pellets = [];
        
        pacman.enemyStarts = [];
       
        pacman.slowMovement = [];
        
        pacman.pelletCount = 0;
		
		pacman.teleports = [];

        
        for (var row = 0; row < pacman.config.playField.height; row++) {
            pacman.playerMovement[row] = [];
            pacman.enemyMovement[row] = [];
            pacman.pellets[row] = [];
            pacman.slowMovement[row] = [];
            for (var col = 0; col < pacman.config.playField.width; col++) {
                pacman.playerMovement[row][col] = true;
                pacman.enemyMovement[row][col] = true;
                pacman.slowMovement[row][col] = false;
                switch (pacman.config.playField.field[row].charAt(col)) {
                    case "W":
                        
                        createWall(row, col);
                        pacman.playerMovement[row][col] = false;
                        pacman.enemyMovement[row][col] = false;
                        break;
                    case "G":
                        
                        createGate(row, col);
                        pacman.playerMovement[row][col] = false;
                                                break;
                    case "C":
                        
                        createPellet(row, col);
                        pacman.pelletCount++;
                        break;
						
					case "T":
						createTeleport(row, col);
						pacman.pelletCount++;
						break;
					
                    case "U":
                       
                        createPowerPellet(row, col);
                        pacman.pelletCount++;
                        break;
                    case "P":
                        
                        setPlayerStart(row, col);
                        break;
                    case "H":
                        
                        setEnemyStart(row, col);
                        
                        break;
                    case "B":
        
                        break;
                    case "Z":
                        pacman.goodTarget = {row: row, col: col};
                        break;
                    case "X":
                        pacman.ghostHome = {row: row, col: col};
                        break;
                    case "S":
                        pacman.slowMovement[row][col] = true;
                        break;
                }
            }
        }
    }

    
    function createWall(row, col) {
        var wall = pacman.paper.rect(
                col * pacman.config.tileSize + pacman.config.tileSize / 6,
                row * pacman.config.tileSize + pacman.config.tileSize / 6,
                pacman.config.tileSize / 1.5,
                pacman.config.tileSize / 1.5);
        wall.attr("fill", pacman.config.colours.wall);
        wall.attr("stroke", "none");
    }

    function createGate(row, col) {
        var gate = pacman.paper.rect(
                col * pacman.config.tileSize + pacman.config.tileSize / 6,
                row * pacman.config.tileSize + pacman.config.tileSize / 6,
                pacman.config.tileSize / 1.5,
                pacman.config.tileSize / 1.5);
        gate.attr("fill", pacman.config.colours.gate);
        gate.attr("stroke", "none");
    }

    function createPellet(row, col) {
        var pellet = pacman.paper.rect(
                col * pacman.config.tileSize + pacman.config.tileSize / (8 / 3),
                row * pacman.config.tileSize + pacman.config.tileSize / (8 / 3),
                pacman.config.tileSize / 4,
                pacman.config.tileSize / 4);
        pellet.attr("fill", pacman.config.colours.pellet);
        pellet.attr("stroke", "none");
        pellet.eaten = false;
        pacman.pellets[row][col] = pellet;
    }
	
	function createTeleport(row, col) {
	var pellet = pacman.paper.rect(
                col * pacman.config.tileSize + pacman.config.tileSize / (8 / 3),
                row * pacman.config.tileSize + pacman.config.tileSize / (8 / 3),
                pacman.config.tileSize / 4,
                pacman.config.tileSize / 4);
        pellet.attr("fill", pacman.config.colours.teleport);
        pellet.attr("stroke", "none");
        pellet.eaten = false;
		pellet.isTeleport = true;
		pellet.row = row;
		pellet.col = col;
        pacman.pellets[row][col] = pellet;
		pacman.teleports[pacman.teleports.length] = pellet;	
	}

    function createPowerPellet(row, col) {
        var powerPellet = pacman.paper.rect(
                col * pacman.config.tileSize + pacman.config.tileSize / (10 / 3),
                row * pacman.config.tileSize + pacman.config.tileSize / (10 / 3),
                pacman.config.tileSize / 2.5,
                pacman.config.tileSize / 2.5);
        powerPellet.attr("fill", pacman.config.colours.powerPellet);
        powerPellet.attr("stroke", "none");
        
        var anim = Raphael.animation({
            "50%": {
                transform: "s1.5"
            },
            "100%": {
                transform: ""
            }
        }, 1000).repeat("Infinity");
        powerPellet.animate(anim);
        powerPellet.isPowerPellet = true;
        powerPellet.eaten = false;
        pacman.pellets[row][col] = powerPellet;
    }

    function setPlayerStart(row, col) {
        pacman.playerStart = {
            x: (col + 1) * pacman.config.tileSize,
            y: row * pacman.config.tileSize + pacman.config.tileSize / 2
        };
    }

    function setEnemyStart(row, col) {
        pacman.enemyStarts.push({
            x: (col + 1) * pacman.config.tileSize,
            y: row * pacman.config.tileSize + pacman.config.tileSize / 2
        });
    }

    function renewPellets() {
        for (var row = 0; row < pacman.fieldInUse.height; row++) {
            for (var col = 0; col < pacman.fieldInUse.width; col++) {
                if (pacman.pellets[row][col] !== undefined) {
                    pacman.pellets[row][col].show();
                    pacman.pellets[row][col].eaten = false;
                    pacman.pelletCount++;
                }
            }
        }
    }

    function buildStartScreen() {
        clearPaper();
        var x = pacman.config.playField.width / 2 * pacman.config.tileSize;
        var y = pacman.config.playField.height / 3 * pacman.config.tileSize;
        var maxY = pacman.config.playField.height * (2/3) * pacman.config.tileSize;
        var startText = pacman.paper.text(x, y, "Press ENTER");
        startText.attr({fill: pacman.config.colours.textColour});
        var scores = pacman.stats.getTopScores();

        y += 2 * pacman.config.tileSize;
        pacman.paper.text(x, y, "Scores:").attr({fill: pacman.config.colours.textColour});

        $.each(scores, function(index, score) {
            y += pacman.config.tileSize;
            if (y > maxY) {
                return;
            }
            pacman.paper.text(x, y, score).attr({fill: pacman.config.colours.textColour});
        });

    }

    function buildEndScreen() {
        clearPaper();
        var startText = pacman.paper.text(
                pacman.config.playField.width / 2 * pacman.config.tileSize,
                pacman.config.playField.height / 2 * pacman.config.tileSize,
                "GAME OVER\n\n ENTER"
                );
        startText.attr({fill: pacman.config.colours.textColour});
    }

    return {
        buildPlayField: buildPlayField,
        renewPellets: renewPellets,
        buildStartScreen: buildStartScreen,
        buildEndScreen: buildEndScreen
    };
})();
