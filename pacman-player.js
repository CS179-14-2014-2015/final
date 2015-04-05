pacman.Player = function() {

    this.position = pacman.playerStart;
    this.originalStart = pacman.playerStart;
    this.movement = {x: -1, y: 0, objectRotation: 180};
    this.newMovement = this.movement;
    this.mode = "chase";

 
    this.paperObject = pacman.paper.path();

    this.animate = function() {
        this.paperObject.attr(pacman.svg.pacmanBody(this.position, this.movement.objectRotation));
    };

    this.move = function() {
        var speed = pacman.tools.playerSpeed(this.mode);

        if (this.isChangingDirection() && this.canChangeDirection() && this.canMove(this.newMovement, speed)) {
            
            if (this.movement.x === (-1) * this.newMovement.x && this.movement.y === (-1) * this.newMovement.y) {
                this.doMovement(this.newMovement, speed);
            } else {
                this.align();
                this.doMovement(this.newMovement, speed);
            }

         
            this.movement = this.newMovement;
        } else {
           
            if (this.canMove(this.movement, speed)) {
                this.doMovement(this.movement, speed);
            } else {
                
                this.align();
            }
            this.newMovement = this.movement;
        }

        this.correctPosition();
    };

    this.isChangingDirection = function() {

        if (this.movement.x !== this.newMovement.x || this.movement.y !== this.newMovement.y) {
            return true;
        }
        return false;
    };

    this.canChangeDirection = function() {
        
        if (this.movement.x === (-1) * this.newMovement.x && this.movement.y === (-1) * this.newMovement.y) {
            return true;
        }
        var tilePosition = pacman.tools.getTilePosition(this.position);
        var tileMiddlePosition = {
            x: pacman.config.tileSize * tilePosition.col + pacman.config.tileSize / 2,
            y: pacman.config.tileSize * tilePosition.row + pacman.config.tileSize / 2
        };

        var distanceFromMiddle = pacman.tools.distanceBetween(this.position.x,
                this.position.y, tileMiddlePosition.x, tileMiddlePosition.y);
        if (distanceFromMiddle < pacman.config.tileSize / 8) {
            return true;
        }
        return false;
    };

    this.align = function() {
        
        var tilePosition = pacman.tools.getTilePosition(this.position);
        var tileMiddlePosition = {
            x: pacman.config.tileSize * tilePosition.col + pacman.config.tileSize / 2,
            y: pacman.config.tileSize * tilePosition.row + pacman.config.tileSize / 2
        };
        this.position = tileMiddlePosition;
    };

    this.canMove = function(movement, speed) {
        
        var edgePosition = {
            x: this.position.x + (pacman.config.tileSize / 2 + speed) * movement.x,
            y: this.position.y + (pacman.config.tileSize / 2 + speed) * movement.y
        };
    
        var tilePosition = pacman.tools.getTilePosition(edgePosition);
    
        if (pacman.playerMovement[tilePosition.row] === undefined ||
                pacman.playerMovement[tilePosition.row][tilePosition.col] === undefined ||
                pacman.playerMovement[tilePosition.row][tilePosition.col]) {
            return true;
        }
        return false;
    };

    this.doMovement = function(movement, speed) {
        var newPosition = {
            x: this.position.x + speed * movement.x,
            y: this.position.y + speed * movement.y
        };
        this.position = newPosition;
    };

    
    this.correctPosition = function() {
        
        if (this.position.x < 0) {
            this.position.x += pacman.config.tileSize * pacman.fieldInUse.width;
        }
    
        if (this.position.x > pacman.config.tileSize * pacman.fieldInUse.width) {
            this.position.x -= pacman.config.tileSize * pacman.fieldInUse.width;
        }
        
        if (this.position.y < 0) {
            this.position.y += pacman.config.tileSize * pacman.fieldInUse.height;
        }
        
        if (this.position.y > pacman.config.tileSize * pacman.fieldInUse.height) {
            this.position.y -= pacman.config.tileSize * pacman.fieldInUse.height;
        }
    };

    this.eat = function() {
        
        var playerPosition = pacman.tools.getTilePosition(this.position);
        
        if (pacman.pellets[playerPosition.row][playerPosition.col] !== undefined &&
                !pacman.pellets[playerPosition.row][playerPosition.col].eaten) {
           
            pacman.pelletCount--;
            
			if(pacman.pellets[playerPosition.row][playerPosition.col].isTeleport) {
				
				
				
				//alert("departure : " + row + " " + col);				
				//var i = pacman.teleports.length;
				//while(i >= 0) {
				
				var row = playerPosition.row;
				var col = playerPosition.col;
				var tpFound = false;
				for (var i = 0; i < pacman.teleports.length; i++) {
				
					var teleport = pacman.teleports[i];				
					var trow = teleport.row;
					var tcol = teleport.col;
					
					//alert("test : " + trow + " " + tcol);
					
					//departure tp
					if(row == trow && tcol == col) {
						//remove form list
						//pacman.teleports.splice(i, 1);	
						pacman.pellets[trow][tcol].hide();
						pacman.pellets[trow][tcol].eaten = true;	
						//alert("departure removed");
						
					}
					else {
						//arrival tp
						if(!pacman.pellets[trow][tcol].eaten && !tpFound) {
					
							//move
							this.position.x = tcol * pacman.config.tileSize;
							this.position.y = trow * pacman.config.tileSize;
							
							
							//align();
							
							//remove tp
							pacman.pellets[trow][tcol].hide();
							pacman.pellets[trow][tcol].eaten = true;
							tpFound = true;		
							
							//pacman.teleports.splice(i, 1);						
							//alert("arrival removed");
						}
						
					}
					//i--;
					
					
					
					/*
					var tpPosition = pacman.pellets[trow][tcol].position;
					alert(tpPosition.row);
					*/
					
					/*
					var tpPos = pacman.tools.getTilePosition(pellet.position);
					alert(tpPos.row);
					*/
					
					//if(trow = row
					
					
					
					//found a tp
					
					
					//remove departure tp		
								
				}			
			}
			
            if (pacman.pellets[playerPosition.row][playerPosition.col].isPowerPellet) {
                
                pacman.startFright();
                pacman.stats.addPoints(50);
            } 
			else {
                
                pacman.stats.addPoints(10);
            }
           
            pacman.pellets[playerPosition.row][playerPosition.col].hide();
            pacman.pellets[playerPosition.row][playerPosition.col].eaten = true;
            return true;
        }
        return false;
    };

    this.checkCollisions = function() {
        var playerPosition = pacman.tools.getTilePosition(pacman.player.position);
        $.each(pacman.ghosts, function(index, ghost) {
            var ghostPosition = pacman.tools.getTilePosition(ghost.position);
            if (playerPosition.row === ghostPosition.row && playerPosition.col === ghostPosition.col) {
                
                if (ghost.mode === "fright") {
                    ghost.setMode("dead");
                    pacman.stats.addGhostPoints();
                } else if (ghost.mode !== "dead") {
                    if (pacman.stats.removeLife()) {
                        
                        pacman.death(pacman.continueAfterDeath);
                    } else {
                        // no lifes left so end game
                        pacman.death(pacman.end);
                    }
                }
            }
        });
    };

    this.setMode = function(mode) {
        this.mode = mode;
    };

    // resets position and movements to start conditions
    this.reset = function() {
        this.position = pacman.playerStart;
        this.movement = {x: -1, y: 0, objectRotation: 180};
        this.newMovement = this.movement;
        this.mode = "chase";
    };
};
