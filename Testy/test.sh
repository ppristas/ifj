#!/bin/bash

  testing_files=$(ls -l "Testy/"| grep "\.test" | awk '{print $9}'| sed 's|*.\.||'
)
  testing_file_count=$(ls -l "Testy/"| grep "\.test" | awk '{print $9}'| sed 's|*.\.||' | wc -l )
  counter=1;
  for i in $testing_files; do
    ./interpret "./Testy/$i";
    return_value=$(echo $?);
    echo $return_value;
    if [[ $counter -ne $testing_file_count ]]; then
      echo "Another test? Y/N"
      read next_file;
    fi;
    if [ "$next_file" != "Y" -a "$next_file" != "y" ]
      then
        exit 1;
    fi;


    ((counter+=1));

  done;
