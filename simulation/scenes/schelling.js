'use strict'

/* arduino style: minimize garbage collection and specialty functions */

const TOLERANCE = .3
const MAX_STEPS = 2000   // equilibrium not guaranteed
const COUNTDOWN = 50
const PERCENT_POP = 2/3

// declare variables
let index
let spot_index
let pixel
let spot
let countdown       // delay after finishing
let steps
let same
let total
let moved

// allocate arrays
let neighbors = new Array(8)
let sequence = new Array(PIXELS)


function start() {    
    index = 0
    spot_index = 0
    steps = 0
    countdown = COUNTDOWN
    background(off_color)    
    resetColors()
    for (let pixel=0; pixel<PIXELS; pixel++) {
        if (random() < PERCENT_POP) {
            if (random() > .5) {
                setColor(pixel, color_1)
            } else {
                setColor(pixel, color_2)
            }
        }
    }
    for (let s=0; s<sequence.length; s++) {
        sequence[s] = s
    }
    shuffleSequence(sequence)    
}

function main() {
    let i = 0
    do {                    // each frame, find the next unhappy agent
        pixel = sequence[index++]
        index %= PIXELS
        i++
        steps++
    } while (i < PIXELS && calcHappiness(pixel) && steps < MAX_STEPS)
    if (i == PIXELS || steps >= MAX_STEPS) {            // either everybody is happy, or we've exceeded the max steps, so let's wrap it up
        countdown--
        if (countdown == 0) {
            start()
        }
    } else {        // if the agent is unhappy, pick a random open square to move it to
        moved = false
        while (true) {
            spot = sequence[spot_index++]
            spot_index %= PIXELS
            if (getColor(spot) == off_color) {
                moveAgent(pixel, spot)
                break
            }
        }
    }
}

function calcHappiness(pixel) {
    if (getColor(pixel) == off_color) {
        return true
    }
    same = 0.0
    total = 8.0
    neighbors = [   pixel - 1, pixel + 1, 
                    pixel + PIXELS_PER_ROW - 1, pixel + PIXELS_PER_ROW, pixel + PIXELS_PER_ROW + 1,
                    pixel - PIXELS_PER_ROW - 1, pixel - PIXELS_PER_ROW, pixel - PIXELS_PER_ROW + 1
                    ]
    for (let n=0; n<8; n++) {
        if (neighbors[n] < 0 || neighbors[n] > PIXELS - 1) {
            total--
            continue
        }
        if (getColor(neighbors[n]) == off_color) {
            total--
            continue
        }
        total++
        if (getColor(neighbors[n]) == getColor(pixel)) {            
            same++
        }
    }    
    if (total == 0) {
        return true
    } else {
        return (same / total > TOLERANCE)
    }
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


