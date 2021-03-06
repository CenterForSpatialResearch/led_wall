'use strict'

/* arduino style: minimize garbage collection and specialty functions */

// settable params
const PIXELS_PER_ROW = 15
const ROWS = 15
const TYPES = 2
const MAX_STEPS = 2500
const COUNTDOWN = 100
let COLORS = [[255, 0, 0], [0, 0, 255]]
const OFF_COLOR = [255, 255, 255]
const TRANSITION = 2

// dependent constants
const PIXELS = PIXELS_PER_ROW * ROWS
// const POPULATION = Math.floor(PIXELS / 2)
const POPULATION = 150

// states
const STARTUP = 0
const INTRO = 1
const PLAY = 2
const HOLD = 3
const CODA = 4

// dependent constants
let CELL_SIZE
let NEIGHBORS 

// declare persistant variables
let state
let index
let steps
let countdown

// interface elements
let r1_field
let g1_field
let b1_field
let r2_field
let g2_field
let b2_field

// allocate arrays
let pixel_colors = new Array(PIXELS)
let previous_pixel_colors = new Array(PIXELS)
let sequence = new Array(PIXELS)
let transitions = new Array(PIXELS)

// p5 specific initialization
function setup() {
    let canvas = createCanvas(400, 400)
    canvas.parent('p5') 

    r1_field = select('#R1')
    r1_field.changed(changeColors)
    g1_field = select('#G1')
    g1_field.changed(changeColors)
    b1_field = select('#B1')
    b1_field.changed(changeColors)
    r2_field = select('#R2')
    r2_field.changed(changeColors)
    g2_field = select('#G2')
    g2_field.changed(changeColors)
    b2_field = select('#B2')
    b2_field.changed(changeColors)

    CELL_SIZE = width / PIXELS_PER_ROW
    initNeighbors()
    stroke(0)
    rect(0, 0, width, height) 
    background(OFF_COLOR) 
    resetColors()
    frameRate(10)
    textFont('monospace')
    strokeWeight(1)
    reset()
    state = INTRO
}

// reset process
function reset() {
    console.log("start")
    state = INTRO
    index = 0
    steps = 0    
    countdown = COUNTDOWN
    background(OFF_COLOR)    
    resetColors()
    for (let t=0; t<sequence.length; t++) {
        transitions[t] = -1
    }    
    for (let s=0; s<sequence.length; s++) {
        sequence[s] = s
    }
    shuffleSequence(sequence)    
}

function draw() {

    if (state == STARTUP) {
        let pixel = index++;
        setColor(pixel, COLORS[pixel % TYPES]);
        if (index == PIXELS) {
            reset()
            return
        }
    }  

    else if (state == INTRO) {
        let pixel = sequence[index++]
        setColor(pixel, COLORS[index % TYPES])
        if (index == POPULATION) {
            state = PLAY        
            console.log("play")
        }
    } 

    else if (state == PLAY) {
        while (true) {
            if (steps == MAX_STEPS) {
                console.log("hit max")
                state = HOLD
                return
            }
            let pixel = sequence[index++]
            index %= PIXELS
            if (getColor(pixel) == OFF_COLOR) {
                continue
            }
            let neighbors = NEIGHBORS[pixel]
            let happiness = calcHappiness(pixel, neighbors)
            let happiest_neighbor = null
            let max_happiness = 0            
            let offset = int(random(neighbors.length))
            for (let j=neighbors.length - 1; j>=0; j--) {
                let n = (j + offset) % neighbors.length
                if (neighbors[n] == null) {
                    continue
                }
                if (getColor(neighbors[n]) == OFF_COLOR) {
                    let n_happiness = calcHappiness(pixel, NEIGHBORS[neighbors[n]])
                    if (n_happiness >= max_happiness) {
                        max_happiness = n_happiness
                        happiest_neighbor = neighbors[n]
                    }
                }
            }
            if (happiest_neighbor != null && max_happiness >= happiness) {
                moveAgent(pixel, happiest_neighbor)
                steps++           
                if (steps % 500 == 0) {
                    console.log(steps)
                }
                break
            }
            
        } 
    }

    else if (state == HOLD) {
        countdown--
        if (countdown == 0) {
            state = CODA
        }
    }

    else if (state == CODA) {
        let checked = 0
        while (true) {
            if (checked == PIXELS) {
                reset()
                return
            }                    
            let pixel = sequence[index++]
            index %= PIXELS
            checked++
            if (getColor(pixel) != OFF_COLOR) {
                setColor(pixel, OFF_COLOR)
                break
            }
        }
    }

    updateTransitions()

}

