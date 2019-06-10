'use strict'

/* arduino style: minimize garbage collection and specialty functions */

const TYPES = 2
const HOMOPHILY = 3
const MAX_STEPS = 100000   // equilibrium not guaranteed
const COUNTDOWN = 100
const POPULATION = Math.floor(PIXELS / TYPES)

// declare variables
let pixel
let index
let checked
let same
let steps
let countdown       // delay after finishing


// allocate arrays
let neighbors = new Array(8)
let sequence = new Array(PIXELS)


function start() {
    index = 0
    steps = 0
    countdown = COUNTDOWN
    background(off_color)    
    resetColors()
    for (let s=0; s<sequence.length; s++) {
        sequence[s] = s
    }
    shuffleSequence(sequence)    
    for (let agent=0; agent<POPULATION; agent++) {
        pixel = sequence[agent]
        setColor(pixel, colors[agent % TYPES])
    }
}

function main() {
    checked = 0
    while (true) {                    // each frame, find the next unhappy agent
        if (checked == PIXELS) {
            console.log("checked all")
            start()
            return
        }
        if (steps == MAX_STEPS) {
            console.log("hit max")
            start()
            return
        }
        pixel = sequence[index++]
        index %= PIXELS
        checked++
        calcNeighbors(pixel)
        if (!calcHappiness(pixel)) {
            let r = int(random(neighbors.length))
            for (let j=0; j<neighbors.length; j++) {
                let n = (j + r) % neighbors.length;
                if (neighbors[n] == null) {
                    continue
                }
                if (getColor(neighbors[n]) == off_color) {
                    moveAgent(pixel, neighbors[n])
                    break
                }
            }
            steps++
            return
        }
    } 
}

function calcNeighbors(p) {
    neighbors = [   p - 1, p + 1, 
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
}

function calcHappiness(p) {
    if (getColor(p) == off_color) {
        return true
    }
    same = 0
    for (let n=0; n<neighbors.length; n++) {
        if (neighbors[n] >= 0 && neighbors[n] < PIXELS) {
            if (getColor(neighbors[n]) == getColor(p)) {            
                same++
            }
        }
    }    
    return same < HOMOPHILY ? false : true
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
