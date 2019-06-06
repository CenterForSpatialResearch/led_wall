const PIXELS_PER_ROW = 15
const ROWS = 15
const PIXELS = PIXELS_PER_ROW * ROWS

let color_1 = 'red'
let color_2 = 'blue'
let off_color = 'white'

let cell_size
let pixel
let r

function setup() {
    let canvas = createCanvas(600, 600) 
    canvas.parent('p5') 
    cell_size = width / PIXELS_PER_ROW
    stroke(0)
    rect(0, 0, width, height)    
}

function draw() {
    pixel = int(random(0, PIXELS))
    r = int(random(4))
    if (r == 0) {
        setPixel(pixel, color_1)
    } else if (r == 1) {
        setPixel(pixel, color_2)
    } else if (r == 2) {
        setPixel(pixel, off_color)
    } else {

    }
}

function neighborScore(n, color) {

}

function setPixel(n, color) {
    let y = floor(n / ROWS)
    let x = n % PIXELS_PER_ROW    
    stroke(255)
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