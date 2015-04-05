pacman.Enemy = function(name, startPosition) {
    this.name = name;
    this.originalStart = startPosition;
    this.position = startPosition;
    this.movement = {x: -1, y: 0};
    this.untilNextTile = pacman.config.tileSize / 2;
    this.mode = "chase";
    this.forcedTarget = pacman.goodTarget;
    this.elroyLevel = 0;
    this.ai = pacman.ai[this.name];
    this.colour = pacman.config.colours.ghost[this.name];
    this.body = pacman.paper.path();
    this.leftEye = pacman.paper.circle(0, 0, 0);
    this.leftPupil = pacman.paper.circle(0, 0, 0);  
    this.rightEye = pacman.paper.circle(0, 0, 0);
    this.rightPupil = pacman.paper.circle(0, 0, 0);

    this.animate = function() {
        this.body.attr(pacman.svg.ghostBody(this.position, this.colour));
        this.leftEye.attr(pacman.svg.ghostEye("left", this.position));
        this.leftPupil.attr(pacman.svg.ghostPupil("left", this.position, this.movement));
        this.rightEye.attr(pacman.svg.ghostEye("right", this.position));
        this.rightPupil.attr(pacman.svg.ghostPupil("right", this.position, this.movement));
    };

    this.move = function() {
        var speed = pacman.tools.enemySpeed(this.mode, this.elroyLevel);
        var ownTilePosition = pacman.tools.getTilePosition(this.position);
        if (this.mode !== "dead" && pacman.slowMovement[ownTilePosition.row][ownTilePosition.col]) {
            speed /= 2;
        }

        if (speed <= this.untilNextTile) {
            this.untilNextTile -= speed;
            this.doMovement(speed);
        } else {
            this.doMovement(this.untilNextTile);
            
            if (pacman.goodTarget.col === ownTilePosition.col &&
                    pacman.goodTarget.row === ownTilePosition.row) {
                
                if (this.mode === "dead") {
                    this.forcedTarget = pacman.ghostHome;
                } else {
                    
                    this.forcedTarget = null;
                }
            } else if (this.mode === "dead" && pacman.ghostHome.col === ownTilePosition.col &&
                    pacman.ghostHome.row === ownTilePosition.row) {
                
                this.mode = "";
                this.setMode("chase");
                
                this.forcedTarget = pacman.goodTarget;
            }

            var newMovement = this.ai.getMovement(this.movement, ownTilePosition, this.mode, this.forcedTarget);
            if (newMovement !== undefined) {
                this.movement = newMovement;
            }
            var speedLeft = speed - this.untilNextTile;
            this.doMovement(speedLeft);
            this.untilNextTile = pacman.config.tileSize - speedLeft;
        }

    
        this.correctPosition();
    };

    this.doMovement = function(speed) {
        var newPosition = {
            x: this.position.x + speed * this.movement.x,
            y: this.position.y + speed * this.movement.y
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

    this.setMode = function(mode) {
        switch (mode) {
            
            case "chase":
            case "scatter":
            case "fright":
                if (this.mode !== "dead") {
                    this.mode = mode;
                }
                break;
                
            case "dead":
                this.mode = mode;
                this.forcedTarget = pacman.goodTarget;
                break;
        }
        if (this.mode === "dead") {
            this.body.hide();
            return;
        }
        if (this.mode === "fright") {
            this.colour = pacman.config.colours.frightGhost;
            this.body.show();
            return;
        }
        this.colour = pacman.config.colours.ghost[this.name];
        this.body.show();
    };
    this.reset = function() {
        this.position = this.originalStart;
        this.movement = {x: -1, y: 0};
        this.untilNextTile = pacman.config.tileSize / 2;
        this.setMode("chase");
        this.forcedTarget = pacman.goodTarget;
    };
};
