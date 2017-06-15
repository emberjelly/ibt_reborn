#!/usr/sh

FILES=open/*.wav
FOLDER="beat_database/"

for filename in $FILES; do
    #echo "$filename"
        #./MyProgram.exe "$filename" "Logs/$(basename "$filename" .txt)_Log$i.txt"
    output_name=`echo $filename | sed -e 's/wav/csv/g'`
    #echo "$output_name"

    output_string=$FOLDER$output_name

	echo "$output_string"
	./marsyas-release-0.5/build/bin/ibt -i $filename > tmp.csv
	
	tail -n +3 tmp.csv > $output_string
done