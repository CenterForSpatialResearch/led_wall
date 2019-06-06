'use strict'

const TOLERANCE = .3

let neighbors = new Array(8)
let sequence = new Array(PIXELS)

let countdown

function start() {    
    countdown = 50
    background(off_color)    
    for (let pixel=0; pixel<PIXELS; pixel++) {
        if (random() < .5) {
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
    for (let i=0; i<PIXELS; i++) {
        let pixel = sequence[i]
        if (!calcHappiness(pixel)) {
            let p = 0
            let b = 0
            let r = random(PIXELS / 2)
            while (b < r) {
                if (getColor(p) == off_color) {
                    b++
                }
                p = (p + 1) % PIXELS
            }
            moveAgent(pixel, p - 1)
            return
        }
    }
    countdown--
    if (countdown == 0) {
        start()
    }
}

function calcHappiness(pixel) {
    if (getColor(pixel) == off_color) {
        return true
    }
    let same = 0.0
    let total = 8.0
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

function moveAgent(n1, n2) {
    setColor(n2, getColor(n1))
    setColor(n1, off_color)
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


// note: equilibrium not guaranteed

