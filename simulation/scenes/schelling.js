'use strict'

/* arduino style: minimize garbage collection and specialty functions */

const HOMOPHILY = 3
const MAX_STEPS = 2000   // equilibrium not guaranteed
const COUNTDOWN = 100
const TYPES = 2
const POPULATION = Math.floor(PIXELS / TYPES)

// declare variables
let index
let spot_index
let pixel
let spot
let countdown       // delay after finishing
let steps
let same
let i

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
    i = 0
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

function calcHappiness(p) {
    if (getColor(p) == off_color) {
        return true
    }
    same = 0.0
    neighbors = [   p - 1, p + 1, 
                    p + PIXELS_PER_ROW - 1, p + PIXELS_PER_ROW, p + PIXELS_PER_ROW + 1,
                    p - PIXELS_PER_ROW - 1, p - PIXELS_PER_ROW, p - PIXELS_PER_ROW + 1
                    ]
    for (let n=0; n<neighbors.length; n++) {
        if (neighbors[n] > 0 && neighbors[n] < PIXELS) {
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
