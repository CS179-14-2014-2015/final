pacman.stats = (function() {
    var points = 0;
    var level = 1;
    var lives = 3;
    var ghostPoints = 200;

    var scoreList = [];

    function addPoints(amount) {
        var before = points % 10000;
        points += amount;
        var after = points % 10000;
        if (after < before) {
            addLife();
        }
        updateView();
    }

    function addGhostPoints() {
        
        addPoints(ghostPoints);
        ghostPoints *= 2;
        updateView();
    }

    
    function frightStart() {
        ghostPoints = 200;
    }

    function addLife() {
        lives++;
        updateView();
    }
    function removeLife() {
        lives--;
        updateView();
        if (lives < 0) {
            return false;
        }
        return true;
    }
    function addLevel() {
        level++;
        pacman.updateElroy();
    }

    function updateView() {
        $("#points").html("Points: " + points);
        $("#lives").html("Lives: " + lives);
        $("#level").html("Level: " + level);
    }

    function resetStats() {
        points = 0;
        level = 1;
        lives = 3;
        updateView();
    }

    function sendPoints() {
        scoreList.push(points);
    }

    function getTopScores() {
        scoreList.sort(function(a, b) {
            return b - a;
        });
        return scoreList;
    }

    return {
        frightStart: frightStart,
        addPoints: addPoints,
        addGhostPoints: addGhostPoints,
        removeLife: removeLife,
        addLevel: addLevel,
        level: level,
        updateView: updateView,
        resetStats: resetStats,
        sendPoints: sendPoints,
        getTopScores: getTopScores
    };
})();