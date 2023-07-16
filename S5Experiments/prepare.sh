#!/bin/bash

cd ./QS5/

for FILE in *
do
    touch ../QS5_CEGARBox/$FILE;
    sed '1d' $FILE | sed '2d' | sed 's/r//g' > ../QS5_CEGARBox/$FILE;
done
