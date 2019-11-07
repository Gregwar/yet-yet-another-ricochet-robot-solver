<?php

$mapWidth = 16;
$mapHeight = 16;
$width = 512;
$height = 512;
$i = imagecreatetruecolor($width, $height);
$boxSize = $width/$mapWidth;

for ($x=0; $x<$mapWidth; $x++) {
    for ($y=0; $y<$mapHeight; $y++) {
        $color = 0xffffff;
        if ($x%2 == $y%2) {
            $color = 0xeeeeee;
        }
        imagefilledrectangle($i, $boxSize*$x, $boxSize*$y, 
                $boxSize*($x+1), $boxSize*($y+1), $color);
    }
}

imagepng($i, 'map.png');