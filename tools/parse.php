<?php

$mapWidth = 16;
$mapHeight = 16;
$i = imagecreatefrompng('map.png');
$width = imagesx($i);
$height = imagesy($i);
$boxSize = $width/$mapWidth;

for ($x=0; $x<$mapWidth; $x++) {
    for ($y=0; $y<$mapHeight; $y++) {
        $byte = 0;
        $left = imagecolorat($i, $x*$boxSize, $y*$boxSize + $boxSize/2)&0xff;
        $right = imagecolorat($i, $x*$boxSize + $boxSize - 1, $y*$boxSize + $boxSize/2)&0xff;
        $up = imagecolorat($i, $x*$boxSize + $boxSize/2, $y*$boxSize)&0xff;
        $bottom = imagecolorat($i, $x*$boxSize + $boxSize/2, $y*$boxSize + $boxSize - 1)&0xff;
        $center = imagecolorat($i, $x*$boxSize + $boxSize/2, $y*$boxSize + $boxSize/2);
        $b = (($center>>0)&0xff) > 128;
        $g = (($center>>8)&0xff) > 128;
        $r = (($center>>16)&0xff) > 128;

        if ($r && !$g && !$b) { // Red
            $byte |= (1<<4);
        }
        if (!$r && $g && !$b) { // Green
            $byte |= (2<<4);
        }
        if (!$r && !$g && $b) { // Blue
            $byte |= (3<<4);
        }
        if ($r && $g && !$b) { // Yellow
            $byte |= (4<<4);
        }
        if ($r && !$g && $b) { // Purple
            $byte |= (5<<4);
        }

        if ($left < 128) $byte |= 0b1;
        if ($right < 128) $byte |= 0b10;
        if ($up < 128) $byte |= 0b100;
        if ($bottom < 128) $byte |= 0b1000;
        
        // printf("%02d ", $byte);
        echo chr($byte);
    }
    // echo "\n";
}