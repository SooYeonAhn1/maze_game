document.addEventListener("DOMContentLoaded", () => {
    const display = document.getElementById("maze-display");

    function updateMaze() {
        fetch("/maze-data")
            .then(res => res.text())
            .then(data => {
                display.textContent = data;
            });
    }

    updateMaze(); // Initial load

    document.addEventListener("keydown", (e) => {
        let dx = 0, dy = 0;
        if (e.key === "w") dx = -1;
        else if (e.key === "s") dx = 1;
        else if (e.key === "a") dy = -1;
        else if (e.key === "d") dy = 1;
        else return;

        fetch(`/move/${dx}/${dy}`)
            .then(res => res.json())
            .then(data => {
                display.textContent = data.maze;
                if (data.done) {
                    window.location.href = "/win";
                }
            })
            .catch(err => console.error("Move error:", err));
    });

});
