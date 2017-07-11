#!/usr/sh

FILES=../../drumless_music/*.wav
FOLDER=""

for filename in $FILES; do
    #echo "$filename"
        #./MyProgram.exe "$filename" "Logs/$(basename "$filename" .txt)_Log$i.txt"
    output_name=`echo $filename | sed -e 's/wav/txt/g'`
    #echo "$output_name"

    output_string=`echo $output_name | sed -e 's/drumless_music/drumless_seb/g'`

	echo "$output_string"
	#./marsyas-release-0.5/build/bin/ibt -i $filename -o $output_string
	DBNBeatTracker single $filename > $output_string

done