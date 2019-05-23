let grid_size = 15;
let cell_size;


function setup() {
    let canvas = createCanvas(600, 600) 
    canvas.parent('p5') 
    cell_size = width / grid_size;
    console.log("grid_size", grid_size)
    console.log("cell_size", cell_size)
    stroke(0)
    rect(0, 0, width, height)    
}

function draw() {
    // background(255)
    let types = ['red', 'blue'];
    let n = floor(random() * grid_size * grid_size)
    let i = floor(random() * 2)
    lightCell(n, types[i])
}

function neighborScore(n, color) {

}

function lightCell(n, color) {
    let y = floor(n / grid_size)
    let x = n % grid_size    
    noStroke()
    fill(color)    
    circle((x * cell_size) + (cell_size / 2), (y * cell_size) + (cell_size / 2), cell_size - 4)
}


/*
 so, what, how do we do this?
 agents are a color, a happiness, and a coordinate
 maybe forget agents, just do it by n

 for every n, check the neighbors in the race array to update the happiness array
 if unhappy, move to any random open space

 races
 happiness
 by index

 */