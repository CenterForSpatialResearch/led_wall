'use strict'

let pixel
let r

function start() {

}

function main() {
    pixel = int(random(0, PIXELS))
    r = int(random(4))
    if (r == 0) {
        setColor(pixel, color_1)
    } else if (r == 1) {
        setColor(pixel, color_2)
    } else if (r == 2) {
        setColor(pixel, off_color)
    } else {

    }
}