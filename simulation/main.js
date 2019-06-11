'use strict'

// const PIXELS_PER_ROW = 15
// const ROWS = 15
// const PIXELS = PIXELS_PER_ROW * ROWS

// let colors = [[255, 0, 0], [0, 0, 255], 'orange', 'purple', 'green']
// let off_color = [255, 255, 255]

let cell_size
let pixel_colors = []
let previous_pixel_colors = []

function setup() {
    let canvas = createCanvas(400, 400)
    canvas.parent('p5') 
    cell_size = width / PIXELS_PER_ROW
    stroke(0)
    rect(0, 0, width, height)   
    background(off_color) 
    resetColors()
    start()
    frameRate(30)
    textFont('monospace')
    strokeWeight(1)
}

function draw() {
    main()
    let fr = int(frameRate())
    fill(200)
    stroke(200)
    rect(0, 0, 20, 15)
    fill(0)
    stroke(0)    
    textSize(10)
    text(fr, 5, 10)
}

function getColor(n) {
    return pixel_colors[n]
}

function setColor(pixel, color) {
    let y = floor(pixel / ROWS)
    let x = pixel % PIXELS_PER_ROW    
    stroke(255)
    fill(color)    
    circle((x * cell_size) + (cell_size / 2), (y * cell_size) + (cell_size / 2), cell_size - 4)
    pixel_colors[pixel] = color
}

function resetColors() {
    pixel_colors = []
    previous_pixel_colors = []
    for (let pixel=0; pixel<PIXELS; pixel++) {
        pixel_colors.push(off_color)
        previous_pixel_colors.push(off_color)
    }
}