function initNeighbors() {
    NEIGHBORS = new Array(PIXELS)
    for (let pixel=0; pixel<PIXELS; pixel++) {
        let neighbors = [   pixel - 1, pixel + 1, 
                        pixel + PIXELS_PER_ROW - 1, pixel + PIXELS_PER_ROW, pixel + PIXELS_PER_ROW + 1,
                        pixel - PIXELS_PER_ROW - 1, pixel - PIXELS_PER_ROW, pixel - PIXELS_PER_ROW + 1
                        ]    
        if (pixel < PIXELS_PER_ROW) {
            neighbors[5] = null
            neighbors[6] = null
            neighbors[7] = null
        }
        if (pixel >= PIXELS - PIXELS_PER_ROW) {
            neighbors[2] = null
            neighbors[3] = null
            neighbors[4] = null
        }
        if (pixel % PIXELS_PER_ROW == 0) {
            neighbors[0] = null
            neighbors[2] = null
            neighbors[5] = null
        }
        if (pixel % PIXELS_PER_ROW == PIXELS_PER_ROW - 1) {
            neighbors[1] = null
            neighbors[4] = null
            neighbors[7] = null
        }
        NEIGHBORS[pixel] = neighbors
    }
}

function calcHappiness(pixel, neighbors) {
    let same = 0
    for (let n=0; n<neighbors.length; n++) {
        if (getColor(neighbors[n]) == getColor(pixel)) {            
            same++
        }
    }    
    return same
}

function moveAgent(current_pixel, new_pixel) {
    setColor(new_pixel, getColor(current_pixel))
    setColor(current_pixel, OFF_COLOR)
}

function shuffleSequence(a) {
    let temp
    let r
    for (let i=0; i<a.length; i++) {
        temp = a[i]
        r = int(random(a.length))
        a[i] = a[r]
        a[r] = temp
    }
}

function getColor(pixel) {
    return pixel_colors[pixel]
}

function resetColors() {
    for (let pixel=0; pixel<PIXELS; pixel++) {
        pixel_colors[pixel] = OFF_COLOR
        previous_pixel_colors[pixel] = OFF_COLOR
    }
}

function setColor(pixel, color) {
    if (color != pixel_colors[pixel]) {
        previous_pixel_colors[pixel] = pixel_colors[pixel]
        pixel_colors[pixel] = color
        transitions[pixel] = TRANSITION
    }
}

function updateTransitions() {    
    for (let pixel=0; pixel<PIXELS; pixel++) {
        if (transitions[pixel] >= 0) {
            let c1 = previous_pixel_colors[pixel]
            let c2 = pixel_colors[pixel]
            let pos = ((TRANSITION + 1) - transitions[pixel]) / (TRANSITION + 1)
            let c = lerpColor(color(c1), color(c2), pos)
            paintColor(pixel, c)
            transitions[pixel]--                        
        }
    }
}

function paintColor(pixel, color) {        
    let y = floor(pixel / ROWS)
    let x = pixel % PIXELS_PER_ROW    
    stroke(255)
    fill(color)    
    circle((x * CELL_SIZE) + (CELL_SIZE / 2), (y * CELL_SIZE) + (CELL_SIZE / 2), CELL_SIZE - 4)
}

let capture = 0

function mouseClicked() {
    if (mouseX < 400 && mouseY < 400 && mouseX > 0 && mouseY > 0) {
        save("capture_" + capture + ".png")
        capture += 1
    }
}

function changeColors() {
    let r1 = parseInt(r1_field.value())
    let g1 = parseInt(g1_field.value())
    let b1 = parseInt(b1_field.value())
    let r2 = parseInt(r2_field.value())
    let g2 = parseInt(g2_field.value())
    let b2 = parseInt(b2_field.value())    
    COLORS = [[r1, g1, b1], [r2, g2, b2]]
    reset()
}
