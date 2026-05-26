#! /usr/bin/env bash
digits=1
expected=0
actual=0
while [ $start -le 7 ]; do
    phsstep = $((5 * digits * digits))
    echo "
WIDTH=2256
HEIGHT=160
WWIDTH=282
WHEIGHT=20
RENDER_FRAMERATE=20
PHYSICS_SUBSTEPS=$phsstep
DIGITS=$digits
SPEED=10
RENDERGRID=0
ENABLESOUND=0
    " > ./config.txt
    read expected actual <<< $( ./phys )
    if [[expected -ne actual]];then
        echo -e "Test case failed: $digits\nphyssteps:$phsstep\n$expected : $actual"
        exit
    fi
    start=$((start+1))
done