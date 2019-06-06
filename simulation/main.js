'use strict'

const PIXELS_PER_ROW = 15
const ROWS = 15
const PIXELS = PIXELS_PER_ROW * ROWS

let color_1 = 'red'
let color_2 = 'blue'
let off_color = 'white'

let cell_size
let pixel_colors = []

function setup() {
    let canvas = createCanvas(400, 400)
    canvas.parent('p5') 
    cell_size = width / PIXELS_PER_ROW
    stroke(0)
    rect(0, 0, width, height)   
    background(off_color) 
    for (let pixel=0; pixel<PIXELS; pixel++) {
        pixel_colors.push(off_color)
    }
    start()
    frameRate(30)
}

function draw() {
    // console.log(loop)
    main()
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


