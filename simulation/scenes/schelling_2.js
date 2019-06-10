'use strict'

/* arduino style: minimize garbage collection and specialty functions */

const TYPES = 2
const HOMOPHILY = 6
const MAX_STEPS = 1000
const COUNTDOWN = 100
const POPULATION = Math.floor(PIXELS / TYPES)

const STARTUP = 0
const PLAY = 1
const HOLD = 2
const SHUTDOWN = 3


// declare variables
let state
let pixel
let neighbors
let index
let same
let steps
let countdown       // delay after finishing

// allocate arrays
let sequence = new Array(PIXELS)
let transitions = new Array(PIXELS)

function start() {
    reset()
}

function reset() {
    console.log("start")
    state = STARTUP
    index = 0
    steps = 0    
    countdown = COUNTDOWN
    background(off_color)    
    resetColors()
    for (let t=0; t<sequence.length; t++) {
        transitions[t] = false
    }    
    for (let s=0; s<sequence.length; s++) {
        sequence[s] = s
    }
    shuffleSequence(sequence)    
}

function main() {

    if (state == STARTUP) {
        pixel = sequence[index++]
        setColor(pixel, colors[index % TYPES])
        if (index == POPULATION) {
            state = PLAY        
            console.log("play")
        }
    } 

    else if (state == PLAY) {
        while (true) {
            if (steps == MAX_STEPS) {
                console.log("hit max")
                state = SHUTDOWN
                return
            }
            pixel = sequence[index++]
            index %= PIXELS
            if (getColor(pixel) == off_color) {
                continue
            }
            neighbors = getNeighbors(pixel)
            let happiness = calcHappiness(pixel, neighbors)
            let happiest_neighbor = null
            let max_happiness = 0
            // let r = int(random(neighbors.length))
            let r = 0
            for (let j=neighbors.length - 1; j>=0; j--) {
                let n = (j + r) % neighbors.length;
                if (neighbors[n] == null) {
                    continue
                }
                if (getColor(neighbors[n]) == off_color) {
                    let h = calcHappiness(pixel, getNeighbors(neighbors[n]))
                    if (n >= max_happiness) {
                        max_happiness = h
                        happiest_neighbor = neighbors[n]
                    }
                }
            }
            if (happiest_neighbor != null && max_happiness >= happiness) {
                moveAgent(pixel, happiest_neighbor)
                steps++           
                updateTransitions()
                return
            }
            
        } 
    }

    else if (state == HOLD) {
        countdown--
        if (countdown == 0) {
            state = SHUTDOWN
        }
    }

    else if (state == SHUTDOWN) {
        checked = 0
        while (true) {
            if (checked == PIXELS) {
                reset()
                return
            }                    
            pixel = sequence[index++]
            index %= PIXELS
            checked++
            if (getColor(pixel) != off_color) {
                setColor(pixel, off_color)
                return
            }
        }
    }

}

function getNeighbors(p) {
    let neighbors = [   p - 1, p + 1, 
                    p + PIXELS_PER_ROW - 1, p + PIXELS_PER_ROW, p + PIXELS_PER_ROW + 1,
                    p - PIXELS_PER_ROW - 1, p - PIXELS_PER_ROW, p - PIXELS_PER_ROW + 1
                    ]    
    if (p < PIXELS_PER_ROW) {
        neighbors[5] = null
        neighbors[6] = null
        neighbors[7] = null
    }
    if (p >= PIXELS - PIXELS_PER_ROW) {
        neighbors[2] = null
        neighbors[3] = null
        neighbors[4] = null
    }
    if (p % PIXELS_PER_ROW == 0) {
        neighbors[0] = null
        neighbors[2] = null
        neighbors[5] = null
    }
    if (p % PIXELS_PER_ROW == PIXELS_PER_ROW - 1) {
        neighbors[1] = null
        neighbors[4] = null
        neighbors[7] = null
    }
    return neighbors
}

function calcHappiness(p, ns) {
    same = 0
    for (let n=0; n<ns.length; n++) {
        if (getColor(ns[n]) == getColor(p)) {            
            same++
        }
    }    
    return same
    // return same < HOMOPHILY ? false : true
}

function moveAgent(current_space, new_space) {
    setColor(new_space, getColor(current_space))
    setColor(current_space, off_color)
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

function setColor(pixel, color) {
    let y = floor(pixel / ROWS)
    let x = pixel % PIXELS_PER_ROW    
    stroke(255)
    fill(color)    
    circle((x * cell_size) + (cell_size / 2), (y * cell_size) + (cell_size / 2), cell_size - 4)
    pixel_colors[pixel] = color
}

function updateTransitions() {
    for (let p=0; p<PIXELS; p++) {
        if (transitions[p]) {
            transitions[p]--
        }
    }
}

